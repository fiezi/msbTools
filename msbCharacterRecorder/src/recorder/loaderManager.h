#ifndef LOADERMANAGER_H
#define LOADERMANAGER_H

/*
Loads pre-recorded sessions.
*/

#include "ofxXmlSettings.h"
#include "recorderManager.h"
#include "pocoDirectoryLister.h"

class loaderManager
{
    public:
        loaderManager();
        virtual ~loaderManager();

        void setup( int vidW, int vidH);
        void loadSession( string filename );

        recorderManager * rManager;
        ofxPocoDirectoryLister dirLister;

    protected:

        ofImage imageL, imageR;

    private:
};

#endif // LOADERMANAGER_H
