#include "recorderManager.h"
#include <windows.h>

recorderManager::recorderManager()
{
    bRecording      = false;
    bPaused         = true;
    fpsRec          = 1/12.0;                   // 12 frames per second record
    fpsPlay         = 1/6.0;                    // 6 frames per second playback
    startTime       = ofGetElapsedTimef();
    timer           = 0;
    frameOn         = 0;
    recordCounter   = 0;
    bTypingName     = false;
    bKeying         = false;
    bShiftMouseOn   = false;

    font.loadFont("fonts/verdana.ttf",8, false, true);

    cKeyer.setFromRGB(0,255,0);
    outputDir = "";

    inPos = -1;
    outPos = -1;

}

recorderManager::~recorderManager()
{
    //dtor
}

void recorderManager::setup( int w, int h )
{
    vidW = w;
    vidH = h;

    // allocate images for keying
    keySrc.allocate(vidW, vidH);
    keyDst.allocate(vidW, vidH);
    keyDst.set(0,0,0);

}

void recorderManager::reset()
{
    // clear everything so we can start a new sequence
    framesL.clear();
    framesR.clear();

    thumbL.clear();
    thumbR.clear();

    gui.resetFrames();

    boneMan.reset();

    inPos = 0;
    outPos = 0;

}


void recorderManager::update( unsigned char * pixelsL, unsigned char * pixelsR )
{
    float timeNow   = ofGetElapsedTimef();
    float timeDiff  = (timeNow - startTime) - timer;


    if( bRecording )
    {

        if(timeDiff > fpsRec)
        {
            // increment framerate counter
            timer += fpsRec;

            // push back images for each camera
            addFrame(pixelsL,pixelsR);

            // add button to timeline for this frame
            gui.addFrame();

        }

    }else{

       if( framesL.size() == 0 ) return;


       if(timeDiff > fpsPlay)
        {

            timer += fpsPlay;

            if( !bPaused )
            {
               // move to next frame (with skipping of "off" frames)
               frameOn = getNextFrame();

               // adjust timline so current frame always onscreen
               gui.adjustTimeLinePosition(frameOn);

               gui.timeSlider.setValue(  (float)frameOn  / framesL.size() );


            }

            // key current frame
            keyDst.set(0,0,0);
            keySrc.setFromPixels(framesL[frameOn].getPixels(),vidW, vidH);
            cKeyer.keyImage(keySrc,keyDst,vidW, vidH);//cvResize(keySrc.getCvImage(),keyDst.getCvImage());

            // make thumbnails for frames
            makeThumbnails();
        }

    }



}

void recorderManager::addFrame(unsigned char * pixelsL, unsigned char * pixelsR)
{
    ofImage image;
    image.setFromPixels(pixelsL, vidW, vidH, OF_IMAGE_COLOR);
    framesL.push_back(image);

    image.setFromPixels(pixelsR, vidW, vidH, OF_IMAGE_COLOR);
    framesR.push_back(image);
}


int recorderManager::getNextFrame()
{
    // searches and returns the next active frame in the timeline
    int myFrame = frameOn;

    for( int i = frameOn+1; i < frameOn+framesL.size(); i++)
    {
        int f = i % framesL.size();
        if( gui.timeLine[ f ].bOn )
        {
            myFrame = i % framesL.size();
            break;
        }
    }

    return myFrame;

}

void recorderManager::mouseReleased(int x, int y, int button)
{
    boneMan.releaseSelectBone();
    bShiftMouseOn = false;

    gui.releaseRulers();
    gui.bAlterLVideo = false;
    gui.bAlterRVideo = false;
}

void recorderManager::mouseMoved(int x, int y)
{
    //lastX = x;
    //lastY = y;
}

