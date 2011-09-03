#include "testApp.h"
#include "actor.h"
#include "assignButton.h"
#include "textInputButton.h"
#include "msbLight.h"




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

	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_RGB);

    depthMap     	= new unsigned char[camWidth*camHeight*3];
    colorMap        = new unsigned char[camWidth*camHeight*3];
    fullMap         = new unsigned char[camWidth*camHeight*4];

    inkScanTexture.allocate(camWidth,camHeight, GL_RGB);


    keySource= new ofxCvColorImage;
    keySource->allocate(camWidth,camHeight);

    chromaKey=new ChromaKeyer;

    threshhold=100;
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
    bLumaKey=true;

    mouseButton=0;

	//set background to black
	ofBackground(128, 128, 128);

    filename="scan.tga";

    msbSetup();

}


void testApp::msbSetup(){

    registerProperties();

    renderer=Renderer::getInstance();
    input=Input::getInstance();

    renderer->loadPreferences();

    renderer->setup();

    renderer->camActor=new Actor;
    renderer->camActor->setLocation(Vector3f(0,0,-5));
    renderer->camActor->postLoad();

    //Adding MSB content
    BasicButton *but;

    but= new AssignButton;
    but->location.x=500;
    but->location.y=320;
    but->scale.y=64;
    but->scale.x=128;
    but->name="scan";
    but->bDrawName=true;
    but->tooltip="start scanning";
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->color=COLOR_WHITE;
    but->setup();
    but->parent=this;
    renderer->buttonList.push_back(but);
    startButton=but;

    but= new AssignButton;
    but->location.x=636;
    but->location.y=320;
    but->scale.y=64;
    but->scale.x=128;
    but->name="reset";
    but->bDrawName=true;
    but->tooltip="reset scan buffers";
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->color=COLOR_WHITE;
    but->setup();
    but->parent=this;
    renderer->buttonList.push_back(but);


    but= new AssignButton;
    but->location.x=500;
    but->location.y=392;
    but->scale.x=128;
    but->scale.y=32;
    but->name="save";
    but->bDrawName=true;
    but->tooltip="save TGA file";
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->color=Vector4f(0.7,0.7,0.7,1.0);
    but->setup();
    but->parent=this;
    renderer->buttonList.push_back(but);


    but= new TextInputButton;
    but->location.x=500;
    but->location.y=430;
    but->scale.x=128;
    but->scale.y=16;
    but->name="save As...";
    but->bDrawName=true;
    but->buttonProperty="FILENAME";
    but->tooltip="string scan.tga";
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->color=Vector4f(0.7,0.7,0.7,1.0);
    but->setup();
    but->parent=this;
    renderer->buttonList.push_back(but);


    SliderButton* slBtn = new SliderButton;
    slBtn->location.x=340;
    slBtn->location.y=290;
    slBtn->scale.x=640;
    slBtn->scale.y=16;
    slBtn->bVertical=false;
    slBtn->name="threshhold";
    slBtn->bDrawName=true;
    slBtn->tooltip="slide to change";
    slBtn->setLocation(slBtn->location);
    slBtn->textureID="icon_flat";
    slBtn->color=Vector4f(0.7,0.7,0.7,1.0);
    slBtn->setup();
    slBtn->parent=this;
    renderer->buttonList.push_back(slBtn);
    threshholdSlider=slBtn;

    slBtn = new SliderButton;
    slBtn->location.x=340;
    slBtn->location.y=300;
    slBtn->scale.x=640;
    slBtn->scale.y=16;
    slBtn->bVertical=false;
    slBtn->name="hue threshhold";
    slBtn->bDrawName=true;
    slBtn->tooltip="slide to change";
    slBtn->setLocation(slBtn->location);
    slBtn->textureID="icon_flat";
    slBtn->color=Vector4f(0.7,0.7,0.7,1.0);
    slBtn->setup();
    slBtn->parent=this;
    renderer->buttonList.push_back(slBtn);
    hueThreshholdSlider=slBtn;

    slBtn = new SliderButton;
    slBtn->location.x=340;
    slBtn->location.y=310;
    slBtn->scale.x=640;
    slBtn->scale.y=16;
    slBtn->bVertical=false;
    slBtn->name="sat threshhold";
    slBtn->bDrawName=true;
    slBtn->tooltip="slide to change";
    slBtn->setLocation(slBtn->location);
    slBtn->textureID="icon_flat";
    slBtn->color=Vector4f(0.7,0.7,0.7,1.0);
    slBtn->setup();
    slBtn->parent=this;
    renderer->buttonList.push_back(slBtn);
    satThreshholdSlider=slBtn;

    slBtn = new SliderButton;
    slBtn->location.x=340;
    slBtn->location.y=320;
    slBtn->scale.x=640;
    slBtn->scale.y=16;
    slBtn->bVertical=false;
    slBtn->name="vel threshhold";
    slBtn->bDrawName=true;
    slBtn->tooltip="slide to change";
    slBtn->setLocation(slBtn->location);
    slBtn->textureID="icon_flat";
    slBtn->color=Vector4f(0.7,0.7,0.7,1.0);
    slBtn->setup();
    slBtn->parent=this;
    renderer->buttonList.push_back(slBtn);
    velThreshholdSlider=slBtn;

    but= new AssignButton;
    but->location.x=500;
    but->location.y=500;
    but->scale.x=72;
    but->scale.y=32;
    but->name="using ink";
    but->bDrawName=true;
    but->tooltip="click to change to milk";
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->color=COLOR_BLACK;
    but->setup();
    but->parent=this;
    renderer->buttonList.push_back(but);

    but= new AssignButton;
    but->location.x=580;
    but->location.y=500;
    but->scale.x=72;
    but->scale.y=32;
    but->name="continuous";
    but->bDrawName=true;
    but->tooltip="click to change";
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->color=COLOR_YELLOW;
    but->setup();
    but->parent=this;
    renderer->buttonList.push_back(but);


    //heightfield based on videoTexture from OF
    patchActor = new Actor;
    patchActor->setLocation(Vector3f(-1.5,-0.5,-5));
    patchActor->postLoad();
    patchActor->color=Vector4f(1,1,1,1);
    patchActor->drawType=DRAW_POINTPATCH;
    patchActor->particleScale=250;
    patchActor->scale=Vector3f(2,-1.5,1);
    patchActor->bTextured=true;
    patchActor->textureID="NULL";
    patchActor->sceneShaderID="heightfield";

    renderer->actorList.push_back(patchActor);
    renderer->layerList[0]->actorList.push_back(patchActor);


}


