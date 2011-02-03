#include "guiSlider.h"

guiSlider::guiSlider()
{
    //ctor
}

guiSlider::~guiSlider()
{
    //dtor
}



//------------------------------------------------
guiSlider::guiSlider(string sliderName, float defaultVal, float min_, float max_)
{
		value 	= defaultVal;
		min 	= min_;
		max 	= max_;
		name 	= sliderName;
		xpos 	= 0;
		ypos 	= 0;
		w 		= 100;
		h 		= 20;

		hitArea = ofRectangle(xpos,ypos,w,h);

}
void guiSlider::setup( string sliderName, float defaultVal, float min_, float max_, float x, float y, float w_, float h_ )
{
		value 	= (defaultVal-min_)/max_;
		min 	= min_;
		max 	= max_;
		name 	= sliderName;
		xpos 	= x;
		ypos 	= y;
		w 		= w_;
		h 		= h_;

		hitArea = ofRectangle(xpos,ypos,w,h);

}


//-----------------------------------------------.
void guiSlider::setup( float x, float y, float w_, float h_ )
{
		xpos = x;
		ypos = y;
		w = w_;
		h = h_;
		hitArea = ofRectangle(xpos,ypos,w,h);
}

bool guiSlider::update(int x, int y)
{
        bool bHit = hitTest(x,y);
        if( bHit )
        {
           x-=xpos;
           y-=ypos;
           value = ( (x-min) / w);
        }

        return bHit;
}

//	-----------------------------------------------.
bool guiSlider::hitTest( int x, int y )
{
    return pointInRectangle( hitArea, x, y );

}

//-----------------------------------------------.
void guiSlider::draw()
{

    ofSetColor(255,255,255,200);
    ofFill();
    ofRect(xpos,ypos,w*value,h);

    ofSetColor(255,255,255,255);
    ofNoFill();
    ofRect(xpos,ypos,w,h);

    //ofDrawBitmapString( ofToString( getFloatValue(),0 ), xpos + w + 4, ypos + h );
    //ofDrawBitmapString( name, xpos, ypos - 4 );

}

//-----------------------------------------------.
void guiSlider::drawAsMarker()
{
    ofSetColor(255,255,255,200);
    ofFill();
    ofRect(xpos+ (w*value-2),ypos,4,h+10);

    ofSetColor(255,255,255,255);
    ofNoFill();
    ofRect(xpos,ypos,w,h);
}

 //--------------------------------
int guiSlider::getIntValue()
 {
     return (int)(max*value);
 }

 float guiSlider::getFloatValue()
 {
     return (max*value);
 }

float guiSlider::getPctValue()
{
    	 return value;
}

void guiSlider::setValue( float val )
{
    value = (val-min) / max;
}


