#ifndef CHROMAKEYER_H
#define CHROMAKEYER_H

#include "ofxOpenCv.h"

class ChromaKeyer
{
    public:
        ChromaKeyer();
        virtual ~ChromaKeyer();

        void setHSV(int h, int s, int v);
        void setFromRGB(int r, int g, int b);
        void setHThreshold( int value ){ tH = value; }
        void setSThreshold( int value ){ tS = value; }
        void setVThreshold( int value ){ tV = value; }
        void getColorFromScreen( int x, int y, int w, ofxCvColorImage & src);

        void keyImage(ofxCvColorImage & src, ofxCvColorImage & dst, int w, int h);

        void keyPixel();

        void drawColor(int x, int y, int size);

        int R,G,B;
        int tH,tS,tV;                                      // RGB value of chomra keying color
        ofxCvGrayscaleImage hueImg, satImg, valImg;         // images for extracting hsv channels

        int H,S,V;                                      // HSV values of color
                                                        // Threshold for comparison to each value

        ofxCvColorImage hsvImage;                       // hsv converted image
        ofxCvColorImage rgbBox;                         //


};

#endif // CHROMAKEYER_H