void testApp::registerProperties(){

    createMemberID("FILENAME",&filename,this);
}
//--------------------------------------------------------------
void testApp::update(){

	vidGrabber.grabFrame();

	//if (vidGrabber.isFrameNew()){

    doStep();

    bJustStepped=false;


    //MSBStuff
    renderer->update();
    patchActor->addRotation(0.5,Vector3f(0.3,1.0,0.0));
    patchActor->ofTexturePtr=&inkScanTexture;

}

//--------------------------------------------------------------
void testApp::draw(){


    //MSBStuff
    renderer->draw();

    glPushMatrix();
    glTranslatef(0,32,0);
    ofSetColor(255,255,255);
    glScalef(0.5,0.5,1.0);
	vidGrabber.draw(20,20);
	glScalef(2.0,2.0,1.0);
	videoTexture.draw(20+camWidth/2.0f,10,320,240);
    inkScanTexture.draw(40+camWidth,10,320,240);

    //ofSetColor(chromaKey->R,chromaKey->G,chromaKey->B,255);

    glTranslatef(350,0,0);
	deltaTime=glutGet(GLUT_ELAPSED_TIME)-currentTime;
    currentTime=glutGet(GLUT_ELAPSED_TIME);

	if (bIsRecording)
	  {
	  verdana.drawString("RECORDING...", 100,750);
	  if (int(currentTime)%int(intervalTime)<deltaTime)
	    if (bFrontToBack)
            currentStep--;
	    else
            currentStep++;
	  }
    glPopMatrix();

    ofSetColor(chromaKey->R,chromaKey->G, chromaKey->B);
    ofRect(20,260,100,100);

}