void recorderManager::mousePressed(int x, int y, int button)
{

    lastX = x;
    lastY = y;

    if( glutGetModifiers() == GLUT_ACTIVE_SHIFT ) bShiftMouseOn = true;

    // check save;
    if( gui.saveButton.update(x,y) ){
         save();
         return;
    }

    if( gui.saveSessionButton.update(x,y) ){
         saveSessionData("recordings/"+outputDir+"/");
         return;
    }


    // check keying
    if( gui.chromaOnButton.update(x,y) )
    {
        bKeying = gui.chromaOnButton.bOn;
        return;
    }

    // check typing folder name box
    if( gui.folderNameButton.update(x,y) )
    {
        return;
    }else if( gui.folderNameButton.bOn )
        gui.folderNameButton.bOn = false;

    // check if we slected a new frame and if so, set that to current frame
    int id = gui.hitTest(x,y,button);
    if( id >= 0 && button == 0)
    {
        frameOn = id;
        gui.timeSlider.setValue(  (float)frameOn  / framesL.size() );
        return;
    }


    // check if we selected a bone to move
    boneMan.checkSelectBone(x,y, frameOn);


    // check the record button
    if( gui.recordButton.update(x,y,true) )
    {
       cout << " record " << endl;
       toggleRecording( gui.recordButton.bOn );
       return;
    }

    // check play/pause button
    if( gui.playButton.update(x,y,true) )
    {
       bPaused =  !gui.playButton.bOn;
       togglePaused(bPaused);
       return;
    }

    // check the clear button
    if( gui.clearButton.update(x,y,false) )
    {
       reset();
       return;
    }

    // check add bone
    if( framesL.size() > 0  && gui.addBoneButton.update(x,y,false))
    {
        boneMan.addBoneSet( frameOn, frameOn * fpsPlay, boneMan.masterBone );
        return;
    }

    // check remove bone
    if( framesL.size() > 0  && gui.removeBoneButton.update(x,y,false))
    {
        boneMan.removeBoneSet( frameOn );
        return;
    }

    // check rulers
    gui.selectRulers(x,y);

    // check video boxes
    //bAlterVideoDraw = false;
    ofRectangle RCam = ofRectangle((gui.vidDrawPos[1].x+gui.vidDrawScale[1].x)-15,(gui.vidDrawPos[1].y+gui.vidDrawScale[1].y)-15,30,30);
    ofRectangle LCam = ofRectangle((gui.vidDrawPos[0].x+gui.vidDrawScale[0].x)-15,(gui.vidDrawPos[0].y+gui.vidDrawScale[0].y)-15,30,30);
    if( pointInRectangle(LCam, x,y) ) gui.bAlterLVideo = true;
    else if( pointInRectangle(RCam, x,y) ) gui.bAlterRVideo = true;
}

bool recorderManager::mouseDragged(int x, int y, int button)
{

    // check editing bones
    if( boneMan.editBones( x, y, bShiftMouseOn ) )
    {
        return true;
    }

    // check if we are dragging the timline scrubber
    if( framesL.size() > 0 && gui.timeSlider.update(x,y) )
    {

     // pause in case we are playing
     togglePaused(true);

     // set current frame to position from slider
     frameOn = (int)(gui.timeSlider.getPctValue() * framesL.size() );

     // adjust timeline in case new frame is offf screen
     gui.adjustTimeLinePosition(frameOn);
     return true;
    }

    // check framerate slider
    if( gui.fpsSlider.update(x,y) )
    {
        // set new framerate
        fpsPlay = 1 / (gui.fpsSlider.getPctValue()*30);
        return true;
    }

    // check chroma keying sliders and update accordingly
    if( gui.rSlider.update(x,y) ){       cKeyer.setFromRGB( gui.rSlider.getIntValue(), cKeyer.G,cKeyer.B); return true; }
    else if( gui.gSlider.update(x,y) ){  cKeyer.setFromRGB( cKeyer.R, gui.gSlider.getIntValue(),cKeyer.B); return true; }
    else if( gui.bSlider.update(x,y) ){  cKeyer.setFromRGB( cKeyer.R, cKeyer.G,gui.bSlider.getIntValue()); return true; }

    if( gui.htSlider.update(x,y) ){      cKeyer.setHThreshold( gui.htSlider.getIntValue() ); return true; }
    else if( gui.stSlider.update(x,y) ){ cKeyer.setSThreshold( gui.stSlider.getIntValue() ); return true; }

    // check if we are slecting a color from current frame
    ofRectangle rect = ofRectangle( 30,30,vidW,vidH);
    if( bShiftMouseOn && bKeying && pointInRectangle( rect, x,y) )
    {
        cKeyer.getColorFromScreen((x-30)*(vidW/480.f),(y-30)*(vidH/360.f),vidW,keySrc);
        gui.rSlider.setValue(cKeyer.R);
        gui.gSlider.setValue(cKeyer.G);
        gui.bSlider.setValue(cKeyer.B);
        return true;
    }

    gui.alterVideo( x,  y,  lastX,  lastY,  button);

    if( gui.bHRuler ) gui.updateRuler(x,y,false);
    else if( gui.bVRuler) gui.updateRuler(x,y,true);

    lastX = x;
    lastY = y;

    return false;
}

