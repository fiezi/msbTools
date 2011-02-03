#ifndef BONES_H
#define BONES_H


#include "ofxVectorMath.h"
#include "vmath.h"
#include <map>

typedef struct bone{
    ofxVec3f    loc;        // location on screen. z coordinate is x in the right (side) camera view
    ofxVec3f    dist;        // normalized distance between bone and parent
    string      name;       // name of bone
    int         parent;     // id of parent
    vector<int> children;
};

class bones
{
    public:
        bones();
        virtual ~bones();
        bones(const bones & B);

        //---- creation functions

        // initialize default skeleton
        // id is the frame (frameOn) this skeleton is associated with
        void init(int id);

        // init from copy of another skeleton (such as master)
        void init(int id_, bones & sister);

        // adds a new bone to the skeleton
        void addBoneToSkeleton( int x, int y, int z, int parent, string name );
        void addBoneSansParent( int x, int y, int z, string name );
        void findParent(int me, string name);

        // recursive function that sets all relationships
        bool findChildren( int me, int child );

        // clears all bones
        void reset();

        void draw();

        // test if we are on top of any joint in the skeleton
        bool hitTest(int x, int y);

        // move the selected joint (part) to x,y
        void moveSelectedTo( int x, int y, int part, bool bMoveChildren = true );

        // moves whichever point is stored in selectedJoint variable
        void moveSelectedTo( int x, int y, bool bMoveChildren = true  );

        // calculates the normalized vectors between each bone and it parent
        void calculateBoneAngles();

        // calculate the rotation between me and parent (mom) and store in rotA and rotB
        void calculate3dRotation( int me, int mom, Vector3f xAxis, Vector3f yAxis, ofxVec3f & rotA, ofxVec3f & rotB );
        void calculateRight3dRotation( int me, int mom, Vector3f xAxis, Vector3f yAxis, ofxVec3f & rotA, ofxVec3f & rotB );

        void calculateAxis(int me, int mom, Matrix3f & rotA, Matrix3f & rotB, int flip);
        void calculateSpineAxis(Matrix3f & rotA, Matrix3f & rotB, Matrix3f & rotC);
        void calculateLegAxis(int me, int mom, Matrix3f & rotA, Matrix3f & rotB, int flip);

        Matrix3f getNeckRotation(){ return neckRot; };
        Matrix3f getTorsoRotation()    { return torsoRot; };
        Matrix3f getPelvisRotation()    { return pelvisRot; };

        Matrix3f getLeftShoulderRotation(){ return leftShoulderRot; };
        Matrix3f getLeftElbowRotation()    { return leftElbowRot; };

        Matrix3f getRightShoulderRotation(){ return rightShoulderRot; };
        Matrix3f getRightElbowRotation()    { return rightElbowRot; };

        Matrix3f getLeftHipRotation()    { return leftHipRot; };
        Matrix3f getLeftKneeRotation()    { return leftKneeRot; };

        Matrix3f getRightHipRotation()    { return rightHipRot; };
        Matrix3f getRightKneeRotation()    { return rightKneeRot; };


        //----------
        vector<bone>    skeleton;           // all bones in skelton with associated parent
        map<string,int> connections;        // map of names to ids for ease in connecting bones

        int     id;                         // id of key frame this skeleton belongs to (frame in timeline)
        float   time;                       // ?? maybe not working but should be time from timeline




    protected:

        void colorJoint( string name );     // sets right and left side to different color

        int selectedSide;                   // indicates if we selected joint in left or right camera view
        int selectedJoint;

        Matrix3f neckRot;
        Matrix3f torsoRot;
        Matrix3f pelvisRot;

        Matrix3f leftShoulderRot;
        Matrix3f leftElbowRot;

        Matrix3f rightShoulderRot;
        Matrix3f rightElbowRot;

        Matrix3f leftHipRot;
        Matrix3f leftKneeRot;

        Matrix3f rightHipRot;
        Matrix3f rightKneeRot;




    private:
};

#endif // BONES_H
