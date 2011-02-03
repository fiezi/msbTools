#ifndef ARTKPMULTITRACKER_H
#define ARTKPMULTITRACKER_H

#include "ofMain.h"
#include "ar.h"

class arTKPMultiTracker
{
    public:
        arTKPMultiTracker();
        virtual ~arTKPMultiTracker();

        bool setup(int width, int height, bool useBCH= false);
        void update( unsigned char * pixels);
        void draw();

        float armatrix[16];
        ofTrueTypeFont idNumb;

    protected:
    private:
};

#endif // ARTKPMULTITRACKER_H
