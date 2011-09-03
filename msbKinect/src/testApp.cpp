#include "testApp.h"

#ifdef TARGET_WIN32

	#include <windows.h>
	#include <stdio.h>
	#include <conio.h>
	#include <tchar.h>

	TCHAR szName[]=TEXT("Global\\MyFileMappingObject");
#else

	#include <sys/mman.h>

#endif

#include "actor.h"
#include "sliderButton.h"
#include "textInputButton.h"
#include "assignButton.h"
#include "msbLight.h"

#define BUF_SIZE 640*480*4*32


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

    tiBut= new TextInputButton;
    tiBut->location.x=10;
    tiBut->location.y=570;
    tiBut->scale.x=100;
    tiBut->scale.y=12;
    tiBut->color=Vector4f(0.5,0.5,0.5,1.0);
    tiBut->textureID="icon_flat";
    tiBut->name="previewRes";
    tiBut->bDrawName=true;
    tiBut->setLocation(tiBut->location);
    tiBut->parent=patchActor;
    tiBut->buttonProperty="PARTICLESCALE";
    renderer->buttonList.push_back(tiBut);
    tiBut->bPermanent=true;
    tiBut->setup();

    tiBut= new TextInputButton;
    tiBut->location.x=10;
    tiBut->location.y=600;
    tiBut->scale.x=100;
    tiBut->scale.y=12;
    tiBut->color=Vector4f(0.5,0.5,0.5,1.0);
    tiBut->textureID="icon_flat";
    tiBut->name="setCutoffZero";
    tiBut->bDrawName=true;
    tiBut->setLocation(tiBut->location);
    tiBut->parent=this;
    tiBut->buttonProperty="BSETCUTOFFTOZERO";
    renderer->buttonList.push_back(tiBut);
    tiBut->bPermanent=true;
    tiBut->setup();

    tiBut= new TextInputButton;
    tiBut->location.x=10;
    tiBut->location.y=620;
    tiBut->scale.x=100;
    tiBut->scale.y=12;
    tiBut->color=Vector4f(0.5,0.5,0.5,1.0);
    tiBut->textureID="icon_flat";
    tiBut->name="send to MSB";
    tiBut->bDrawName=true;
    tiBut->setLocation(tiBut->location);
    tiBut->parent=this;
    tiBut->buttonProperty="BSHAREMEMORY";
    renderer->buttonList.push_back(tiBut);
    tiBut->bPermanent=true;
    tiBut->setup();

    //OF_STUFF

	//kinect.init(true);  //shows infrared image
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();

#ifdef TARGET_WIN32

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

#else
	sourcebuffer=NULL;
	//taken from: http://mike.kronenberg.org/?p=40
	//all credit goes to Mike Kronenberg!

	// we need a dummy file, this gives us the possibility to
	// access the shared memory over the filesystem
	sharedMemFile = malloc(BUF_SIZE);

	// open trunkate/create read/write for everybody
	fd = open("/tmp/msbRamFile", O_CREAT|O_RDWR, 0666);
	if (fd == -1) {
		cout <<"Could not open '/tmp/msbRamFile'"<<endl;
	}
	ret = write(fd, sharedMemFile, BUF_SIZE); // create the file
	ret = close(fd);
	free(sharedMemFile);

	//attach shared memory file
	fd = open("/tmp/msbRamFile", O_RDWR);
	if(!fd)
		cout << "Could not open '/tmp/msbRamFile'"<<endl;

	// load the file into memory, shared, read & write access
	sourcebuffer = mmap( 0, BUF_SIZE, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0);
	if(!sourcebuffer)
		cout << "Could not mmap '%s.vga'"<< endl;

	// once the file is mapped, we can dispose of the filehandle
	close(fd);

#endif
    myPic= new float[640*480*4];

    bShareMemory=false;
    bSetCutoffToZero=false;
}


void testApp::registerProperties(){

   createMemberID("BSHAREMEMORY",&bShareMemory,this);
   createMemberID("CUTOFFDEPTH",&cutOffDepth,this);
   createMemberID("BSETCUTOFFTOZERO",&bSetCutoffToZero,this);
}

int testApp::shareMemory(){


    //store xyz values in rgb pixels, soon.
    if (kinect.getDepthPixels()){


        //construct full color image
        for (int i=0;i<640*480*4;i+=4){
            myPic[i]=(float)kinect.getPixels()[640*480 * 3 - (i/4) *3] * 1.0f/255.0f;
            myPic[i+1]=(float)kinect.getPixels()[640*480 * 3 - (i/4) *3 + 1] * 1.0f/255.0f;
            myPic[i+2]=(float)kinect.getPixels()[640*480 * 3 - (i/4) *3 + 2] * 1.0f/255.0f;
            //alpha from here
            myPic[i+3]=(float)kinect.getDistancePixels()[640*480 - i/4];
            if (myPic[i+3]>cutOffDepth){
                if (bSetCutoffToZero)
                    myPic[i+3]=0.0f;
                else
                    myPic[i+3]=1.0f;
            }else{
                myPic[i+3]=myPic[i+3]/cutOffDepth;
                if (myPic[i+3]<=0){
                    if (bSetCutoffToZero)
                        myPic[i+3]=0.0f;
                    else
                        myPic[i+3]=1.0f;
                }

            }

        }
#ifdef TARGET_WIN32
       CopyMemory((PVOID)pBuf, myPic, (640*480 * 4* sizeof(float)));
#else
		memcpy(sourcebuffer, myPic, (640*480 * 4* sizeof(float)));
#endif
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

//--------------------------------------------------------------
void testApp::exit(){

#ifdef TARGET_WIN32
    UnmapViewOfFile((void*)pBuf);
    CloseHandle(hMapFile);
#else
	//... do something intresting with the shared memory, then unmap
	if (sourcebuffer)
		munmap(sourcebuffer, BUF_SIZE);
#endif


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