void testApp::trigger(Actor* other){

    if (other==threshholdSlider){
        threshhold=threshholdSlider->sliderValue* 768;
    }

    if (other==hueThreshholdSlider){
        chromaKey->tH=hueThreshholdSlider->sliderValue*255.0;
    }

    if (other==satThreshholdSlider){
        chromaKey->tS=satThreshholdSlider->sliderValue*255.0;
    }

    if (other==velThreshholdSlider){
        chromaKey->tV=velThreshholdSlider->sliderValue*255.0;
    }

    if (other->name=="scan" || other->name=="scanning"){
        bIsRecording=!bIsRecording;
        startButton->tooltip="start scanning";
        startButton->name="scan";
        startButton->color=COLOR_WHITE;
        if (bIsRecording){
            startButton->color=COLOR_RED;
            startButton->name="scanning";
            startButton->tooltip="stop scanning";
            //generate color image
            unsigned char * pixels = vidGrabber.getPixels();
            for (int i=0;i<camWidth*camHeight*3;i++)
                colorMap[i]=pixels[i];
        }

    }

    if (other->name=="step"){
        currentStep++;
        bJustStepped=true;
    }

    if (other->name=="reset"){
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


    if (other->name=="using ink"){
        fluid="milk";
        other->name="using milk";
        ((BasicButton*)other)->tooltip="click to change to ink";
        other->color=COLOR_WHITE;
        return;
    }

    if (other->name=="using milk"){
        fluid="ink";
        other->name="using ink";
        ((BasicButton*)other)->tooltip="click to change to milk";
        other->color=COLOR_BLACK;
        return;
    }

    if (other->name=="save" || other->name=="save As..."){
        generateAlphaImage();
    }

    if (other->name=="continuous"){
        bStepping=false;
        other->name="stepping";
        startButton->name="step";
        startButton->color=COLOR_YELLOW;
        bIsRecording=false;
        return;
    }

    if (other->name=="stepping"){
        bStepping=true;
        other->name="continuous";
        startButton->name="scan";
        startButton->color=COLOR_WHITE;
        bIsRecording=false;
        return;
    }
}

void testApp::doStep(){

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

    int totalPixels = camWidth*camHeight*3;
    unsigned char * pixels = vidGrabber.getPixels();

    //key and transfer
    keySource->setFromPixels(pixels,camWidth,camHeight);
    keySource->convertRgbToHsv();

    unsigned char * pixelsHSV = keySource->getPixels();

    for (int i = 0; i < totalPixels; i+=3){

        bool isInteresting=false;

        //luminance key

        if (bLumaKey){

            if (fluid=="ink" && pixels[i]+pixels[i+1]+pixels[i+2]>threshhold)
              isInteresting=true;
            if (fluid=="milk" && pixels[i]+pixels[i+1]+pixels[i+2]<threshhold)
              isInteresting=true;

        }else{


            //chroma key
            if( pixelsHSV[i] >= chromaKey->H-chromaKey->tH && pixelsHSV[i] <= chromaKey->H+chromaKey->tH &&
                pixelsHSV[i+1] >= chromaKey->S-chromaKey->tS && pixelsHSV[i+1] <= chromaKey->S+chromaKey->tS &&
                pixelsHSV[i+1] >= chromaKey->V-chromaKey->tV && pixelsHSV[i+1] <= chromaKey->V+chromaKey->tV ){

                isInteresting=false;
            }else{
                isInteresting=true;
            }

        }
        //transfer
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
endImage.saveImage(filename);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    input->normalKeyDown(key, mouseX, mouseY);
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){


    input->keyUp(key,mouseX,mouseY);

    if (!input->bTextInput){

        if (key==' '){
            bIsRecording=!bIsRecording;
            startButton->tooltip="start scanning";
            startButton->name="scan";
            startButton->color=COLOR_WHITE;
            if (bIsRecording){
                startButton->color=COLOR_RED;
                startButton->name="scanning";
                startButton->tooltip="stop scanning";
                //generate color image
                unsigned char * pixels = vidGrabber.getPixels();
                for (int i=0;i<camWidth*camHeight*3;i++)
                    colorMap[i]=pixels[i];
            }
        }

      if (key=='s' || key=='S')
            vidGrabber.videoSettings();

    }



}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

    input->moveMouse(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    input->dragMouse(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    input->pressedMouse(button,MOUSEBTNPRESSED,x,y);
    mouseButton=button;

    if (x>10 && x<10+camWidth/2.0 && y>42 && y<42+camHeight/2.0){

        x-=10;
        y-=42;

        x*=2;
        y*=2;

        unsigned char* myPix = vidGrabber.getPixels();

        chromaKey->R =myPix[camWidth * 3 * y + x * 3];
        chromaKey->G =myPix[camWidth * 3 * y + x * 3 +1];
        chromaKey->B =myPix[camWidth * 3 * y + x * 3 +2];

        chromaKey->setFromRGB(chromaKey->R,chromaKey->G,chromaKey->B);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(){

    input->pressedMouse(mouseButton,MOUSEBTNRELEASED,mouseX,mouseY);
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

}
