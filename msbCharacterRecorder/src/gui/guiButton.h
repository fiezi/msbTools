#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "ofMain.h"
#include "guiUtils.h"


class guiButton
{
    public:
        guiButton();
        virtual ~guiButton();
        guiButton( const guiButton & g);

        void setup(int x, int y, int w, int h, bool bOn = false);
        bool update(int x, int y, bool bTurnOnOff = true);
        void draw();
        void setId( int i ) { id = i; }
        void setOn( bool bOn_ ){ bOn = bOn_; };


        float getX(){return hitBox.x;};
        float getY(){ return hitBox.y; };

        bool  bOn;
        int id;
        ofRectangle hitBox;

    protected:
    private:
};

#endif // GUIBUTTON_H
