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

Component::Component(int id, int location)
{
    this->colorId=id;
    this->location=location;
}

void Component::setID(int id)
{
    this->colorId=id;
}
void Component::setLocation(int c)
{
    this->location = c;
}
int Component::getID()
{
    return this->colorId;
}
int Component::getLocation()
{
    return location;
}