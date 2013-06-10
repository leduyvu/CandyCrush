#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene() {
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init() {
    if (!CCLayer::init()) return false;
    HelloWorld::addTileMap();
    CCSprite *background = CCSprite::create("FramgiaLogo.png");
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(background, -1, -1);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    return true;
}

void HelloWorld::addTileMap () {
    map = CCTMXTiledMap::create("NewTile.tmx");
    this->addChild(map, 0, 0);
    CCArray *pChildrenArray = map->getChildren();
    CCObject *pObject = NULL;
    CCSpriteBatchNode *child = NULL;
    CCARRAY_FOREACH(pChildrenArray, pObject) {
        child = ((CCSpriteBatchNode *)pObject);
        if (!child) break;
        child->getTexture()->setAntiAliasTexParameters();
    }
    CCLog("Map width: %f length: %f; Map position X:%f Y:%f",map->getContentSize().width, map->getContentSize().height, map->getPositionX(), map->getPositionY());
}

bool HelloWorld::touchPosValidation(CCPoint touchLoc) {
    if (touchLoc.x < 0 || touchLoc.y <0 || touchLoc.x >= map->getContentSize().width || touchLoc.y >= map->getContentSize().height) return  false;
    else return true;
}

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    return true;
}

void HelloWorld::ccTouchMoved (CCTouch *touch, CCEvent *event) {
   
}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    CCPoint convertedPoint = CCDirector::sharedDirector()->convertToGL(touchLoc);
    
    if (HelloWorld::touchPosValidation(convertedPoint)) {
        CCTMXLayer *layer = map->layerNamed("Tile Layer 1");
        CCSprite *tile0 = layer->tileAt(ccp(1,5));
        unsigned int m_gid = layer->tileGIDAt(touchLoc);
        CCLog("Tile ID: %i", m_gid);
        //CCLog("Tile location X:%f Y:%f ", tile0->getPositionX(), tile0->getPositionY());
        tile0->runAction(CCRemoveSelf::create());
    }
    else CCLog("Invalid Position");
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
