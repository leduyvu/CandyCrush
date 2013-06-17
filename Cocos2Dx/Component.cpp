//
//  Component.cpp
//  Cocos2Dx
//
//  Created by macbook_006 on 13/06/17.
//
//

#include "Component.h"
using namespace cocos2d;
using namespace std;
USING_NS_CC;
#pragma mark Default
Component::Component(int id, int location)
{
    this->id=id;
    this->location=location;
    //this->image=image;
}

void Component::setID(int id)
{
    this->id=id;
}
void Component::setLocation(int c)
{
    this->location = c;
}
void Component::setImage(string str)
{
    image = str;
}
int Component::getID()
{
    return this->id;
}
int Component::getLocation()
{
    return location;
}
string Component::getImage()
{
    return image;
}