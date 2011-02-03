#include "content.h"
#include "input.h"
#include "assignButton.h"

void Content::setup(){

    renderer=Renderer::getInstance();
    input=Input::getInstance();



    BasicButton *but;

    but= new AssignButton;
    but->location.x=100;
    but->location.y=100;
    but->name="hello_world";
    but->tooltip="hello_world";
    but->setLocation(but->location);
    but->textureID="icon_select";
    but->color=COLOR_WHITE;
    but->setup();
    but->parent=NULL;
    renderer->buttonList.push_back(but);



    Actor* myActor = new Actor;
    myActor->setLocation(Vector3f(0,0,-15));
    myActor->postLoad();
    myActor->color=Vector4f(1,1,1,1);
    renderer->actorList.push_back(myActor);
    renderer->layerList[0]->actorList.push_back(myActor);


 }


void Content::reset(){

renderer->actorList.clear();
renderer->buttonList.clear();
setup();
}
