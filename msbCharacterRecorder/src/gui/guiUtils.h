#ifndef GUIUTILS_H_INCLUDED
#define GUIUTILS_H_INCLUDED

#include "ofMain.h"

inline bool pointInRectangle( ofRectangle & rect, float x_, float y_ )
{
		if( x_ > rect.x && x_ < rect.x+rect.width && y_ > rect.y && y_ < rect.y + rect.height) return true;
		else return false;
};


#endif // GUIUTILS_H_INCLUDED
