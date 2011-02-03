#include "testApp.h"
#include "actor.h"

//--------------------------------------------------------------
void testApp::setup(){

renderer=Renderer::getInstance();
input=Input::getInstance();

renderer->loadPreferences();

renderer->setup();

renderer->camActor=new Actor;
renderer->camActor->setLocation(Vector3f(0,0,-5));
renderer->camActor->postLoad();
renderer->camActor->drawType=DRAW_TEA;
renderer->actorList.push_back(renderer->camActor);
}

//--------------------------------------------------------------
void testApp::update(){

    renderer->update();
    renderer->draw();

}


//--------------------------------------------------------------
void testApp::draw(){


    renderer->draw();

    ofCircle(300,400,100);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    input->normalKeyDown(key,mouseX,mouseY);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

    input->keyUp(key,mouseX,mouseY);
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