void recorderManager::draw()
{

    //----- show preview
    ofNoFill();
    ofRect((gui.vidDrawPos[0].x+gui.vidDrawScale[0].x)-15,(gui.vidDrawPos[0].y+gui.vidDrawScale[0].y)-10,20,20);
    ofRect((gui.vidDrawPos[1].x+gui.vidDrawScale[1].x)-15,(gui.vidDrawPos[1].y+gui.vidDrawScale[1].y)-10,20,20);

    //----- draw boxes for frames (covered with thumbnails after)
    if( !bRecording && framesL.size() > 0 )
    {
        framesL[ frameOn ].draw(gui.vidDrawPos[0].x,gui.vidDrawPos[0].y,gui.vidDrawScale[0].x,gui.vidDrawScale[0].y );//700, 30, vidW*.25f, vidH*.25f );
        framesR[ frameOn ].draw(gui.vidDrawPos[1].x,gui.vidDrawPos[1].y,gui.vidDrawScale[1].x,gui.vidDrawScale[1].y );//700, 30, vidW*.25f, vidH*.25f );
    }

    //---- draw in and out points on timeline
    drawInOutPoints();

    //---- draw bones/skeleton
    boneMan.draw(frameOn);

    //----- show recorded timeline
    drawTimeline();

    //---- draw keying color
    cKeyer.drawColor(360,590,36);
    ofSetColor(255,255,255);

    //---- draw keying preview
    if( bKeying)
    {
        keySrc.draw(500,590,160,120);
        keyDst.draw(670,590,160,120);
        cKeyer.hueImg.draw(670+170,590,160,120);
    }


    //----- show gui
    gui.draw(font);

    //----- folder name
    font.drawString( outputDir, gui.folderNameButton.getX()+3, gui.folderNameButton.getY()+12 );

    //----- draw current frame num on slider
    font.drawString(ofToString( frameOn ), gui.timeSlider.getX(), gui.timeSlider.getY() + 38 );

    //----- some labels
    ofSetColor(0,0,0);
    ofRect(30, 15,200,15);
    ofRect(34+480, 15,40,15);
    ofSetColor(255,255,255);
    font.drawString("Front", 32, 25 );
    font.drawString("( 'c' to swap cameras )", 80, 25 );
    font.drawString("Side", 36+480, 25 );

    //----- show stats
    ofSetColor(255,255,255);
    int dataY = 660;
    font.drawString("Total Frames: ", 30, dataY );
    font.drawString(ofToString( gui.totalOn-1 ), 300, dataY );

    font.drawString("Total Keys: ", 30, dataY+=20 );
    font.drawString(ofToString( (int)(boneMan.boneset.size()) ), 300, dataY );

    font.drawString("FPS: ", 30, dataY+=20 );
    font.drawString(ofToString( gui.fpsSlider.getPctValue()*30,0 ), 300, dataY);

    font.drawString( ofToString(ofGetFrameRate(), 2 ), 300, dataY+=20 );

}

