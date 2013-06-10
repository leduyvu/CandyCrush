#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
class HelloWorld : public cocos2d::CCLayer {
private:
    CCTMXTiledMap *map;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    void menuCloseCallback(CCObject* pSender);
    void addTileMap();
    bool touchPosValidation(CCPoint touchLoc);
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    CREATE_FUNC(HelloWorld);
};

#endif 
