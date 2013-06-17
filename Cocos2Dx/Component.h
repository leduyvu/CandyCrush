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
//#include "string.h"
USING_NS_CC;
class Component : public cocos2d::CCObject{
private:
    int location;
    int id ;
    std::string image;
public:
    Component(int id ,int location);
    void setLocation(int c);
    void setID(int id);
    void setImage(std::string str);
    int getLocation();
    int getID();
    std::string getImage();
 };
#endif
