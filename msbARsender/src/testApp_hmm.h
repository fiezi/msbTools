#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxThread.h"
#include "ofxXmlSettings.h"

#include "ar.h"
#include "arTKPMultiTracker.h"

class testApp : public ofBaseApp{

	public:

		testApp();
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

        void loadConfig(string filename);
        void sendMessages();

        // need video input
        ofBaseVideo *		videoSource;
		bool                bUseVideoFiles;
		string              videoFile;
		int                 cameraId;
		int                 camW,camH;
		ofxCvColorImage     colorPixels;
		ofxCvGrayscaleImage grayPixels;


        // ar toolkit stuff
        //unsigned char *cameraBuffer;
		arTKPMultiTracker arTracker;
		bool useBCH;


        // osc output
        ofxOscSender        sender;
        string              host;
        int                 port;
        bool                bSending;



};

#endif
