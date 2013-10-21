#ifndef _TEST_APP
#define _TEST_APP




#include "ofMain.h"

#include "ofxKinect.h"
#include "ofxOsc.h"
#include "ofxOpenCV.h"
#include "msbOFCore.h"
#include "actor.h"

#define KINECTSIZE unsigned char
//#define KINECTSIZE float


struct actorID;
struct memberID;

class testApp : public ofBaseApp, public Actor{

	public:

        testApp();
        virtual ~testApp();

		void setup();

        void msbSetup();
		void interfaceSetup();
		void filemappingSetup();

		void update();

		void sendSkeleton(int i);

        Matrix4f makeMatrix4(XnSkeletonJointOrientation* joint);

        string makeMatrixMessage( ofxOscMessage* myMessage, Matrix4f myMat4 );

		void draw();
		void exit();

        int shareMemory();

		void keyPressed  (int key);
		void keyReleased  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

        //msbSpecific
        void registerProperties();
        void trigger(Actor* other);
        void loadSettings();

#ifdef TARGET_WIN32
        HANDLE hMapFile;
        LPCTSTR pBuf;
#else
		void *sharedMemFile;
		int fd,ret;
		void *sourcebuffer;

#endif
        ofxKinect           kinect;
        ofxOscSender        osc_sender;


        string              ipAddress;
        int                 channel;

        int                 skelNo;

		bool                bShareMemory;
        bool                bSetCutoffToZero;
        bool                bFullscreen;
        bool                bSendSkeleton;

        //msbTools specific

        KINECTSIZE *        myPic;

        bool                bHighZRes;

        Input*              input;
        Renderer*           renderer;
        Actor*              patchActor;

        float               cutOffDepth;

        float               thresh;

        int                 dilate;
        int                 erode;
        int                 blur;

        int                 dilateMask;
        int                 erodeMask;
        int                 blurMask;

        ofxCvGrayscaleImage          cvImage;
        ofxCvGrayscaleImage          cvFinal;
        ofxCvGrayscaleImage          cvMaskBase;
        ofxCvGrayscaleImage          cvMask;

        unsigned char*               pixelBufferOne;
        unsigned char*               pixelBufferTwo;
        unsigned char*               pixelBufferThree;

};

#endif
