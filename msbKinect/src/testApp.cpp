#include "testApp.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include "actor.h"
#include "sliderButton.h"
#include "textInputButton.h"
#include "assignButton.h"
#include "msbLight.h"

#define BUF_SIZE 640*480*32
TCHAR szName[]=TEXT("Global\\MyFileMappingObject");


//--------------------------------------------------------------
void testApp::setup()
{
    //MSB setup
    renderer=Renderer::getInstance();
    input=Input::getInstance();

    renderer->loadPreferences();

    renderer->setup();

    renderer->camActor=new Actor;
    renderer->camActor->setLocation(Vector3f(0,0,-5));
    renderer->camActor->postLoad();

    cutOffDepth=4096;
    registerProperties();


    //Adding MSB content
    //heightfield based on videoTexture from OF
    Actor* myActor = new Actor;
    myActor->setLocation(Vector3f(-0.5,-1.0,-5));
    myActor->setRotation(Vector3f(0,180,0));
    myActor->color=Vector4f(1,1,1,1);
    myActor->drawType=DRAW_POINTPATCH;
    myActor->particleScale=250;
    myActor->scale=Vector3f(1,-1,1);
    myActor->bTextured=true;
    myActor->textureID="NULL";
    myActor->sceneShaderID="heightfield";
    myActor->setup();

    renderer->actorList.push_back(myActor);
    renderer->layerList[0]->actorList.push_back(myActor);
    patchActor=myActor;

    SliderButton *but;

    but= new SliderButton;
    but->location.x=10;
    but->location.y=740;
    but->scale.x=1000;
    but->color=Vector4f(0.3,0.3,0.3,1.0);
    but->scale.y=10;
    but->name="RotateViewX";
    but->tooltip="drag to Change";
    but->setLocation(but->location);
    but->textureID="icon_flat";
    but->setup();
    but->bDrawName=true;
    but->parent=this;
    but->bVertical=false;
    but->sliderValue=0.5;
    renderer->buttonList.push_back(but);

    TextInputButton *tiBut;

    tiBut= new TextInputButton;
    tiBut->location.x=10;
    tiBut->location.y=550;
    tiBut->scale.x=100;
    tiBut->scale.y=12;
    tiBut->color=Vector4f(0.5,0.5,0.5,1.0);
    tiBut->textureID="icon_flat";
    tiBut->name="cutOffDepth";
    tiBut->bDrawName=true;
    tiBut->setLocation(tiBut->location);
    tiBut->parent=this;
    tiBut->buttonProperty="CUTOFFDEPTH";
    renderer->buttonList.push_back(tiBut);
    tiBut->bPermanent=true;
    tiBut->setup();

    //OF_STUFF

	//kinect.init(true);  //shows infrared image
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThresh.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);

	nearThreshold = 50;
	farThreshold  = 180;
	bThreshWithOpenCV = true;

	ofSetFrameRate(60);

	angle = 0;

   hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 0,                       // maximum object size (high-order DWORD)
                 BUF_SIZE,                // maximum object size (low-order DWORD)
                 szName);                 // name of mapping object

    if (hMapFile == NULL)
    {
      _tprintf(TEXT("Could not create file mapping object (%d).\n"),
             GetLastError());
      //OF_EXIT_APP(1);
    }
    pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        BUF_SIZE);


   if (pBuf == NULL)
   {
      _tprintf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());

       CloseHandle(hMapFile);

      //OF_EXIT_APP(0);
   }

    myPic= new float[640*480];

    bShareMemory=true;
}


void testApp::registerProperties(){

   createMemberID("CUTOFFDEPTH",&cutOffDepth,this);
}

int testApp::shareMemory(){


    //store xyz values in rgb pixels, soon.
    if (kinect.getDepthPixels()){

    /*
        for (int col=0;col<240;col++){
            for (int row=0;row<320;row++){
                myPic[320*col+row]=(kinect.getDepthPixels()[640*col*2 + 2*row]);
            }
        }

    */

        //reverse image
        for (int i=0;i<640*480;i++){
            myPic[i]=(float)kinect.getDistancePixels()[640*480-i];
            if (myPic[i]>4192.0f)
                myPic[i]=0.0f;
            else
                myPic[i]=myPic[i]/cutOffDepth;

        }
       //CopyMemory((PVOID)pBuf, myPic, (320*240 * sizeof(float)));
       CopyMemory((PVOID)pBuf, myPic, (640*480 * sizeof(float)));
    }
   // _getch();



}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(100, 100, 100);
	kinect.update();

    if (bShareMemory)
        shareMemory();

    renderer->update();

    patchActor->ofTexturePtr=&kinect.getDepthTextureReference();
}

//--------------------------------------------------------------
void testApp::draw()
{
    glEnable(GL_LIGHTING);
    renderer->draw();
    glDisable(GL_LIGHTING);

	ofSetColor(255, 255, 255);

	kinect.drawDepth(10, 50, 400, 300);
	kinect.draw(420, 50, 400, 300);


}

void testApp::drawPointCloud() {

	ofScale(400, 400, 400);
	int w = 640;
	int h = 480;
	ofRotateY(mouseX);
	float* distancePixels = kinect.getDistancePixels();
	glBegin(GL_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint cur = kinect.getWorldCoordinateFor(x, y);
			glVertex3f(cur.x, cur.y, cur.z);
		}
	}
	glEnd();
}

//--------------------------------------------------------------
void testApp::exit(){

    UnmapViewOfFile((void*)pBuf);
    CloseHandle(hMapFile);

    cout << "woohoo!" << endl;


}

//--------------------------------------------------------------
void testApp::keyPressed (int key){

    input->normalKeyDown(key,mouseX,mouseY);
    input->specialKeyDown(key,mouseX,mouseY);

}

void testApp::keyReleased(int key){

    input->keyUp(key,mouseX,mouseY);
    input->specialKeyUp(key,mouseX,mouseY);

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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
void testApp::windowResized(int w, int h)
{}

void testApp::trigger(Actor* other){

    if (other->name=="RotateViewX"){
        float slValue= 0.0f;
        slValue= ((SliderButton*)other)->sliderValue;
        patchActor->setRotation(Vector3f(0,slValue * 360.0,0));
    }
    if (other->name=="cutOffDepth")
        kinect.cutOffFar=cutOffDepth;
}
