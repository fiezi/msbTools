#ifndef _TEST_APP
#define _TEST_APP


#include "tinyxml.h"
#include "ofMain.h"
#include <sstream>


class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();

        void doStep();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

		void generateAlphaImage();

		void createXML();
		void createCutouts();

		ofVideoGrabber 		vidGrabber;
		ofVideoGrabber 		vidGrabber2;
		unsigned char * 	videoInverted;

		ofTexture			videoTexture;

		ofTexture			inkScanTexture;
        unsigned char * 	depthMap;
        unsigned char * 	fullMap;
        unsigned char * 	colorMap;

        vector<ofImage*>     cutouts;

        string              fluid; //ink or milk

		int 				camWidth;
		int 				camHeight;

		int                 currentStep;
		int                 threshhold;
		int                 fileRevision;

		int                 cutOutScale;

		float               currentTime;
		float               deltaTime;
		float               intervalTime;

        bool                bIsRecording;
        bool                bIsInterval;
        bool                bStepping;
        bool                bJustStepped;
        bool                bFrontToBack;

		ofTrueTypeFont	    verdana;

		ofImage             endImage;
};

#endif
