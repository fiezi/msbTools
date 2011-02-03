#include "boneManager.h"

boneManager::boneManager()
{
    bSelected = false;
    masterBone.init(0);
    selectedBone = -1;
}

boneManager::~boneManager()
{
    //dtor
}


void boneManager::reset()
{
    boneset.clear();
}

void boneManager::addBoneSet(int frameOn, float time, bones & mom )
{

    int pos;

    if(boneset.size() == 0)
    {
        bones tempBone;
        boneset.push_back(tempBone);
        pos = boneset.size()-1;

        boneset[ pos ].init( frameOn,mom );
        boneset[ pos ].time  = time;

        return;
    }

    pos = boneset.size();

    for( int i = 0; i < boneset.size(); i++)
    {
        if( boneset[i].id == frameOn ) return;
        if( boneset[i].id > frameOn ) pos = i;
    }

    bones tempBone;
    boneset.insert(boneset.begin()+pos,tempBone);
    boneset[ pos ].init( frameOn, mom );
    boneset[ pos ].time  = time;

}

void boneManager::removeBoneSet( int frameOn )
{
    int id = hasKeyFrame( frameOn );
    if( id >= 0 )
    {
        boneset.erase(boneset.begin()+id);
    }

}

void boneManager::draw( int frameOn )
{


    ofFill();
    if( boneset.size() > 0 )
    {

        ofSetColor(255,255,255,200);
        for( int i = 0; i < boneset.size(); i++)
        {
            if( frameOn == boneset[i].id )
            {
                boneset[i].draw();
                return;

            }
        }
    }

    ofSetColor(255,255,255,255);
    masterBone.draw();


}


bool boneManager::editBones( int x, int y, bool bShiftOn )
{
    //cout << "selected bone " << selectedBone << endl;

    if( bSelected )
    {
        if( selectedBone >= 0 && boneset.size() >selectedBone )
            boneset[ selectedBone ].moveSelectedTo(x,y,!bShiftOn);
        else masterBone.moveSelectedTo(x,y,!bShiftOn);

        return true;
    }

    return false;

}


void boneManager::checkSelectBone( int x, int y, int frameOn)
{

    //cout << "check slect: frameOn " << frameOn << endl;

    // check if this frame has a key already
    if(boneset.size() > 0 )
    {
        for( int i = 0; i < boneset.size(); i++)
        {
            if( frameOn == boneset[i].id )
            {
                if( boneset[i].hitTest(x,y)  )
                {
                    bSelected = true;
                    selectedBone = i;
                    cout << "select id " << boneset[i].id << " i " << i << endl;
                    return;
                }
            }
        }

    }

    //if(boneset.size() < frameOn || boneset.size() == 0) return;

    if( masterBone.hitTest(x,y)  )
    {
        bSelected = true;
        selectedBone = -1;
    }



}

void boneManager::releaseSelectBone()
{
    bSelected = false;
}


int boneManager::hasKeyFrame( int frameOn )
{
    for( int i = 0; i < boneset.size(); i++)
    {
        if( boneset[i].id == frameOn )
                return i;
    }

    return -1;
}

