#include "testApp.h"
#include "actor.h"
#include "assignButton.h"
#include "msbLight.h"

//--------------------------------------------------------------
void testApp::setup(){

    //MSB setup
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
    but->location.x=100;
    but->location.y=100;
    but->name="hello_world";
    but->bDrawName=true;
    but->tooltip="hello_world";
    but->setLocation(but->location);
    but->textureID="icon_base";
    but->color=COLOR_RED;
    but->setup();
    but->parent=NULL;
    renderer->buttonList.push_back(but);


    //heightfield based on videoTexture from OF
    Actor* myActor = new Actor;
    myActor->setLocation(Vector3f(-1,-0.5,-5));
    myActor->postLoad();
    myActor->color=Vector4f(1,1,1,1);
    myActor->drawType=DRAW_POINTPATCH;
    myActor->particleScale=250;
    myActor->scale=Vector3f(2,-1.5,1);
    myActor->bTextured=true;
    myActor->textureID="NULL";
    myActor->sceneShaderID="heightfield";

    renderer->actorList.push_back(myActor);
    renderer->layerList[0]->actorList.push_back(myActor);

    //teapot with MSB texture
    myActor = new Actor;
    myActor->setLocation(Vector3f(1,-0.5,-4));
    myActor->postLoad();
    myActor->color=Vector4f(1,1,1,1);
    myActor->drawType=DRAW_TEA;
    myActor->particleScale=1;
    myActor->bTextured=true;
    myActor->textureID="grid_solid";
    myActor->sceneShaderID="texture";

    renderer->actorList.push_back(myActor);
    renderer->layerList[0]->actorList.push_back(myActor);

    //MSB light
    myActor = new MsbLight;
    myActor->setLocation(Vector3f(1,-0.5,-4));
    myActor->postLoad();
    myActor->color=Vector4f(1,1,0,1);
    myActor->particleScale=1;
    renderer->actorList.push_back(myActor);
    renderer->lightList.push_back((MsbLight*)myActor);
    renderer->layerList[0]->actorList.push_back(myActor);

    //OF stuff

    vidGrabber.setVerbose(true);
	vidGrabber.setUseTexture(true);
	vidGrabber.initGrabber(640,480);

    vidGrabber.listDevices();


    ofBackground(128, 128, 164);


}

//--------------------------------------------------------------
void testApp::update(){


    //rotate the actor
    Actor* patchActor= renderer->actorList[0];
    patchActor->addRotation(0.5,Vector3f(0.3,1.0,0.0));

    Actor* teaActor= renderer->actorList[1];
    teaActor->addRotation(2.7,Vector3f(0.3,0.5,0.0));

    Actor* lightActor= renderer->actorList[2];
    lightActor->setLocation(Vector3f(sin(ofGetElapsedTimef()) * 1.0f, 0.0f, -7.0f + cos(ofGetElapsedTimef()) * 4.0f));


    renderer->update();
    vidGrabber.grabFrame();

    patchActor->ofTexturePtr=&vidGrabber.getTextureReference();


    return;
}


//--------------------------------------------------------------
void testApp::draw(){

    renderer->draw();

    glPushMatrix();
    glScalef(0.5,0.5,1.0);
    glDisable(GL_LIGHTING);
	vidGrabber.draw(400,100);
    glEnable(GL_LIGHTING);

	glPopMatrix();


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

