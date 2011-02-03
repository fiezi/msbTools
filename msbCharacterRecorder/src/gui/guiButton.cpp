#include "guiButton.h"

guiButton::guiButton()
{
    hitBox = ofRectangle(0,0,10,10);
    bOn = false;
    id = 0;
}

guiButton::~guiButton()
{
    //dtor
}

guiButton::guiButton( const guiButton & g)
{
    hitBox  = ofRectangle(g.hitBox.x,g.hitBox.y,g.hitBox.width,g.hitBox.height);
    bOn     = g.bOn;
    id      = g.id;
}

void guiButton::setup(int x, int y, int w, int h, bool bOn_)
{
    hitBox = ofRectangle(x,y,w,h);
    bOn = bOn_;
}

bool guiButton::update(int x, int y, bool bTurnOnOff )
{
    if( pointInRectangle( hitBox,x,y) )
    {
        if(bTurnOnOff) bOn = !bOn;
        return true;
    }

    return false;


}

void guiButton::draw()
{

    ofRect( hitBox.x,hitBox.y,hitBox.width, hitBox.height);
}