void recorderManager::drawTimeline()
{

    int offScreenX = ofGetWidth()-60;

    ofEnableAlphaBlending();

    ofFill();

    // draw any recorded skeleton keyframes
    for( int i = 0; i < boneMan.boneset.size(); i++)
    {
        int f = boneMan.boneset[i].id;
        if(gui.timeLine[f].hitBox.x > offScreenX || gui.timeLine[f].hitBox.x < gui.timeLineX ) continue;

        ofCircle(gui.timeLine[f].getX()+(gui.timeLineW/2), gui.timeLine[f].getY()+gui.timeLineH+10,6);
    }

    // draw timeline key frames
    for( int i = 0; i < gui.timeLine.size(); i++)
    {

        // don't draw any frames not onscreen
        if(gui.timeLine[i].hitBox.x > offScreenX || gui.timeLine[i].hitBox.x < gui.timeLineX ) continue;

        if( gui.timeLine[i].bOn )
        {
            ofFill();
            ofSetColor(255,255,255,(int)(.4*255));
            gui.timeLine[i].draw();

            // show thumbnail of each frame
            ofSetColor(255,255,255,255);
            if( thumbL.size() > i ) thumbL[i].draw(gui.timeLine[i].getX(), gui.timeLine[i].getY());
            if( thumbR.size() > i ) thumbR[i].draw(gui.timeLine[i].getX(), gui.timeLine[i].getY()+(gui.timeLineH/2));
        }

        // draw border
        ofSetColor(255,255,255,100);
        ofNoFill();
        gui.timeLine[i].draw();


        // label frame number
        ofFill();
        ofSetColor(255,255,255,200);
        ofRect(gui.timeLine[i].getX(),gui.timeLine[i].getY(),18,15);
        ofSetColor(0,0,0,255);
        font.drawString( ofToString( gui.timeLine[i].id),gui.timeLine[i].getX()+2,gui.timeLine[i].getY()+10 );


    }

    // highlight current frame
    ofNoFill();
    ofSetColor(255,0,0,255);
    glLineWidth(3);
    if(gui.timeLine.size() > frameOn ) gui.timeLine[frameOn].draw();
    glLineWidth(1);

    ofDisableAlphaBlending();

}

void recorderManager::toggleRecording(bool bOn)
{
    bRecording = bOn;
    timer = 0;
    startTime = ofGetElapsedTimef();

    gui.recordButton.setOn( bRecording );
    cout << "Recording: " << bRecording << endl;
}

void recorderManager::toggleRecording()
{
    bRecording = !bRecording;
    timer = 0;
    startTime = ofGetElapsedTimef();

    gui.recordButton.setOn( bRecording );
    cout << "Recording: " << bRecording << endl;
}

void recorderManager::togglePaused()
{
    bPaused = !bPaused;
}

void recorderManager::togglePaused(bool bOn)
{
    bPaused = bOn;
}

void recorderManager::makeThumbnails()
{
    // scales and creates small thumbnails for each frame
    if( thumbL.size() < framesL.size() )
    {
        ofImage thumbnailL;
        thumbnailL.allocate( vidW, vidH, OF_IMAGE_COLOR );

        thumbnailL.setFromPixels( framesL[ thumbL.size() ].getPixels(), vidW, vidH, OF_IMAGE_COLOR);
        thumbnailL.resize(gui.timeLineW,gui.timeLineH/2);
        thumbL.push_back( thumbnailL );

        ofImage thumbnailR;
        thumbnailR.allocate( vidW, vidH, OF_IMAGE_COLOR );
        thumbnailR.setFromPixels( framesR[ thumbR.size() ].getPixels(), vidW, vidH, OF_IMAGE_COLOR);
        thumbnailR.resize(gui.timeLineW,gui.timeLineH/2);

        thumbR.push_back( thumbnailR );
    }

}

