#ifndef Cocos2Dx_CCObjectExtension_h
#define Cocos2Dx_CCObjectExtension_h
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class CCObjectExtension : public CCObject{
private:
    int gid;
    int colorId;
    CCSprite *blockSprite;
    CCPoint position;
    CCPoint coordination;
public:
    CCObjectExtension(int colorId ,int location, CCSprite *sprite, CCPoint position, CCPoint coordination);
    
    void setGid(int c);
    int getGid();
    
    int getID();
    void setID(int colorId);

    CCSprite *getSprite();
    void setSprite(CCSprite *sprite);
    
    CCPoint getPosition();
    void setPosition(CCPoint position);

    CCPoint getCoordination();
    void setCoordination(CCPoint position);

 };
#endif
