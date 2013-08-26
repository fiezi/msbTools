#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "msbOFCore.h"


class testApp : public ofBaseApp{

	public:

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

		// we don't actually use these
        // just checking to see if they
        // all work in the same place :)

        ofxCvGrayscaleImage cvGray;
        ofxOscSender osc_sender;

		ofVideoGrabber 		vidGrabber;

        Input*      input;
        Renderer*   renderer;

};

#endif