//void  recorderManager::saveKeyFrames()
//{
//    // save key frames to xml file
//    ofxXmlSettings xml;
//
//    // store each set of bones recorded
//
//    for( int i = 0; i < boneMan.boneset.size(); i++)
//    {
//        xml.addTag("bone");
//        xml.pushTag("bone",i);
//
//            // save key frame associated with
//            xml.setValue("id",boneMan.boneset[i].id);
//
//            // save positions
//            for( int j = 0; j < boneMan.boneset[i].rJoints.size(); j++)
//            {
//                xml.setValue("r_joint_"+ofToString(j)+":x",boneMan.boneset[i].rJoints[j].x);
//                xml.setValue("r_joint_"+ofToString(j)+":y",boneMan.boneset[i].rJoints[j].y);
//            }
//
//            for( int j = 0; j < boneMan.boneset[i].lJoints.size(); j++)
//            {
//                xml.setValue("l_joint_"+ofToString(j)+":x",boneMan.boneset[i].lJoints[j].x);
//                xml.setValue("l_joint_"+ofToString(j)+":y",boneMan.boneset[i].lJoints[j].y);
//            }
//
//
//        xml.popTag();
//
//    }
//
//    string relPath = outputDir+"/";//rec_"+ofToString(recordCounter);
//    xml.saveFile( relPath+"/bones.xml" );
//}
void recorderManager::saveSessionData(string relPath)
{
    // save info about this session to xml
    // saves in recordings --> my_project_name --> sessionData.xml
    ofxXmlSettings xml;

    // store on and off frames
    xml.addTag("activeFrames");
    xml.pushTag("activeFrames",0);
    for( int i = 0; i < framesL.size(); i++)
    {

        xml.addTag("on");
        xml.setValue("on",gui.timeLine[i].bOn,i);

    }
    xml.popTag();

    // store framerate
    xml.setValue("recordFPS",1.0/fpsRec);
    xml.setValue("playbackFPS",1.0/fpsPlay);

    // store in and out points
    xml.setValue("in:",inPos);
    xml.setValue("out",outPos);

    // store chroma value
    xml.setValue("chroma:color:r",cKeyer.R);
    xml.setValue("chroma:color:g",cKeyer.G);
    xml.setValue("chroma:color:b",cKeyer.B);
    xml.setValue("chroma:range:hue",cKeyer.tH);
    xml.setValue("chroma:range:sat",cKeyer.tS);

    // store camera position + scale
    xml.setValue("leftCamera:pos:x", gui.vidDrawPos[0].x);
    xml.setValue("leftCamera:pos:y", gui.vidDrawPos[0].y);
    xml.setValue("leftCamera:scl:x", gui.vidDrawScale[0].x);
    xml.setValue("leftCamera:scl:y", gui.vidDrawScale[0].y);

    xml.setValue("rightCamera:pos:x", gui.vidDrawPos[1].x);
    xml.setValue("rightCamera:pos:y", gui.vidDrawPos[1].y);
    xml.setValue("rightCamera:scl:x", gui.vidDrawScale[1].x);
    xml.setValue("rightCamera:scl:y", gui.vidDrawScale[1].y);

    // saves guides
    xml.addTag("hGuides");
    xml.pushTag("hGuides",0);
    for( int i = 0; i < gui.hRulers.size(); i++)
    {
        xml.addTag("pos");
        xml.setValue("pos",gui.hRulers[i],i);
    }
    xml.popTag();

    xml.addTag("vGuides");
    xml.pushTag("vGuides",0);
    for( int i = 0; i < gui.vRulers.size(); i++)
    {
        xml.addTag("pos");
        xml.setValue("pos",gui.vRulers[i],i);
    }
    xml.popTag();


    xml.saveFile( relPath+"/sessionData.xml" );
    cout << "relPath " + relPath << endl;

    //save skeletonData too when we save sessions!
    saveSkeletonData(relPath);
}

void recorderManager::save()
{
    // pause so we don't jump ahead
    togglePaused(true);

    // make new directory if have not already
    cout << "Warning: Windows Specific Code." << endl;

    if( outputDir == "" ) outputDir = "rec_"+ofToString( ofGetYear() )+ "_" + ofToString( ofGetMonth() ) + "_" + ofToString( ofGetDay() ) + "_"+ ofToString( ofGetHours() ) + "_" + ofToString( ofGetMinutes() ) + "_" + ofToString( ofGetSeconds() );

    string makedir = ofToDataPath("recordings/"+outputDir,true);

    if(mkdir(makedir.c_str())==-1)//creating a directory
    {
        cerr<<"Error Creating Directory:  "<<strerror(errno)<<endl;
    }

    // save tgas
    saveFrames();

    // save session data (on and off frames, etc.);
    string relPath = "recordings/"+outputDir+"/";
    saveSessionData(relPath);

    // save skeleton data
    saveSkeletonData(relPath);

}

