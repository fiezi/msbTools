#ifndef _TEST_APP
#define _TEST_APP




#include "ofMain.h"

#include "ofxKinect.h"
#include "msbOFCore.h"
#include "actor.h"

struct actorID;
struct memberID;

class testApp : public ofBaseApp, public Actor
{

	public:

		void setup();
		void update();
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
#ifdef TARGET_WIN32
        HANDLE hMapFile;
        LPCTSTR pBuf;
#else
		void *sharedMemFile;
		int fd,
			ret;
		void *sourcebuffer;

#endif
        ofxKinect           kinect;

		bool                bShareMemory;
        bool                bSetCutoffToZero;

        //msbTools specific

        float*          myPic;

        Input*          input;
        Renderer*       renderer;
        Actor*          patchActor;

        float           cutOffDepth;

};

#endif
