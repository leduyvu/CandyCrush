#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
class HelloWorld : public cocos2d::CCLayer {
private:
    CCPoint startSwipePoint;
    CCPoint movingSwipePoint;
    CCPoint endSwipePoint;
    
    CCSprite *tileSprite;
    CCSprite *pickedSprite;
    
    CCSprite *swipeRightSprite;
    CCSprite *swipeLeftSprite;
    CCSprite *swipeUpSprite;
    CCSprite *swipeDownSprite;
    
    CCTMXLayer *layer;
    
    CCArray *spriteArray;
    CCArray *tileArray;
    
    CCTMXTiledMap *map;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    bool spriteChosen;
    
    bool swiping;
    bool swipeRecognized;
    
    bool swipeRight;
    bool swipeLeft;
    bool swipeUp;
    bool swipeDown;
    
    bool swipeUpAllowed;
    bool swipeLeftAllowed;
    bool swipeDownAllowed;
    bool swipeRightAllowed;
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    void addTileMap();
    
    void menuCloseCallback(CCObject* pSender);
    void createFixture(CCTMXLayer *layer);
    void letThereBeACandy(CCTMXLayer *layer);
    
    bool touchPosValidation(CCPoint touchLoc);
    
    CCPoint tileCoorForPosition(CCPoint position);
    CCPoint positionForTileCoor(CCPoint tileCoor);
    
    void setSwipeNavigation();
    void removeSwipeNavigation();
    
    void swipedUp(CCSprite *sprite);
    void swipedDown(CCSprite *sprite);
    void swipedLeft(CCSprite *sprite);
    void swipedRight(CCSprite *sprite);
    
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    virtual void update(float dt);
    CREATE_FUNC(HelloWorld);
};

#endif 
