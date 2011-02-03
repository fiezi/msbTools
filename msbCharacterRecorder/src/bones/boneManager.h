#ifndef BONEMANAGER_H
#define BONEMANAGER_H

#include "ofxVectorMath.h"
#include "ofMain.h"
#include "bones.h"

/*
*/


class boneManager
{
    public:
        boneManager();
        virtual ~boneManager();


        // clear current set of saved skeleton key frames
        void reset();

        // add a new skeleton key frame
        void addBoneSet( int frameOn, float time, bones & mom  );

        // delete a skeleton key frame
        void removeBoneSet( int frameOn );

        // draw current skeleton - either master or saved key frame skeleton
        void draw( int frameOn );

        // checks if we are on a bone joint
        void checkSelectBone( int x, int y, int frameOn);
        void releaseSelectBone();

        // returns true if we are draging a joint on the skeleton
        bool editBones( int x, int y, bool bMoveChildren = false);

        // checks whether the current frame we are on has a skeleton key frame saved
        // if it does returns id of the skelton key frame else return -1
        int hasKeyFrame( int frameOn );

        // vector of all saved key frames
        vector< bones > boneset;

        // master set of bones
        bones   masterBone;

    protected:


        bool bSelected;         // true if a joint is selected (when mouse dragging)
        int selectedBone;       // id of current joint selected


    private:
};

#endif // BONEMANAGER_H