void recorderManager::saveSkeletonData(string path)
{


    cout << "saving skeleton data..." << endl;

    ofxXmlSettings xml;

    xml.addTag("skeletonFrames");
    xml.pushTag("skeletonFrames",0);

    // loop through all skeletons and save data
    for( int i = 0; i < boneMan.boneset.size(); i++)
    {
        xml.addTag("skeleton");
        xml.pushTag("skeleton",i);
        // id of keyframe
        xml.setValue("id",boneMan.boneset[i].id);


        boneMan.boneset[i].calculateBoneAngles();
        string content="mat3f ";
        //save bone matrices per set
        Matrix3f neck = boneMan.boneset[i].getNeckRotation();
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",neck.data[i]);
            content.append(" ");
            content.append(str);
        }

        xml.setValue("neckRot",content);


        Matrix3f torso = boneMan.boneset[i].getTorsoRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",torso.data[i]);
            content.append(" ");
            content.append(str);
        }

        xml.setValue("torsoRot",content);


        Matrix3f pelvis= boneMan.boneset[i].getPelvisRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",pelvis.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("pelvisRot",content);


        Matrix3f leftShoulder = boneMan.boneset[i].getLeftShoulderRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",leftShoulder.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("leftShoulderRot",content);


        Matrix3f leftElbow = boneMan.boneset[i].getLeftElbowRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",leftElbow.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("leftElbowRot",content);


        Matrix3f rightShoulder = boneMan.boneset[i].getRightShoulderRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",rightShoulder.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("rightShoulderRot",content);


        Matrix3f rightElbow = boneMan.boneset[i].getRightElbowRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",rightElbow.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("rightElbowRot",content);


        Matrix3f leftHip = boneMan.boneset[i].getLeftHipRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",leftHip.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("leftHipRot",content);


        Matrix3f leftKnee = boneMan.boneset[i].getLeftKneeRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",leftKnee.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("leftKneeRot",content);

        Matrix3f rightHip = boneMan.boneset[i].getRightHipRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",rightHip.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("rightHipRot",content);


        Matrix3f rightKnee = boneMan.boneset[i].getRightKneeRotation();
        content="mat3f ";
        for (int i=0;i<9;i++){
            char* str = new char[30];
            sprintf(str,"%f",rightKnee.data[i]);
            content.append(" ");
            content.append(str);
        }
        xml.setValue("rightKneeRot",content);



        // location of all nodes
        for( int j = 0; j < boneMan.boneset[i].skeleton.size(); j++)
        {
            xml.addTag("joint");
            xml.pushTag("joint",j);

            xml.setValue("name",boneMan.boneset[i].skeleton[j].name);
            xml.setValue("x",boneMan.boneset[i].skeleton[j].loc.x);
            xml.setValue("y",boneMan.boneset[i].skeleton[j].loc.y);
            xml.setValue("z",boneMan.boneset[i].skeleton[j].loc.z);

            int parent = boneMan.boneset[i].skeleton[j].parent;
            xml.setValue("parent_name",boneMan.boneset[i].skeleton[parent].name);

            xml.popTag();

        }
        xml.popTag();
    }

    xml.popTag();
    xml.saveFile(path+"/bones.xml" );
}

void recorderManager::saveFrames()
{

    // save images for this recordings
    string relPath = "recordings/"+outputDir+"/";
    cout << relPath << endl;


    int imageCount = 0;
    ofImage temp;
    temp.allocate(vidW,vidH,OF_IMAGE_COLOR);
    for( int i = 0; i < framesL.size(); i++)
    {
        if( gui.timeLine[i].bOn )
        {
            // keying
            keySrc.setFromPixels(framesL[i].getPixels(), vidW,vidH);
            if( bKeying ){
                cKeyer.keyImage(keySrc,keyDst,vidW,vidH);
                temp.setFromPixels(keyDst.getPixels(),vidW,vidH,OF_IMAGE_COLOR);
            }else{
                temp.setFromPixels(keySrc.getPixels(),vidW,vidH,OF_IMAGE_COLOR);
            }


            temp.saveImage(relPath+"/L_"+ofToString(imageCount)+".jpg");

            keySrc.setFromPixels(framesR[i].getPixels(), vidW,vidH);

            if( bKeying )
            {
                cKeyer.keyImage(keySrc,keyDst,vidW,vidH);
                temp.setFromPixels(keyDst.getPixels(),vidW,vidH,OF_IMAGE_COLOR);
            }else{
                temp.setFromPixels(keySrc.getPixels(),vidW,vidH,OF_IMAGE_COLOR);
            }

            temp.saveImage(relPath+"/R_"+ofToString(imageCount)+".jpg");

            imageCount++;
        }

    }

    recordCounter++;



}

void recorderManager::computeRotation()
{
    if( boneMan.boneset.size() > 0 )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );

        if(bnId>=0)
            return boneMan.boneset[bnId].calculateBoneAngles();

    }

    boneMan.masterBone.calculateBoneAngles();
}


Matrix3f recorderManager::getNeckRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getNeckRotation();
    }


    return boneMan.masterBone.getNeckRotation();
}

Matrix3f recorderManager::getTorsoRotation(){
    //Matrix3f myMatrix;
    //return myMatrix;

    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getTorsoRotation();
    }


    return boneMan.masterBone.getTorsoRotation();
}

