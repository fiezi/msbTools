#ifndef H_CONTENT
#define H_CONTENT

#include "basicButton.h"

class Renderer;

class Content
{
public:

    Renderer* renderer;
    Input* input;

    virtual void setup();
    virtual void reset();

};
#endif
