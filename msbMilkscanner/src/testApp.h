#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include <sstream>
#include "msbOFCore.h"
#include "SliderButton.h"
#include "chromaKeyer.h"

class testApp : public ofSimpleApp, public Actor{

	public:

		void setup();
		void msbSetup();
		void registerProperties();
		void update();
		void draw();

        void trigger(Actor* other);

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
		unsigned char * 	videoInverted;

		ofTexture			videoTexture;

		ofTexture			inkScanTexture;
        unsigned char * 	depthMap;
        unsigned char * 	fullMap;
        unsigned char * 	colorMap;

        ofxCvColorImage*    keySource;

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
        bool                bLumaKey;

        string              filename;

		ofTrueTypeFont	    verdana;

		ofImage             endImage;

        ChromaKeyer*        chromaKey;

		//MSBStuff
        Input*              input;
        Renderer*           renderer;
        Actor*              patchActor;
        BasicButton*        startButton;
        SliderButton*       threshholdSlider;
        SliderButton*       hueThreshholdSlider;
        SliderButton*       satThreshholdSlider;
        SliderButton*       velThreshholdSlider;

        int                 mouseButton;
};

#endif
