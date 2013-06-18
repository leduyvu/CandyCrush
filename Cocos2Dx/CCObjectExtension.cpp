//
//  Component.cpp
//  Cocos2Dx
//
//  Created by macbook_006 on 13/06/17.
//
//

#include "CCObjectExtension.h"

using namespace cocos2d;
using namespace std;
USING_NS_CC;

CCObjectExtension::CCObjectExtension(int id, int location)
{
    this->colorId=id;
    this->location=location;
}

void CCObjectExtension::setID(int id)
{
    this->colorId=id;
}
void CCObjectExtension::setLocation(int c)
{
    this->location = c;
}
int CCObjectExtension::getID()
{
    return this->colorId;
}
int CCObjectExtension::getLocation()
{
    return location;
}