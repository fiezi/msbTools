#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOsc.h"
#include "msbOFCore.h"

#ifdef TARGET_WIN32
#include "windows.h"
#include "sixense.h"
#include <sixense_math.hpp>
#include <sixense_utils/mouse_pointer.hpp>
#include <sixense_utils/derivatives.hpp>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/event_triggers.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>

#include <deque>

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
        void sendGamePad();
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
        ofThread thread;

        Input*      input;
        Renderer*   renderer;

        vector<BasicButton*>  joyList;
        jsJoystick*     joy;

        vector<BasicButton*>  eventList;
        vector<string> btnEventNames;

        string  ipAddress;
        int     channel;

        bool    bSending;

        bool    bAdditive;

        float   axisOne;
        float   axisTwo;

};

#endif
