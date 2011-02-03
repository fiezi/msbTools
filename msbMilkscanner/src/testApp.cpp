#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){

    //fonts
	verdana.loadFont("verdana.ttf",8, false, true);
	verdana.setLineHeight(20.0f);

    //interval
    intervalTime=100;

    fluid = "ink";

	camWidth 		= 640;	// try to grab at this size.
	camHeight 		= 480;

	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);

    vidGrabber.listDevices();

    ofVideoGrabber bla;

    bla.setVerbose(true);
    bla.setDeviceID(0);
    bla.initGrabber(camWidth,camHeight);

	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_RGB);

    depthMap     	= new unsigned char[camWidth*camHeight*3];
    colorMap        = new unsigned char[camWidth*camHeight*3];
    fullMap         = new unsigned char[camWidth*camHeight*4];

    inkScanTexture.allocate(camWidth,camHeight, GL_RGB);

    threshhold=255;
    //threshhold=255;
    fileRevision=0;
    ofBackground(0, 0, 0);

//    currentStep=255;
    currentStep=0;

    bStepping=false;
    bJustStepped=false;
    bIsRecording=false;
    bIsInterval=false;
    bFrontToBack=false;

}


//--------------------------------------------------------------
void testApp::update(){


    unsigned char currentRed=0;
    unsigned char currentGreen=0;
    unsigned char currentBlue=0;

    if (currentStep<256 && currentStep >= 0)
     {
     currentBlue=currentStep;
     currentRed=currentStep;
     currentGreen=currentStep;
     }
    else
      bIsRecording=false;

	vidGrabber.grabFrame();

	//if (vidGrabber.isFrameNew()){

		int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = vidGrabber.getPixels();

		for (int i = 0; i < totalPixels; i+=3){

			bool isInteresting=false;

			if (fluid=="ink" && pixels[i]+pixels[i+1]+pixels[i+2]>threshhold)
			  isInteresting=true;
            if (fluid=="milk" && pixels[i]+pixels[i+1]+pixels[i+2]<threshhold)
              isInteresting=true;

			if (isInteresting){
                videoInverted[i] = 255;
                videoInverted[i+1] = 0;
                videoInverted[i+2] = 0;

                if (bIsRecording || bJustStepped){
                    if (bFrontToBack){
                        if (depthMap[i]<currentRed){
                            depthMap[i]=currentRed;
                            depthMap[i+1]=currentGreen;
                            depthMap[i+2]=currentBlue;
                        }
                    }else{
                        depthMap[i]=currentRed;
                        depthMap[i+1]=currentGreen;
                        depthMap[i+2]=currentBlue;
                    }
                }
            }else{
                videoInverted[i] = 0;
                videoInverted[i+1] = 0;
                videoInverted[i+2] = 0;
            }
        }

		videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
		inkScanTexture.loadData(depthMap, camWidth,camHeight, GL_RGB);

        if (bIsRecording && currentStep%16==0 && currentStep>0){

            ofImage* nextCutOut = new ofImage;
            nextCutOut->setFromPixels(videoInverted,640,480,OF_IMAGE_COLOR,true);
            cutouts.push_back(nextCutOut);
        }

    bJustStepped=false;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	vidGrabber.draw(10,10);
	videoTexture.draw(30+camWidth,10,320,240);
    inkScanTexture.draw(30+camWidth,260,320,240);


	deltaTime=glutGet(GLUT_ELAPSED_TIME)-currentTime;
    currentTime=glutGet(GLUT_ELAPSED_TIME);

    verdana.drawString("OpenFrameworks Rocks! \n\nSPACE - start/stop recording \n B     - Flip Back to Front recording \n R     - Reset \n F     - Save File \n C     - Make Cutouts ", 100,550);

    char threshString [255];
    sprintf(threshString,"current Threshhold is: %i (change with '+/-'),\n also, we're scanning with %s (change with 't') \n and stepping is set to %i (change with 'n')",threshhold, fluid.c_str(),bStepping);
    verdana.drawString(threshString,400,550);

	char timeString [255];
    sprintf(timeString,"time passed: %f  currentStep: %i", currentTime/1000.0f, currentStep);
	verdana.drawString(timeString, 400,700);
	if (bIsRecording)
	  {
	  verdana.drawString("RECORDING...", 100,750);
	  if (int(currentTime)%int(intervalTime)<deltaTime)
	    if (bFrontToBack)
            currentStep--;
	    else
            currentStep++;
	  }
}


void testApp::doStep(){

}

