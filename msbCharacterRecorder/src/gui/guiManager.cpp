#include "guiManager.h"

guiManager::guiManager()
{
    timeLineW = 40;
    timeLineH = 60;
    timeLineX = 30;
    timeLineY = 460;

    firstOn = 0;
    lastOn  = 0;
    totalOn = 0;

    int topPanelY = 410;

    timeSlider.setup("timeline", 0, 0, 1, (ofGetWidth()-30-384), topPanelY, 384, 15 );

    fpsSlider.setup("fps", 12/30.f, 0, 1, 250, topPanelY, 60, 18 );

    recordButton.setup(30,topPanelY,60,18,false);
    playButton.setup(100,topPanelY,60,18,false);
    clearButton.setup(170,topPanelY,60,18,false);

    rSlider.setup("r", 0, 0, 255, 420, 590, 60, 10 );
    gSlider.setup("g", 255, 0, 255, 420, 605, 60, 10 );
    bSlider.setup("b", 0, 0, 255, 420, 620, 60, 10 );

    htSlider.setup("ht", 10, 0, 255, 420, 660, 60, 10 );
    stSlider.setup("st", 10, 0, 255, 420, 675, 60, 10 );

    addBoneButton.setup(380,topPanelY,18,18,false);
    removeBoneButton.setup(405,topPanelY,18,18,false);

    folderNameButton.setup(110,590,220,15,true);
    folderNameButton.bOn = false;

    saveButton.setup( 330-90, 615, 40, 15, false );
    loadButton.setup( 330-40, 615, 40, 15, false );
    saveSessionButton.setup(330-90-100, 615, 90, 15, false );

    chromaOnButton.setup( 500,570,15,15,true);
    chromaOnButton.bOn = false;

    vidDrawPos[0].set(30,30);
	vidDrawPos[1].set(34+480,30);
	vidDrawScale[0].set(480,360);
	vidDrawScale[1].set(480,360);
	bAlterRVideo = false;
	bAlterLVideo = false;

	bHRuler = false;
	bVRuler = false;
	selectedRuler=-1;
}

guiManager::~guiManager()
{
    //dtor
}

//--------------------------------------------------------------
void guiManager::addFrame()
{
    // creates a new button and add it to the timeline

    // position after last frame added or at start if there are no frames
    int lastX = (timeLine.size() == 0) ? timeLineX: (int)timeLine[timeLine.size()-1].getX()+timeLineW;

    guiButton button;
    button.setup( lastX, timeLineY, timeLineW,timeLineH);
    button.setId( timeLine.size() );
    button.bOn = true;

    timeLine.push_back(button);
    totalOn++;
}
//--------------------------------------------------------------
void guiManager::resetFrames()
{
    timeLine.clear();
    firstOn = 0;
    lastOn  = 0;
    totalOn = 0;
}
//--------------------------------------------------------------
void guiManager::draw( ofTrueTypeFont & font )
{


    ofNoFill();
    if(folderNameButton.bOn)    ofSetColor(255,0,0);
    else                        ofSetColor(255,255,255);

    font.drawString("Directory: ", 30,folderNameButton.getY()+12 );
    folderNameButton.draw();

    ofSetColor(255,255,255);
    saveButton.draw();
    font.drawString("Save", saveButton.getX()+7, saveButton.getY()+12);

    loadButton.draw();
    font.drawString("Load", loadButton.getX()+7, loadButton.getY()+12);

    saveSessionButton.draw();
    font.drawString("Save Session", saveSessionButton.getX()+7, saveSessionButton.getY()+12);

    if(chromaOnButton.bOn) ofFill();
    else ofNoFill();
    chromaOnButton.draw();
    font.drawString("Toggle Keying", chromaOnButton.getX()+19, chromaOnButton.getY()+12);


    ofSetColor(255,255,255);
    timeSlider.drawAsMarker();

    fpsSlider.draw();
    font.drawString(ofToString(fpsSlider.getPctValue()*30,0)+" fps", fpsSlider.xpos+70, fpsSlider.getY()+10);

    if(recordButton.bOn) ofFill();
    else ofNoFill();
    recordButton.draw();

    if(playButton.bOn) ofFill();
    else ofNoFill();
    playButton.draw();

    ofNoFill();
    clearButton.draw();



    if(recordButton.bOn) ofSetColor(0,0,0);
    else ofSetColor(255,255,255);

    font.drawString("Record", recordButton.getX()+8, recordButton.getY()+13);

    if(playButton.bOn) ofSetColor(0,0,0);
    else ofSetColor(255,255,255);
    font.drawString("Play", playButton.getX()+17, playButton.getY()+13);

    if(clearButton.bOn) ofSetColor(0,0,0);
    else ofSetColor(255,255,255);
    font.drawString("Clear", clearButton.getX()+13, clearButton.getY()+13 );

    addBoneButton.draw();
    removeBoneButton.draw();
    font.drawString("+", addBoneButton.getX()+5, addBoneButton.getY()+13);
    font.drawString("-", removeBoneButton.getX()+5, removeBoneButton.getY()+13);
    font.drawString("Skeleton Key", removeBoneButton.getX()+22, removeBoneButton.getY()+14);

    rSlider.draw();
    gSlider.draw();
    bSlider.draw();
    htSlider.draw();
    stSlider.draw();

    font.drawString("R", rSlider.xpos-12, rSlider.getY()+10);
    font.drawString("G", gSlider.xpos-12, gSlider.getY()+10);
    font.drawString("B", bSlider.xpos-12, bSlider.getY()+10);

    font.drawString("Threshold Range", htSlider.xpos-25, htSlider.getY()-8);
    font.drawString("Hue", htSlider.xpos-25, htSlider.getY()+10);
    font.drawString("Sat", stSlider.xpos-25, stSlider.getY()+10);


    drawRulers();

}

