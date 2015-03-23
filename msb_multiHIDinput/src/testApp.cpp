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

    axisOne=0.0;
    axisTwo=0.0;

    bAdditive=true;

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
        string jName=joy->getName();

        //figure out if gametrak...
        string str="Game-Trak";
        if (jName.find(str)!=string::npos){
            sendGameTrak();
        str="Razer Hydra";
        }else if (jName.find(str)!=string::npos){
            sendHydra();
        }else
            sendGamePad();


    }
}


//--------------------------------------------------------------
void testApp::draw(){

    renderer->draw();


    if (joy){
        int myInt;
        float myAxis[10];
        joy->read(&myInt, &myAxis[0]);

        char myString[512];

        for (int i=0;i<joy->getNumAxes();i++){
            sprintf(myString,"Axis %d: %f",i,myAxis[i]);
            string infoString=myString;
            ofDrawBitmapString(infoString,200,300 + i*16);
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

                //sixense stuff
                #ifdef TARGET_WIN32
                    sixenseInit();
                    sixenseSetActiveBase(0);
                    sixenseAutoEnableHemisphereTracking(0);

                #endif



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

void testApp::sendGamePad(){

        int myInt;
        float myAxis[10];
        joy->read(&myInt, &myAxis[0]);
        float xAxis=myAxis[0];
        float yAxis=myAxis[1];
        float zAxis=myAxis[2];

        if (bAdditive){
            axisOne+=xAxis * -0.02;
            axisTwo+=yAxis * 0.02;

        }

        ofxOscMessage myMessage;
        myMessage.addFloatArg(axisOne);
        myMessage.addFloatArg(zAxis * 0.0);
        myMessage.addFloatArg(axisTwo);

        myMessage.setAddress("/pilot/vector3f");
        osc_sender.sendMessage(myMessage);

        cout << "sending... gp" << endl;
}

void testApp::sendHydra(){

#ifdef TARGET_WIN32

    sixenseSetActiveBase(0);
    sixenseAllControllerData acd;
    sixenseGetAllNewestData( &acd );
    //sixenseUtils::getTheControllerManager()->update( &acd );
    Vector3f myPos;
    myPos.x=acd.controllers[0].pos[0];
    myPos.y=acd.controllers[0].pos[1];
    myPos.z=acd.controllers[0].pos[2];

    Quatf quatern= Quatf(acd.controllers[0].rot_quat[0],acd.controllers[0].rot_quat[1],acd.controllers[0].rot_quat[2],acd.controllers[0].rot_quat[3]);

    Matrix3f myRot= quatern.rotMatrix();

    myRot.transpose();


        ofxOscMessage myMessage;
        myMessage.addFloatArg(myPos.x/10.0);
        myMessage.addFloatArg(myPos.y/10.0);
        myMessage.addFloatArg(myPos.z/10.0);
        myMessage.addFloatArg(1.0);

        myMessage.setAddress("/brush/vector3f/float");
/*
        Matrix4f myMatrix;

        myMatrix.setRotation(myRot);
        myMatrix.setTranslation(myPos/100.0);
        for (int i=0;i<16;i++)
        myMessage.addFloatArg( myMatrix.data[i]);
*/
        osc_sender.sendMessage(myMessage);

        ofxOscMessage myTrigger;
        myTrigger.setAddress("/brush/draw");

        ofxOscMessage myButtons;
        myButtons.setAddress("/brush/new");



        if (acd.controllers[0].buttons ==128){
            osc_sender.sendMessage(myTrigger);
            cout << "drawing" << endl;
        }


        if (acd.controllers[1].buttons == 128){
            osc_sender.sendMessage(myButtons);
            cout << "new drawing ***************" << endl;
        }



        cout << "sending... hydra" << endl;
#endif

}

void testApp::sendGameTrak(){

        int myInt;
        float myAxis[10];
        joy->read(&myInt, &myAxis[0]);

        float maxDist=100.0;
        float maxAngle=30.0;

        //distance should be from 0 to 1
        float dist=(1.0 - (myAxis[2] + 1.0)/2.0) * maxDist;

        //angles can be from -30 to 30
        float angleX=-myAxis[0]* maxAngle;
        float angleZ=myAxis[1]* maxAngle;

        float xPos = sin(angleX * M_PI_2/180.0) * dist;
        float zPos = sin(angleZ * M_PI_2/180.0) * dist;
        //float yPos = (1.0 - sin(angleX * M_PI_2) - sin(angleZ * M_PI_2)) * dist;
        float yPos = min(cos(angleX * M_PI_2/180.0),cos(angleZ * M_PI_2/180.0)) * dist;


        ofxOscMessage myMessage;
        myMessage.addFloatArg(xPos);
        myMessage.addFloatArg(yPos);
        myMessage.addFloatArg(zPos);

        myMessage.setAddress("/pilot/vector3f");
        osc_sender.sendMessage(myMessage);

        cout << "sending... gt" << endl;
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

