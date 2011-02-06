#include "content.h"
#include "input.h"
#include "assignButton.h"
#include "msbLight.h"

void Content::setup(){

    renderer=Renderer::getInstance();
    input=Input::getInstance();

    //stuff can go here, but can also totally go in testApp!

 }


void Content::reset(){

renderer->actorList.clear();
renderer->buttonList.clear();
setup();
}