Matrix3f recorderManager::getPelvisRotation(){
    //Matrix3f myMatrix;
    //return myMatrix;

    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getPelvisRotation();
    }


    return boneMan.masterBone.getPelvisRotation();
}



Matrix3f recorderManager::getLeftShoulderRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getLeftShoulderRotation();
    }


    return boneMan.masterBone.getLeftShoulderRotation();
}

Matrix3f recorderManager::getLeftElbowRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getLeftElbowRotation();
    }

    return boneMan.masterBone.getLeftElbowRotation();
}

Matrix3f recorderManager::getRightShoulderRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getRightShoulderRotation();
    }


    return boneMan.masterBone.getRightShoulderRotation();
}

Matrix3f recorderManager::getRightElbowRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getRightElbowRotation();
    }

    return boneMan.masterBone.getRightElbowRotation();
}


Matrix3f recorderManager::getLeftHipRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getLeftHipRotation();
    }

    return boneMan.masterBone.getLeftHipRotation();
}


Matrix3f recorderManager::getLeftKneeRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getLeftKneeRotation();
    }

    return boneMan.masterBone.getLeftKneeRotation();
}


Matrix3f recorderManager::getRightHipRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getRightHipRotation();
    }

    return boneMan.masterBone.getRightHipRotation();
}


Matrix3f recorderManager::getRightKneeRotation(){
    if( boneMan.boneset.size() > 0  )
    {
        int bnId = boneMan.hasKeyFrame( frameOn );
        if(bnId>=0) return boneMan.boneset[bnId].getRightKneeRotation();
    }

    return boneMan.masterBone.getRightKneeRotation();
}

void recorderManager::enterFolderName( char key )
{
    if( outputDir.size() > 0 && (key == OF_KEY_DEL || key == OF_KEY_BACKSPACE ) ) outputDir.erase(outputDir.size()-1);
    else if( font.stringWidth( outputDir ) < gui.folderNameButton.hitBox.width-12 ) outputDir.push_back(key);
}

void recorderManager::setInPos()
{

    inPos = frameOn;
    cout << "inPos " << inPos << endl;

    if( inPos > 0 )
    {
        for( int i = inPos-1; i >= 0; i--)
            gui.toggleTimeLineButton(i,false);
    }

    if( outPos > inPos )
    {
        for( int i = inPos; i < outPos; i++)
        {
            gui.toggleTimeLineButton(i,true);
        }
    }


}

void recorderManager::setOutPos()
{

    if( frameOn > inPos ) outPos = frameOn;
    else return;

    if( outPos > framesL.size()-1 ) outPos = framesL.size()-1;

    if( outPos < framesL.size()-1 )
    {
        for( int i = outPos+1; i < framesL.size(); i++)
            gui.toggleTimeLineButton(i,false);


    for( int i = inPos; i <= outPos; i++)
    {
            gui.toggleTimeLineButton(i,true);
    }

    }

    cout << "outPos " << outPos << endl;

}

void recorderManager::drawInOutPoints()
{
    int offScreenX = ofGetWidth()-60;

    int inF = inPos;
    int oF = outPos;

    ofNoFill();
    if(inF >= 0 && gui.timeLine[inF].hitBox.x <= offScreenX && gui.timeLine[inF].hitBox.x >= gui.timeLineX )
    {
            //ofLine(gui.timeLine[inF].getX()+(gui.timeLineW/2), gui.timeLine[inF].getY()-8,gui.timeLine[oF].getX()+(gui.timeLineW/2), gui.timeLine[oF].getY()-8);
            ofCircle(gui.timeLine[inF].getX()+(gui.timeLineW/2), gui.timeLine[inF].getY()-8,4);
    }

    ofFill();

    if(oF >= 0 && gui.timeLine[oF].hitBox.x < offScreenX && gui.timeLine[oF].hitBox.x > gui.timeLineX )
        ofCircle(gui.timeLine[oF].getX()+(gui.timeLineW/2), gui.timeLine[oF].getY()-8,4);



}

void recorderManager::swapCameras()
{
    for( int i = 0; i < numFrames(); i++)
    {
        ofImage tempImage = framesL[i];
        framesL[i] = framesR[i];
        framesR[i] = tempImage;

        ofImage tempTImage = thumbL[i];
        thumbL[i] = thumbR[i];
        thumbR[i] = tempTImage;

    }

}

