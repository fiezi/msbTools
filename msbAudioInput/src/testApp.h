#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxThread.h"
#include "ofx3DModelLoader.h"
#include "msbOFCore.h"
#include "actor.h"

#define HOST "localhost"
#define PORT 31840

class testApp : public ofBaseApp, public Actor{

	public:

		void setup();
		void update();
		void draw();

        void audioReceived 	(float * input, int bufferSize, int nChannels);

        void registerProperties();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);

		void trigger(Actor* other);

		bool    bSending;
		float * left;
		float * right;
		int 	bufferCounter;
		int 	drawCounter;
		int     channel;

		string  oscPath;

        ofxOscSender* sender;

        Input*      input;
        Renderer*   renderer;

};

#endif
