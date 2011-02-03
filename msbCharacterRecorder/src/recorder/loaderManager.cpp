#include "loaderManager.h"

loaderManager::loaderManager()
{
    //ctor
}

loaderManager::~loaderManager()
{
    //dtor
}

void loaderManager::setup( int vidW, int vidH)
{
    //imageLoader.allocate(vidW,vidH);
}

void loaderManager::loadSession( string filename )
{

    string absPath = ofToDataPath(filename,true);

    // clear current session
    rManager->reset();

    // load images
    dirLister.setExtensionToLookFor("jpg");
    dirLister.setPath(absPath);
    int nFiles = dirLister.getNumberOfFiles();
    cout << "num jpg files " << nFiles << endl;

    // load bones
    int total = nFiles / 2;
    for( int i = 0; i < total; i++)
    {
        string imageFile = filename + "/L_" + ofToString(i) + ".jpg";
        imageL.loadImage(imageFile);

        imageFile = filename + "/R_" + ofToString(i) + ".jpg";
        imageR.loadImage(imageFile);

        cout << imageFile << endl;
        rManager->addFrame(imageL.getPixels(),imageR.getPixels());
        rManager->gui.addFrame();

        //imageL.clear();
        //imageR.clear();
    }

    // load on / off frames + session data
    cout <<  "filename " << filename << endl;

    //----------------------------------------------
    ofxXmlSettings xml;
    xml.loadFile(filename+"/sessionData.xml");

    xml.pushTag("activeFrames");
    int nFrames = xml.getNumTags("on");

    for( int i = 0; i < nFrames; i++)
    {
        bool bOn = xml.getValue("on",true,i);
        rManager->gui.toggleTimeLineButton(i,bOn);

    }

    xml.popTag();


    // set in and out points
    rManager->setFrameOn(xml.getValue("in",0));
    rManager->setInPos();
    rManager->setFrameOn(xml.getValue("out",rManager->numFrames()-1));
    rManager->setOutPos();
    rManager->setFrameOn(0);

    // keying
    int r = xml.getValue("chroma:color:r",0);
    int g = xml.getValue("chroma:color:g",0);
    int b = xml.getValue("chroma:color:b",0);
    rManager->cKeyer.setFromRGB(r,g,b);

    rManager->cKeyer.setHThreshold(xml.getValue("chroma:range:hue",10));
    rManager->cKeyer.setSThreshold(xml.getValue("chroma:range:sat",10));

    rManager->fpsRec = 1.0 / xml.getValue("recordFPS",12);
    rManager->fpsPlay = 1.0 / xml.getValue("playbackFPS",6);


    // camera pos + scale
    rManager->gui.vidDrawPos[0].x = xml.getValue("leftCamera:pos:x", rManager->gui.vidDrawPos[0].x);
    rManager->gui.vidDrawPos[0].y = xml.getValue("leftCamera:pos:y", rManager->gui.vidDrawPos[0].y);
    rManager->gui.vidDrawScale[0].x = xml.getValue("leftCamera:scl:x", rManager->gui.vidDrawScale[0].x);
    rManager->gui.vidDrawScale[0].y = xml.getValue("leftCamera:scl:y", rManager->gui.vidDrawScale[0].y);

    rManager->gui.vidDrawPos[1].x = xml.getValue("rightCamera:pos:x", rManager->gui.vidDrawPos[1].x);
    rManager->gui.vidDrawPos[1].y = xml.getValue("rightCamera:pos:y", rManager->gui.vidDrawPos[1].y);
    rManager->gui.vidDrawScale[1].x = xml.getValue("rightCamera:scl:x", rManager->gui.vidDrawScale[1].x);
    rManager->gui.vidDrawScale[1].y = xml.getValue("rightCamera:scl:y", rManager->gui.vidDrawScale[1].y);


    xml.pushTag("hGuides",0);
    int totalHGuides = xml.getNumTags("pos");
    for( int i = 0; i < totalHGuides; i++)
    {
        rManager->gui.hRulers.push_back( xml.getValue("pos",0,i));
    }
    xml.popTag();

    xml.pushTag("vGuides",0);
    int totalVGuides = xml.getNumTags("pos");
    for( int i = 0; i < totalVGuides; i++)
    {
        rManager->gui.vRulers.push_back( xml.getValue("pos",0,i));
    }
    xml.popTag();


    //------------------------- load skeleton
    xml.loadFile(filename+"/bones.xml");
    xml.pushTag("skeletonFrames");

    int numSkeletons = xml.getNumTags("skeleton");

    for( int i = 0; i < numSkeletons; i++)
    {
        xml.pushTag("skeleton",i);
        int numJoints = xml.getNumTags("joint");

        // create skeleton
        bones tempBone;

        for( int j = 0; j < numJoints; j++)
        {
            xml.pushTag("joint",j);
                float x = xml.getValue("x",0.f);
                float y = xml.getValue("y",0.f);
                float z = xml.getValue("z",0.f);
                string name = xml.getValue("name","none");
                tempBone.addBoneSansParent(x,y,z,name);
            xml.popTag();
        }

        for( int j = 0; j < numJoints; j++)
        {
            xml.pushTag("joint",j);
                tempBone.findParent(j,xml.getValue("parent_name","none"));
            xml.popTag();
        }

        for( int j = 0; j < numJoints; j++)
        {
            xml.pushTag("joint",j);
                tempBone.findChildren(tempBone.skeleton[j].parent,j);
            xml.popTag();
        }

        int frameOn = xml.getValue("id",0);
        // add to keyframe
        rManager->boneMan.addBoneSet( frameOn, frameOn * rManager->fpsPlay, tempBone );

        xml.popTag();
    }



}
