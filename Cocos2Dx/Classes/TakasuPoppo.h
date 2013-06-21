#ifndef __TAKASUPOPPO_H__
#define __TAKASUPOPPO_H__

#include "cocos2d.h"
#include "CCObjectExtension.h"
USING_NS_CC;
class TakasuPoppo : public cocos2d::CCLayer {
private:
    CCPoint startSwipePoint;
    CCPoint movingSwipePoint;
    CCPoint endSwipePoint;

    CCSprite *buttonSprite;
    CCSprite *refreshButton;
    CCSprite *removeButton;
    
    CCTMXLayer *layer;
    CCTMXTiledMap *map;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCArray *colorArray;
    CCArray *toDestroyArray;
    CCArray *pickedArray;
    CCArray *debugTilesArray;
    
    bool spriteContained;
    bool swiping;
    bool swipeRecognized;
    
    bool swipeRight;
    bool swipeLeft;
    bool swipeUp;
    bool swipeDown;
    
    bool gridOn = false;
public:
    static cocos2d::CCScene* scene();
    
    virtual bool init();
    virtual void update(float dt);
    
    virtual void ccTouchesBegan(CCSet *touches, CCEvent *event);
    virtual void ccTouchesMoved(CCSet *touches, CCEvent *event);
    virtual void ccTouchesEnded(CCSet *touches, CCEvent *event);
    
    void addTileMap();
    void createFixture();
    
    bool touchPosValidation(CCPoint touchLoc);
    bool isTileMatched(int gid, int typeID);
    
    CCPoint tileCoorForPosition(CCPoint position);
    
    void setSwipeNavigation();
    
    void swapTilesCheck(CCObjectExtension *exObj, int swpGid);
    void swapTilesMoving(CCObjectExtension *exObj, CCObjectExtension *swpObj);
    void swapTilesReturn(CCObjectExtension *exObj, CCObjectExtension *swpObj);
    void swapColorID(CCObjectExtension *exObj, CCObjectExtension *swpObj);
    
    void swipedUp(CCObjectExtension *exObj);
    void swipedDown(CCObjectExtension *exObj);
    void swipedLeft(CCObjectExtension *exObj);
    void swipedRight(CCObjectExtension *exObj);
    
    void setupDebugButton();
    void switchGrid();
    void remove();
    void refresh();
    void checkTile();
    
    void menuCloseCallback(CCObject* pSender);
    CREATE_FUNC(TakasuPoppo);
};

#endif 