//--------------------------------------------------------------
void guiManager::drawRulers()
{
    int w = ofGetWidth();
    int h = 390;//ofGetHeight();

    ofFill();
    ofSetColor(60,60,60);
    ofRect(0,0,20,h);
    ofRect(20,0,w,20);

    ofSetColor(0,0,0);
    for( int i = 10; i < w; i+=10){ int len = (i%50 == 0) ? 15 : 10; ofLine(i,0,i,len); }
    for( int i = 10; i < h; i+=10){ int len = (i%50 == 0) ? 15 : 10; ofLine(0,i,len,i); }

    ofSetColor(255,255,255);

    // draw rulers
    for( int i = 0; i < hRulers.size(); i++)
    {
        ofLine(0,hRulers[i],w,hRulers[i]);
    }

    for( int i = 0; i < vRulers.size(); i++)
    {
        ofLine(vRulers[i],0,vRulers[i],h);
    }

}

//--------------------------------------------------------------
int guiManager::hitTest(int x, int y, int button){

    // tests if x and y is inside any timeline button
    // returns -1 if no hit on the timeline or the id of the button if hit

    for( int i = 0; i < timeLine.size(); i++)
    {
        bool bOnOff = (button != 0) ? true: false;
        if( timeLine[i].update(x,y,bOnOff) )
        {
            updateTimeLineData();
            cout << "timeline id " << timeLine[i].id << endl;

            return timeLine[i].id;
        }

    }

    return -1;
}

void guiManager::toggleTimeLineButton( int id, bool bOn )
{
    if(timeLine.size() > id) timeLine[id].bOn = bOn;
}

void guiManager::updateTimeLineData()
{
    // goes through all buttons and finds the first active, last active, and total active

    totalOn = 0;
    for( int i = 0; i < timeLine.size(); i++)
    {
        if( timeLine[i].bOn && (i < firstOn || !timeLine[firstOn].bOn) ) firstOn = i;
        if( timeLine[i].bOn && i > lastOn )  lastOn = i;
        if( timeLine[i].bOn ) totalOn++;
    }

}

