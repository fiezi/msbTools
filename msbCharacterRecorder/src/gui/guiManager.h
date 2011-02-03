#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include "guiButton.h"
#include "guiSlider.h"

class guiManager
{
    public:
        guiManager();
        virtual ~guiManager();

        void draw( ofTrueTypeFont & font );
        void drawRulers();

        // adds a button key frame to the timeline
        void addFrame();

        // clears timeline buttons
        void resetFrames();

        // adjusts the current timeline buttons being shown based on current frame selected
        void adjustTimeLinePosition(int frameOn);
        // sets the timeline back to the first frame that is turned on
        void rewindTimeLine();

        int hitTest(int x, int y, int button);

        // turns a timeline button on or off
        void toggleTimeLineButton( int id, bool bOn );

        // controls
        guiButton       recordButton;           // button for controlling recording
        guiButton       playButton;             // button for controlling playback
        guiButton       clearButton;            // button to clear all recorded frames and thumbnails
        guiButton       addBoneButton;          // for adding bone key frames to timeline
        guiButton       removeBoneButton;       // for removing bone key frames
        guiButton       folderNameButton;       // text box area for entering folder same where files will be stored
        guiButton       saveButton;             // button for saving frames and session data
        guiButton       loadButton;             // button to load frames from entered directory
        guiButton       chromaOnButton;         // button to toggle keying on and off
        guiButton       saveSessionButton;         // button to save session only (no images)

        // keying controls
        guiSlider       rSlider;                // slider for keyer red channel
        guiSlider       gSlider;                // slider for keyer green channel
        guiSlider       bSlider;                // slider for keyer blue channel
        guiSlider       htSlider;               // slider for hue threshold
        guiSlider       stSlider;               // slider for saturation threshold


        // scrubber
        guiSlider        timeSlider;            // slider for moving through timeline

        // fps slider
        guiSlider       fpsSlider;              // slider for adjusting the playback framerate

		// timeline
		vector<guiButton> timeLine;             // buttons for each frame in timeline that cna be turned on and off

		int timeLineW, timeLineH;               // size of frames in timeline
		int timeLineX, timeLineY;               // position on screen of timeline start
		int firstOn, lastOn, totalOn;           // first and last active frames, total frames active

        // video cam draw
        void alterVideo(int x, int y, int lastX, int lastY, int button);
        bool                    bAlterLVideo, bAlterRVideo;
        ofPoint                 vidDrawScale[2];   // resizing camera image
        ofPoint                 vidDrawPos[2];     // moving pos of camera image

        // rulers
        vector<int>             hRulers;
        vector<int>             vRulers;
        bool                    bHRuler, bVRuler;
        int                     selectedRuler;

        void newRuler( int x, int y, bool bHoriz );
        void updateRuler(int x, int y, bool bHoriz);
        void releaseRulers();
        void clearAllRulers(){ hRulers.clear(); vRulers.clear(); }
        void selectRulers( int x, int y );


    protected:

        void updateTimeLineData();

    private:
};

#endif // GUIMANAGER_H
