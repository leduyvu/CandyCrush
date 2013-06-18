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
    CCSprite *movingSprite;
    CCSprite *swapSprite;
    
    CCSprite *swipeRightSprite;
    CCSprite *swipeLeftSprite;
    CCSprite *swipeUpSprite;
    CCSprite *swipeDownSprite;
    
    CCTMXLayer *layer;
    
    CCArray *spriteArray;
    CCArray *tileArray;
    CCArray *colorArray;
    CCArray *toDestroyArray;
    
    CCTMXTiledMap *map;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    bool spriteChosen;
    bool spriteContained;
    
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
    
    bool isStartTileMatching;
    
    int tileDimentionArray[49];
    const char *plistPath = "TileData.plist";
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    void addTileMap();
    
    void menuCloseCallback(CCObject* pSender);
    void createFixture();
    void removeFixture();
    void letThereBeACandy(CCTMXLayer *layer);
    
    bool touchPosValidation(CCPoint touchLoc);
    bool isTileMatched(int tileID, int colorID);
    
    CCPoint tileCoorForPosition(CCPoint position);
    CCPoint positionForTileCoor(CCPoint tileCoor);
    
    void setSwipeNavigation();
    void removeSwipeNavigation();
    void checkTiles();
    
    void swapTilesMoving(CCSprite *sprite, CCSprite *swapSprite);
    void swapTilesReturn(CCSprite *sprite, CCSprite *swapSprite);
    void swapColorID(int spriteTag, int swapTag);
    
    void swipedUp(CCSprite *sprite);
    void swipedDown(CCSprite *sprite);
    void swipedLeft(CCSprite *sprite);
    void swipedRight(CCSprite *sprite);
    
    void propertiesCheck(CCDictionary *dict);
    
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    virtual void update(float dt);
    CREATE_FUNC(HelloWorld);
};

#endif 