void  guiManager::adjustTimeLinePosition(int frameOn )
{
    // keeps all frames on screen as it is playing or being scrolled

    // find the max x position on screen
    int maxX = timeLineX+timeLineW*floor( (ofGetWidth()-30) / (float)timeLineW );

    // chck if current frame is offf screen and adjust position of all frames if it is

    if( timeLine[frameOn].hitBox.x >= maxX )
    {
        int offset = timeLineW+timeLine[frameOn].hitBox.x-maxX;

        for( int i = 0; i < timeLine.size(); i++)
            timeLine[i].hitBox.x -= offset;

    }else if( timeLine[frameOn].hitBox.x < timeLineW )
    {
        for( int i = 0; i < timeLine.size(); i++)
            timeLine[i].hitBox.x += timeLineW;
    }

    // if we reach end, reset positions (loop)
     if( frameOn == firstOn ) rewindTimeLine();
}

void  guiManager::rewindTimeLine()
{
   // cout << "rewind " << firstOn << endl;
    for( int i = 0; i < timeLine.size(); i++)
        timeLine[i].hitBox.x = timeLineX - (firstOn*timeLineW) + (i*timeLineW);
}

void guiManager::newRuler( int x, int y, bool bHoriz )
{
        if( bHoriz && !bVRuler){
             hRulers.push_back(y);
             bHRuler = true;
             selectedRuler = hRulers.size()-1;
        }
        else if(!bHRuler) {
             vRulers.push_back(x);
             bVRuler = true;
             selectedRuler = vRulers.size()-1;
        }
}

void guiManager::updateRuler(int x, int y, bool bHoriz)
{
    if( bHRuler && hRulers.size() > 0 ) hRulers[selectedRuler] = y;
    else if( bVRuler && vRulers.size() > 0) vRulers[selectedRuler] = x;
}

void guiManager::releaseRulers()
{
    for( int i = 0; i < hRulers.size(); i++)
    {
        if( hRulers[i] < 20 ) hRulers.erase( hRulers.begin()+i);
    }

    for( int i = 0; i < vRulers.size(); i++)
    {
        if( vRulers[i] < 20 ) vRulers.erase( vRulers.begin()+i);
    }

    bHRuler = false;
    bVRuler = false;
}

void guiManager::selectRulers( int x, int y )
{
    if( x < 20 ){
        bVRuler = true;
        newRuler(  x,  y, false );
        selectedRuler=vRulers.size()-1;
    }
    else if( y < 20 ){
        bHRuler = true;
        newRuler(  x,  y, true );
        selectedRuler=hRulers.size()-1;
    }
    else{
        for( int i = 0; i < hRulers.size(); i++)
        {
            if( abs(y - hRulers[i]) < 3 ){ bHRuler = true; selectedRuler=i; return; }
        }

        for( int i = 0; i < vRulers.size(); i++)
        {
            if( abs(x - vRulers[i]) < 3 ){ bVRuler = true; selectedRuler=i; return; }
        }
    }

}

void  guiManager::alterVideo(int x, int y, int lastX, int lastY, int button)
{
    if( bAlterLVideo )
    {
        if( button == 0 )
        {   vidDrawScale[0].y = y-vidDrawPos[0].y;
            vidDrawScale[0].y = MAX( MIN(360,vidDrawScale[0].y),100);
            vidDrawScale[0].x = 1.33*vidDrawScale[0].y;
        }else{
            vidDrawPos[0].x += x-lastX;
            vidDrawPos[0].y += y-lastY;

            vidDrawPos[0].x = MIN( MAX(0,vidDrawPos[0].x), (480-vidDrawScale[0].x)+30);
            vidDrawPos[0].y = MIN( MAX(0,vidDrawPos[0].y), (360-vidDrawScale[0].y)+45);

        }

    }else if( bAlterRVideo )
    {
        if( button == 0 )
        {   vidDrawScale[1].y = y-vidDrawPos[1].y;
            vidDrawScale[1].y = MAX(MIN(360,vidDrawScale[1].y),100);
            vidDrawScale[1].x = 1.33*vidDrawScale[1].y;
        }else{
            vidDrawPos[1].x += x-lastX;
            vidDrawPos[1].y += y-lastY;

            vidDrawPos[1].x = MIN( MAX(510,vidDrawPos[1].x), (480-vidDrawScale[1].x)+545);
            vidDrawPos[1].y = MIN( MAX(0,vidDrawPos[1].y), (360-vidDrawScale[1].y)+45);

        }

    }


}

