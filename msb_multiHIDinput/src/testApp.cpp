#include "testApp.h"
#include "actor.h"
#include "assignButton.h"
#include "textInputButton.h"
#include "msbLight.h"



//--------------------------------------------------------------
void testApp::setup(){


    joy=NULL;
    channel=1;
    ipAddress="127.0.0.1";
    bSending=false;

    for (int i=0;i<12;i++)
        btnEventNames.push_back("trigger");

    registerProperties();

    //MSB setup
    renderer=Renderer::getInstance();
    input=Input::getInstance();

    renderer->loadPreferences();

    renderer->setup();

    renderer->camActor=new Actor;
    renderer->camActor->setLocation(Vector3f(0,0,-5));
    renderer->camActor->postLoad();


    //OF stuff
    ofBackground(128, 128, 128);

    setupInterface();
}


void testApp::setupInterface(){

    //enumerate all joysticks

    jsInit();

    jsJoystick* myJoystick;
    myJoystick = new jsJoystick;

    BasicButton *but;

    int i=0;
    while (myJoystick->notWorking()==JS_FALSE){

            but= new AssignButton;
            but->location.x=10;
            but->location.y=32 + i * 17;
            but->scale.x=128;
            but->scale.y=16;
            but->name=myJoystick->getName();
            but->bDrawName=true;
            but->tooltip=myJoystick->getName();
            but->setLocation(but->location);
            but->textureID="icon_flat";
            but->color=COLOR_WHITE;
            but->setup();
            but->parent=this;
            renderer->buttonList.push_back(but);
            joyList.push_back(but);

            delete(myJoystick);

        i++;
        myJoystick=new jsJoystick(i);
    }

    but= new TextInputButton;
    but->location.x=200;
    but->location.y=200;
    but->scale.x=64;
    but->scale.y=16;
    but->name="Receiver:";
    but->bDrawName=true;
    but->tooltip=ipAddress;
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->color=COLOR_WHITE;
    but->setup();
    but->parent=this;
    but->buttonProperty="IPADDRESS";
    renderer->buttonList.push_back(but);

    char nm[4];

    sprintf(nm,"%d",channel);
    but= new TextInputButton;
    but->location.x=200;
    but->location.y=220;
    but->scale.x=64;
    but->scale.y=16;
    but->name="Channel:";
    but->bDrawName=true;
    but->tooltip=nm;
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->color=COLOR_WHITE;
    but->setup();
    but->parent=this;
    but->buttonProperty="CHANNEL";
    renderer->buttonList.push_back(but);


    for (int i=0;i<12;i++){

        sprintf(nm,"%d",i+1);
        but= new TextInputButton;
        but->location.x=400;
        but->location.y=220 + i*17;
        but->scale.x=16;
        but->scale.y=16;
        but->name=nm;
        but->bDrawName=true;
        but->tooltip=btnEventNames[i];
        but->setLocation(but->location);
        but->textureID="icon_flat";
        but->color=COLOR_WHITE;
        but->setup();
        but->parent=this;
        but->buttonProperty="NULL";
        renderer->buttonList.push_back(but);
        eventList.push_back(but);
    }

}

void testApp::registerProperties(){

    createMemberID("IPADDRESS",&ipAddress,this);
    createMemberID("CHANNEL",&channel,this);

}

//--------------------------------------------------------------
void testApp::update(){

    renderer->update();

    if (bSending && joy){
        int myInt;
        float myAxis[10];
        joy->read(&myInt, &myAxis[0]);

        ofxOscMessage myMessage;
        myMessage.addFloatArg(myAxis[0]);
        myMessage.addFloatArg(myAxis[1]);
        myMessage.addFloatArg(myAxis[2]);

        myMessage.setAddress("/pilot/float/float/float");
        osc_sender.sendMessage(myMessage);

    }
}


//--------------------------------------------------------------
void testApp::draw(){

    renderer->draw();


    if (joy){
        int myInt;
        float myAxis[10];
        joy->rawRead(&myInt, &myAxis[0]);

        char myString[512];

        for (int i=0;i<joy->getNumAxes();i++){
            sprintf(myString,"Axis %d: %f",i,myAxis[i]);
            string infoString=myString;
            ofDrawBitmapString(infoString,200,200 + i*16);
        }
    }


}


void testApp::trigger(Actor* other){

    for (int i=0;i<(int)joyList.size();i++){
        joyList[i]->color=COLOR_WHITE;
        if (other==joyList[i]){
            joy=new jsJoystick(i);
            cout << "connected to:" << joy->getName() << endl;

            other->color=COLOR_RED;

            //OSC stuff
            osc_sender.setup(ipAddress,31840+channel);
            bSending=true;
        }
    }

    //update eventList in case we connect...

    for (int i=0;i<12;i++){
            if (other==eventList[i]){
                cout << "exchanging " << btnEventNames[i] << " with "<< eventList[i]->tooltip << endl;
                btnEventNames[i]=eventList[i]->tooltip;
            }
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

