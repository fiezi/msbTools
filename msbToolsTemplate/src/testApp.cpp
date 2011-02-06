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


	vidGrabber.setVerbose(true);
	vidGrabber.setUseTexture(true);
	vidGrabber.initGrabber(640,480);

    vidGrabber.listDevices();


    ofBackground(128, 128, 164);

    cout << "allocating texture... " << endl;
    renderer->actorList[0]->myTexture.allocate(640,480,GL_RGB,true);
    renderer->checkOpenGLError();

}

//--------------------------------------------------------------
void testApp::update(){


    Actor* myActor= renderer->actorList[0];
    //myActor->addRotation(0.5,Vector3f(0.3,1.0,0.0));

    renderer->update();

    myActor->myTexture.loadData(vidGrabber.getPixels(),640,480,GL_RGB);

    vidGrabber.grabFrame();

    return;

    GLuint texture;

    //allocate texture List
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0 ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0 ? GL_REPEAT : GL_CLAMP );

    // build our texture and mipmaps
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_BGR, GL_UNSIGNED_BYTE, vidGrabber.getPixels() );

    //renderer->textureList[renderer->actorList[0]->textureID]->texture=texture;
    glBindTexture( GL_TEXTURE_2D, 0);


    //glBindTexture( GL_TEXTURE_2D,  vidGrabber.getTextureReference().texData.textureID);


}


//--------------------------------------------------------------
void testApp::draw(){

    renderer->draw();


    glScalef(0.5,0.5,1.0);
	renderer->actorList[0]->myTexture.draw(100,100);
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

