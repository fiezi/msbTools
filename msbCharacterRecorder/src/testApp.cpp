#include "testApp.h"
#include "stdio.h"

unsigned char * blackPixels;

//--------------------------------------------------------------
testApp::testApp(){

    vidW           = 640;
    vidH           = 480;
    camId[0]       = 0;
    camId[1]       = 1;
    host           = "192.168.1.67";
	port           = 31841;

	bSending        = true;
	bSwapCamera     = false;
	bInitCameras[0] = false;
	bInitCameras[1] = false;

	vidDrawPos[0].set(30,30);
	vidDrawPos[1].set(34+480,30);
	vidDrawScale[0].set(480,360);
	vidDrawScale[1].set(480,360);


}
//--------------------------------------------------------------
testApp::~testApp(){

    delete videoSourceL;
    delete videoSourceR;

}
//--------------------------------------------------------------
void testApp::setup(){

    //------ general app settings
	ofBackground(30, 30, 30);
	ofSetVerticalSync(true);
	//ofSetFrameRate(45);
    //ofSetLogLevel(OF_LOG_VERBOSE);



	//------  load settings from config file
	loadSettings("config/config.xml");

    blackPixels = new unsigned char[ 3 * vidW * vidH ];
    memset( blackPixels,0,3 * vidW * vidH);

    //------ initialize cameras
    //------ all devices and ids will be listed in console
    videoSourceL			= new ofVideoGrabber();
	videoSourceL->listDevices();
	videoSourceL->setDeviceID(camId[0]);
	bInitCameras[0] =	videoSourceL->initGrabber(vidW, vidH);


    if( camId[1] < 0 )
    {
        videoSourceR = videoSourceL;
    }else{

    videoSourceR			= new ofVideoGrabber();
    videoSourceR->setVerbose(true);
    videoSourceR->setDeviceID(camId[1]);
    bInitCameras[1] =	videoSourceR->initGrabber(vidW, vidH);

    }


	//------ set up manager objects
	rManager.setup( vidW, vidH );
	sessionLoader.setup( vidW, vidH );
	sessionLoader.rManager = &rManager;

	//------ setup osc
    sender.setup( host, port );
}

//--------------------------------------------------------------
void testApp::update(){



    //------ grab video
    if( rManager.isRecording() || rManager.numFrames() == 0 )
    {
        if( bInitCameras[0] )videoSourceL->update();
        if( bInitCameras[1] )videoSourceR->update();
    }

    //------ update manager
    unsigned char * pixelsR;
    unsigned char * pixelsL;

    if( bInitCameras[0] ) pixelsL = videoSourceL->getPixels();
    else                  pixelsL = blackPixels;

    if( bInitCameras[1] ) pixelsR = videoSourceR->getPixels();
    else                  pixelsR = blackPixels;

    if( !bSwapCamera )    rManager.update( pixelsL,pixelsR );
    else                  rManager.update( pixelsR,pixelsL );

    //----- osc
    if(bSending) sendMessages();
}


//--------------------------------------------------------------
void testApp::draw(){

   // ofRect((vidDrawPos[0].x+vidDrawScale[0].x)-15,(vidDrawPos[0].y+vidDrawScale[0].y)-10,20,20);
   // ofRect((vidDrawPos[1].x+vidDrawScale[1].x)-15,(vidDrawPos[1].y+vidDrawScale[1].y)-10,20,20);

    if(rManager.isRecording() || rManager.numFrames() == 0)
    {
        if(!bSwapCamera)
        {
            videoSourceL->draw(rManager.gui.vidDrawPos[0].x,rManager.gui.vidDrawPos[0].y,rManager.gui.vidDrawScale[0].x,rManager.gui.vidDrawScale[0].y);
            videoSourceR->draw(rManager.gui.vidDrawPos[1].x,rManager.gui.vidDrawPos[1].y,rManager.gui.vidDrawScale[1].x,rManager.gui.vidDrawScale[1].y);
        }else{
            videoSourceR->draw(rManager.gui.vidDrawPos[0].x,rManager.gui.vidDrawPos[0].y,rManager.gui.vidDrawScale[0].x,rManager.gui.vidDrawScale[0].y);
            videoSourceL->draw(rManager.gui.vidDrawPos[1].x,rManager.gui.vidDrawPos[1].y,rManager.gui.vidDrawScale[1].x,rManager.gui.vidDrawScale[1].y);
        }

    }

    rManager.draw();


}

