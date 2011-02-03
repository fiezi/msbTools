#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxDirList.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "recorderManager.h"
#include "loaderManager.h"

class testApp : public ofBaseApp{

	public:

		testApp();
		~testApp();
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);

        void loadSettings(string filename);     // loads general app settings
        void sendMessages();                    // osc messages sent from here

        //----- video input
		ofVideoGrabber *		videoSourceL;   // left camera
		ofVideoGrabber *		videoSourceR;   // right camera

		bool                    bInitCameras[2];   // are cams initialized
		int                     vidW,vidH;         // w and h of video capture
		int                     camId[2];          // ids of camera: list is output to console on startup
        ofPoint                 vidDrawScale[2];   // resizing camera image
        ofPoint                 vidDrawPos[2];     // moving pos of camera image
        int                     lastX,lastY;       // previous mouse x and y

		//----- character recorder
        recorderManager         rManager;       // main manager of recording and keyframing + gui


        //------ session loader
        loaderManager           sessionLoader;  // handles loading of a sequence

        //----- osc output
        ofxOscSender        sender;             // sends the osc messages
        string              host;
        int                 port;
        bool                bSending;           // for toggling osc on and off
        bool                bSwapCamera;        // for toggling which cam is left and right



};

#endif

/*
- video input
- chroma keyer
- recorder
- player
- editor

*/
