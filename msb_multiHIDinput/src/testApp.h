#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxDirList.h"
#include "ofxOsc.h"
#include "ofxThread.h"
#include "ofx3DModelLoader.h"
#include "msbOFCore.h"

#ifdef TARGET_WIN32
#include "windows.h"
#include "sixense.h"
#endif

#include "ul.h"
#include "js.h"



class testApp : public ofBaseApp, public Actor{

	public:

		void setup();
		void setupInterface();

		void registerProperties();

		void update();
		void draw();

        void trigger(Actor* other);

        void sendGameTrak();
        void sendHydra();

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
        ofxOscSender osc_sender;
        ofxThread thread;

        Input*      input;
        Renderer*   renderer;

        vector<BasicButton*>  joyList;
        jsJoystick*     joy;

        vector<BasicButton*>  eventList;
        vector<string> btnEventNames;

        string  ipAddress;
        int     channel;

        bool    bSending;

};

#endif
