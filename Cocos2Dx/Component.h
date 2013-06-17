//
//  Component.h
//  Cocos2Dx
//
//  Created by macbook_006 on 13/06/17.
//
//

#ifndef Cocos2Dx_Component_h
#define Cocos2Dx_Component_h
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class Component : public cocos2d::CCObject{
private:
    int location;
    int colorId;
public:
    Component(int colorId ,int location);
    void setLocation(int c);
    void setID(int colorId);
    void setImage(string str);
    int getLocation();
    int getID();
 };
#endif