//--------------------------------------------------------------
void testApp::keyPressed (int key){


    //------ first check if we are in the text box and update
    if( rManager.isTypingFolder() )
    {
        rManager.enterFolderName( key );
        return;
    }

    //------ if not we are free to use key commands
    switch( key )
    {
        case 'l':   sessionLoader.loadSession("recordings/"+rManager.getOutputDirectory()); break;
        case ' ' :  rManager.toggleRecording(); break;
        case 'p' :  rManager.togglePaused(); break;
        case 's':   rManager.save(); break;
        case 'c':   bSwapCamera ^= true;
                    rManager.swapCameras(); break;

        case 'i':   rManager.setInPos();break;
        case 'o':   rManager.setOutPos();break;
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    //cout << key << endl;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

    rManager.mouseMoved( x,  y);
    lastX = x;
    lastY = y;

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    bool bDragged = rManager.mouseDragged( x,  y,  button);

    if( bDragged ) return;

   lastX = x;
   lastY = y;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    //if( glutGetModifiers() == GLUT_ACTIVE_SHIFT ) cout << "shift " << endl;

    //------ check to load a sequence
    if( rManager.gui.loadButton.update(x,y)  )
        sessionLoader.loadSession("recordings/"+rManager.getOutputDirectory());

    else
        rManager.mousePressed( x,  y,  button);

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    rManager.mouseReleased( x,  y,  button);
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}

//--------------------------------------------------------------
void testApp::loadSettings( string filename )
{
   ofxXmlSettings   xml;

   xml.loadFile(filename);


    vidW  = xml.getValue("camera:width", vidW);
    vidH  = xml.getValue("camera:height", vidH);
    camId[0] = xml.getValue("camera:id_left", camId[0]);
    camId[1] = xml.getValue("camera:id_right", camId[1]);

    port = xml.getValue("osc:port", port);
    host = xml.getValue("osc:host", host);

}


void testApp::sendMessages()
{


    string address = "/pilot";
    ofxOscMessage oscMsg;

    rManager.computeRotation();


//left Arm

    Matrix3f leftShoulder =rManager.getLeftShoulderRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(leftShoulder.data[i]);

    Matrix3f leftElbow = rManager.getLeftElbowRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(leftElbow.data[i]);

//right Arm

    Matrix3f rightShoulder = rManager.getRightShoulderRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(rightShoulder.data[i]);

    Matrix3f rightElbow = rManager.getRightElbowRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(rightElbow.data[i]);

//left leg

    Matrix3f leftHip = rManager.getLeftHipRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(leftHip.data[i]);

    Matrix3f leftKnee = rManager.getLeftKneeRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(leftKnee.data[i]);

//right leg

    Matrix3f rightHip = rManager.getRightHipRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(rightHip.data[i]);

    Matrix3f rightKnee = rManager.getRightKneeRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(rightKnee.data[i]);

//spine
    Matrix3f neck =rManager.getNeckRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(neck.data[i]);

    Matrix3f torso =rManager.getTorsoRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(torso.data[i]);

    Matrix3f pelvis =rManager.getPelvisRotation();
    address.append("/matrix3f");
    for (int i=0;i<9;i++)
        oscMsg.addFloatArg(pelvis.data[i]);

    oscMsg.setAddress(address);
    sender.sendMessage(oscMsg);

    //cout << "yay! sending..." << endl;
}
