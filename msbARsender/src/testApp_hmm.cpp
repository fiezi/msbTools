#include "testApp.h"
#include "stdio.h"


//--------------------------------------------------------------
testApp::testApp(){

    bUseVideoFiles  = false;
    videoFile       ="fingers.mov";
    cameraId        = 0;
	camW            = 640;
	camH            = 480;

	host            = "localhost";
	port            = 12345;

	bSending        = true;
}

//--------------------------------------------------------------
void testApp::setup(){


	//------ load configuration
    loadConfig("config.xml");

    //------ video settings
	if (bUseVideoFiles == true){

		videoSource			= new ofVideoPlayer();
		((ofVideoPlayer *)videoSource)->loadMovie(videoFile);
		((ofVideoPlayer *)videoSource)->play();
		((ofVideoPlayer *)videoSource)->setLoopState(OF_LOOP_NORMAL);
		((ofVideoPlayer *)videoSource)->setSpeed(1);

	} else {

		videoSource			= new ofVideoGrabber();
		((ofVideoGrabber *)videoSource)->listDevices();
		((ofVideoGrabber *)videoSource)->setDeviceID(0);
		((ofVideoGrabber *)videoSource)->initGrabber(camW, camH);
	}


    //------ setup ar toolkit object
    int width   = 640;
	int height  = 480;

	colorPixels.allocate(width, height);
	grayPixels.allocate(width, height);

	useBCH = false; //true;

    arTracker.setup(width,height,useBCH);


    //------ setup osc
    sender.setup( host, port );
}

//--------------------------------------------------------------
void testApp::update(){

    videoSource->update();


    colorPixels.setFromPixels(videoSource->getPixels(), colorPixels.width, colorPixels.height);
    grayPixels.setFromColorImage(colorPixels);

    arTracker.update(grayPixels.getPixels());


    if(bSending) sendMessages();
}


//--------------------------------------------------------------
void testApp::draw(){

    ofSetColor(0xffffff);

	grayPixels.draw(0, 0);

    arTracker.draw();


}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    if( key == 'x' ) ((ofVideoGrabber *)videoSource)->videoSettings();
    if( key == 'o' ) bSending = !bSending;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}


void testApp::loadConfig(string filename)
{
    ofxXmlSettings xml;

    xml.loadFile( filename );
    cameraId = xml.getValue("config:camera:id",cameraId);
    camW     = xml.getValue("config:camera:w",camW);
    camH     = xml.getValue("config:camera:h",camH);

    if( xml.getValue("config:input:type","") == "camera" ) bUseVideoFiles = false;
    else bUseVideoFiles = true;

    videoFile   = xml.getValue("config:video:file",videoFile);

    host    = xml.getValue("config:osc:host",host);
    port    = xml.getValue("config:osc:port",port);
}


void testApp::sendMessages()
{
    int numMarkers = arTracker.getNumMarkers();
    float arMatrix[16];

    memset( arMatrix, 0, 16 * sizeof(float));

    string address = "/pilot";
    ofxOscMessage oscMsg;

    // send osc message with matrix values
    for( int i = 0; i < numMarkers; i++)
    {

        address.append("/int/matrix4f");

        int markerId = arTracker.getTransMatrix(i,arMatrix);

        oscMsg.addIntArg(markerId);

        for( int i = 0; i < 16; i++)
            oscMsg.addFloatArg(arMatrix[i]);

    }

    oscMsg.setAddress(address);
    sender.sendMessage(oscMsg);
}
