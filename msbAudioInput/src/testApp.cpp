#include "testApp.h"
#include "assignButton.h"
#include "textInputButton.h"
#include "msbLight.h"
#include "stdio.h"

//--------------------------------------------------------------
void testApp::setup(){


    registerProperties();

    oscPath="/pilot/float";

    channel=1;
    bSending=false;

    //MSB setup
    renderer=Renderer::getInstance();
    input=Input::getInstance();

    renderer->loadPreferences();

    renderer->setup();

    TextInputButton* txBtn;

    txBtn=new TextInputButton;
    txBtn->location.x= 10;
    txBtn->location.y= 300;
    txBtn->scale.x=100;
    txBtn->color=Vector4f(0.6,0.6,0.6,1.0);
    txBtn->name="Channel:";
    txBtn->bDrawName=true;
    txBtn->buttonProperty="CHANNEL";
    txBtn->parent=this;
    txBtn->textureID="icon_flat";
    txBtn->setup();
    renderer->buttonList.push_back(txBtn);


    txBtn=new TextInputButton;
    txBtn->location.x= 10;
    txBtn->location.y= 340;
    txBtn->scale.x=100;
    txBtn->color=Vector4f(0.6,0.6,0.6,1.0);
    txBtn->name="OSCpath:";
    txBtn->bDrawName=true;
    txBtn->buttonProperty="OSCPATH";
    txBtn->parent=this;
    txBtn->textureID="icon_flat";
    txBtn->setup();
    renderer->buttonList.push_back(txBtn);

    AssignButton* aBtn;
    aBtn=new AssignButton;
    aBtn->location.x= 10;
    aBtn->location.y= 380;
    aBtn->scale.x=100;
    aBtn->color=Vector4f(0.6,0.6,0.6,1.0);
    aBtn->name="start sending";
    aBtn->bDrawName=true;
    aBtn->parent=this;
    aBtn->textureID="icon_flat";
    aBtn->setup();
    renderer->buttonList.push_back(aBtn);

	ofBackground(255,255,255);

	// 0 output channels,
	// 2 input channels
	// 22050 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)

	left = new float[256];

	bufferCounter = 0;
	drawCounter = 0;

    ofSoundStreamListDevices();
	ofSoundStreamSetup(0,1,this, 22050, 256, 4);

}

void testApp::registerProperties(){

    createMemberID("CHANNEL",&channel, this);
    createMemberID("OSCPATH",&oscPath, this);
}

//--------------------------------------------------------------
void testApp::update(){

    renderer->update();

}


//--------------------------------------------------------------
void testApp::draw(){

    renderer->draw();

	// draw the left:
	ofSetHexColor(0xFFFFFF);
	for (int i = 0; i < 256; i++){
		ofLine(10+i,200,10+i,200+left[i]*100.0f);
	}

	ofSetHexColor(0x333333);
	drawCounter++;
	char reportString[255];
	sprintf(reportString, "buffers received: %i\ndraw routines called: %i\n", bufferCounter,drawCounter);
	ofDrawBitmapString(reportString,10,480);
}


void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i];
	}
	bufferCounter++;

    ofxOscMessage m;
	m.setAddress( oscPath );

	float mouth=0.0f;
	for (int i=0;i<256;i++)
        mouth+=abs(left[i]);

    if (mouth/255.0f>0.1f){
        m.addFloatArg( mouth/255.0f );
    }
    else{
        m.addFloatArg( 0.0f );
    }

    if (sender && bSending)
        sender->sendMessage( m );
}


void testApp::trigger(Actor* other){

    if (other->name=="start sending"){
        if (!bSending){
            sender=new ofxOscSender;
            sender->setup(HOST,PORT+channel);
            other->color=COLOR_RED;
        }else{
            other->color=COLOR_WHITE;
            delete(sender);
            sender=NULL;
        }
        bSending=!bSending;
    }
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    input->normalKeyDown(key,mouseX,mouseY);
    input->specialKeyDown(key,mouseX,mouseY);

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

    input->keyUp(key,mouseX,mouseY);
    input->specialKeyUp(key,mouseX,mouseY);

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


    input->pressedMouse(button,0,x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){


    input->pressedMouse(button,1,x,y);

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}

