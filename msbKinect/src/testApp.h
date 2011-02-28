#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"
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

		void drawPointCloud();

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

        HANDLE hMapFile;
        LPCTSTR pBuf;

        ofxKinect           kinect;

		ofxCvColorImage		colorImg;

		ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayThresh;
		ofxCvGrayscaleImage 	grayThreshFar;

		ofxCvContourFinder 	contourFinder;

		bool                bShareMemory;
		bool				bThreshWithOpenCV;

		int 				nearThreshold;
		int					farThreshold;

		int					angle;

        //msbTools specific

        float*          myPic;

        Input*          input;
        Renderer*       renderer;
        Actor*          patchActor;

        float           cutOffDepth;

};

#endif