void testApp::generateAlphaImage(){

int totalPixels = camWidth*camHeight*3;

int current=0;

for (int i=0;i<totalPixels;i+=3)
    {
    fullMap[current++]=colorMap[i];
    fullMap[current++]=colorMap[i+1];
    fullMap[current++]=colorMap[i+2];
    fullMap[current++]=depthMap[i];
    }
endImage.setFromPixels(fullMap,camWidth,camHeight,OF_IMAGE_COLOR_ALPHA);
string filename="inkScanImage";

        std::string s;
        std::stringstream out;
        out << fileRevision;
        s = out.str();

        filename.append(s);
        filename.append(".tga");

endImage.saveImage(filename);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    if (key=='+')
      threshhold++;

    if (key=='-')
      threshhold--;



}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

	// in fullscreen mode, on a pc at least, the
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...

	if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
	}

    if (key == ' ' && !bStepping){

        bIsRecording=!bIsRecording;
        if (bIsRecording)
          {
          //generate color image
          unsigned char * pixels = vidGrabber.getPixels();
          for (int i=0;i<camWidth*camHeight*3;i++)
            colorMap[i]=pixels[i];
          }
    }

    if (key == ' ' && bStepping){

        currentStep++;
        bJustStepped=true;
    }

    if (key == 'r'){

        //clear depthMap
        for (int i=0;i<camWidth*camHeight*3;i++)
          depthMap[i]=0;

        if (bFrontToBack)
            currentStep=255;
        else
            currentStep=0;
        fileRevision++;

        cutouts.clear();
    }

    if (key=='c'){
        createCutouts();
    }

    if (key == 'f' || key == 'F'){
        generateAlphaImage();
        createXML();
    }

    if (key=='b' || key=='B'){
		if (bFrontToBack){
			currentStep=0;
		}
		else {
			currentStep=255;
		}

        bFrontToBack=!bFrontToBack;
    }

    if (key== 'l' || key== 'L'){
        endImage.loadImage("firstTest.bmp");
        depthMap=endImage.getPixels();
    }

    if (key=='t' || key=='T')
      if (fluid=="ink")
        fluid="milk";
      else
        fluid="ink";

    if (key=='n' || key =='N')
      bStepping=!bStepping;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}


void testApp::createCutouts(){


    for (int i=0;i<cutouts.size();i++){
        string filename="inkScanImage";

                std::string s;
                std::stringstream out;
                out << fileRevision << "_cut_" << i;
                s = out.str();
                filename.append(s);
                filename.append(".tga");

        cutouts[i]->saveImage(filename);
    }
}

void testApp::createXML(){

//don't do xml for now!
return;

cout << "doing the xml thing..." << endl;
string filename="inkScanData";

        std::string s;
        std::stringstream out;
        out << fileRevision;
        s = out.str();

        filename.append(s);
        filename.append(".xml");

TiXmlDocument doc;
TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
doc.LinkEndChild( decl );
TiXmlElement * root = new TiXmlElement( "Moviesandbox" );
doc.LinkEndChild( root );

TiXmlElement * element = new TiXmlElement( "Actor" );
element->SetAttribute("name", "string drawing");
TiXmlText * text = new TiXmlText( "14ParticleSystem" );
element->LinkEndChild( text );

TiXmlElement * property=new TiXmlElement("LOCATION");
string value = "vec3f 0.0 0.0 0.0";
property->LinkEndChild ( new TiXmlText( value));
element->LinkEndChild( property );

property=new TiXmlElement("ROTATION");
value = "vec3f 0.0 0.0 0.0";
property->LinkEndChild ( new TiXmlText( value));
element->LinkEndChild( property );

property=new TiXmlElement("COLOR");
value = "vec4f 1.0 1.0 1.0";
property->LinkEndChild ( new TiXmlText( value));
element->LinkEndChild( property );



for (int i=0;i<camWidth*camHeight*3;i+=3)
    {
if (depthMap[i+2]>2)
  {

    float actualImagePosition=float(i)/float(3);
    actualImagePosition=floor(actualImagePosition);  //no commas in image position...
    float currentRow=floor(actualImagePosition/float(camWidth));


    //this is from testing
    float zDistance=(100.0f/currentStep)*100.0f;

    float locX=(float((i/3)%(camWidth)))/(float(50));
    float locY=currentRow/50;
    float locZ=float(depthMap[i+2])/zDistance;
    float locW=0.03f;

    float colR=colorMap[i];
    float colG=colorMap[i+1];
    float colB=colorMap[i+2];
    float colA=1.0f;

    colR=colR/255.0f;
    colG=colG/255.0f;
    colB=colB/255.0f;
    cout << "current red: " << colR << endl;

    TiXmlElement * pElement = new TiXmlElement("particle");

        TiXmlElement * mElement = new TiXmlElement("location");
        char loc[80];
        sprintf(loc,"vec4f %f %f %f %f", locX, locY, locZ, locW);
        mElement->LinkEndChild(new TiXmlText(loc));
        pElement->LinkEndChild( mElement );

        mElement = new TiXmlElement("color");
        char col[50];
        sprintf(col,"vec4f %f %f %f %f", colR, colG, colB, colA);
        mElement->LinkEndChild(new TiXmlText(col));
        pElement->LinkEndChild( mElement );

        mElement = new TiXmlElement("birth");
        char bir[50];
        sprintf(bir,"float %f", 0.0f);
        mElement->LinkEndChild(new TiXmlText(bir));
        pElement->LinkEndChild( mElement );

    element->LinkEndChild( pElement );
  }
    }

root->LinkEndChild(element);

string saveString=filename;

cout << "saving filename: " << saveString << endl;

doc.SaveFile( saveString );

}
