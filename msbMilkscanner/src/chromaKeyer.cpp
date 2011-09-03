#include "chromaKeyer.h"

ChromaKeyer::ChromaKeyer()
{

    tH  = 10;
    tS  = 10;
    tV  = 10;

    hsvImage.allocate(1,1);
    hueImg.allocate(1,1);
    satImg.allocate(1,1);
    valImg.allocate(1,1);

    rgbBox.allocate(10,10);
    rgbBox.set(0,255,0);

    setFromRGB(80,200,0);

    R=80;
    G=200;
    B=0;

}

ChromaKeyer::~ChromaKeyer()
{
    //dtor
}

void ChromaKeyer::setHSV(int h, int s, int v)
{
    H = h;
    S = s;
    V = v;

    rgbBox.set(h,s,v);
    rgbBox.convertHsvToRgb();

    unsigned char * pixels = rgbBox.getPixels();
    R = pixels[0];
    G = pixels[1];
    B = pixels[2];
}

void ChromaKeyer::setFromRGB(int r, int g, int b)
{
    R = r;
    G = g;
    B = b;

    rgbBox.set(r,g,b);
    rgbBox.convertRgbToHsv();

    unsigned char * pixels = rgbBox.getPixels();
    H = pixels[0];
    S = pixels[1];
    V = pixels[2];

}

void ChromaKeyer::keyImage( ofxCvColorImage & src, ofxCvColorImage & dst, int w, int h )
{

    // resize images if not at same size already
    if( hsvImage.width != w || hsvImage.height != h)
    {
            hsvImage.allocate(w,h);
            hueImg.allocate(w,h);
            satImg.allocate(w,h);
            valImg.allocate(w,h);
    }

    // convert src to hsv color space
    hsvImage.setFromPixels(src.getPixels(),w,h);
    hsvImage.convertRgbToHsv();

    // extract the hsv channels to a grayscale image
    hsvImage.convertToGrayscalePlanarImages(hueImg,satImg,valImg);

    unsigned char * pixelsHue = hsvImage.getPixels();
    //unsigned char * pixelsSat = satImg.getPixels();
    unsigned char * dstMask = new unsigned char[w*h];


    // loop through and compare
    /*
    if( pixelsHue[i] >= H-tH && pixelsHue[i] <= H+tH&&
            pixelsSat[i] >= S-tS && pixelsSat[i] <= S+tS
        ){
    */
    for( int i = 0; i < w*h; i++)
    {
        if( pixelsHue[i*3] >= H-tH && pixelsHue[i*3] <= H+tH&&
            pixelsHue[i*3+1] >= S-tS && pixelsHue[i*3+1] <= S+tS
        ){
            dstMask[i] = 0;


        }else{
            dstMask[i] = 255;

        }
    }

    hueImg.setFromPixels(dstMask,w,h);

    cvCopy( hsvImage.getCvImage(),dst.getCvImage(),hueImg.getCvImage());//,hueImg.getCvImage());
    dst.flagImageChanged();
    dst.convertHsvToRgb();

    delete dstMask;
}


void ChromaKeyer::keyPixel(){

}

void ChromaKeyer::drawColor( int x, int y, int size )
{
    ofSetColor(R,G,B,255);
    ofFill();
    ofRect(x,y,size,size);
}

void ChromaKeyer::getColorFromScreen( int x, int y, int w, ofxCvColorImage & src)
{
    // tried many different things here but it doesnt work... need to fix it
    cout << " x " << x << " y " << y << endl;
    if( x > src.width-1 || y > src.height-1 || x < 1 || y < 1 ) return;

    //ofxCvColorImage temp;
    //temp.allocate(src.width,src.height);
    //temp = src;


    unsigned char * pixels = src.getPixels();


    int pix = x * w + y;
    int r = pixels[ pix ];
    int g = pixels[ pix + 1];
    int b = pixels[ pix + 2];

    setFromRGB( r,  g,  b);
}
