#ifndef GUISLIDER_H
#define GUISLIDER_H

#include "guiUtils.h"

class guiSlider
{
    public:
        guiSlider();
        guiSlider(string sliderName, float defaultVal, float min_, float max_);
        virtual ~guiSlider();

        void    setup( string sliderName, float defaultVal, float min_, float max_, float x, float y, float w_, float h_ );
        void    setup( float x, float y, float w_, float h_ );
        bool    update(int x, int y);
        bool    hitTest( int x, int y );
        void    draw();
        void    drawAsMarker();

        int     getIntValue();
        float   getFloatValue();
        float   getPctValue();

        float     getX(){return (xpos+value*w);}
        float     getY(){return ypos; }

        void    setValue( float val );

        float xpos,ypos;
    protected:


        string name;
        float value;
        float min;
        float max;
        //float xpos,ypos;
        float w,h;

        ofRectangle hitArea;


    private:
};

#endif // GUISLIDER_H
