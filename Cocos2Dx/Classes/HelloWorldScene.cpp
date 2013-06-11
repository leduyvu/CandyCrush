#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

#pragma Default

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

#pragma Touches

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
        CCSize layerSize = layer->getLayerSize();
        CCSize tileSize = layer->getMapTileSize();
        
        CCLog("Layer Size width%f height%f", layerSize.width, layerSize.height);
        CCLog("Tile size width%f height%f", tileSize.width, tileSize.height);
        HelloWorld::createFixture(layer);
//        CCSprite *tile0 = layer->tileAt(HelloWorld::tileCoorForPosition(convertedPoint));
//        unsigned int m_gid = layer->tileGIDAt(touchLoc);
//        CCLog("Tile ID: %i", m_gid);
        //CCLog("Tile location X:%f Y:%f ", tile0->getPositionX(), tile0->getPositionY());
//        tile0->runAction(CCRemoveSelf::create());
    }
    else CCLog("Invalid Position");
}

bool HelloWorld::touchPosValidation(CCPoint touchLoc) {
    if (touchLoc.x < 0 || touchLoc.y <0 || touchLoc.x >= map->getContentSize().width || touchLoc.y >= map->getContentSize().height) return  false;
    else return true;
}

#pragma Tile Functions

void HelloWorld::createFixture(CCTMXLayer* layer) {
    CCSize layerSize = layer->getLayerSize();
    for (int y = 0; y < layerSize.height; y++) {
        for (int x = 0; x < layerSize.width; x++) {
            CCSprite *tileSprite = layer->tileAt(ccp(x, y));
            CCLog("Tile position [x%i, y%i]x:%f ,y%f", x, y, tileSprite->getPositionX(), tileSprite->getPositionY());
            unsigned int m_gid = layer->tileGIDAt(ccp(x, y));
            CCLog("Tile position [x%i, y%i] has id:%i", x, y, m_gid);
            char stringID[10];
            sprintf(stringID, "%d", m_gid);
            CCLabelTTF *labelID = CCLabelTTF::create(stringID, "Arial", 20);
            labelID->setPosition(ccp(tileSprite->getPositionX() + tileSprite->getContentSize().width/2,
                                     tileSprite->getPositionY() + tileSprite->getContentSize().height/2));
            this->addChild(labelID, 2, 2);
        }
    }
}

CCPoint HelloWorld::tileCoorForPosition(CCPoint position) {
    int x = position.x / map->getTileSize().width;
    int y = ((map->getContentSize().height * map->getTileSize().height) - position.y) / map->getTileSize().height;
    return ccp(x, y);
}

CCPoint HelloWorld::positionForTileCoor(CCPoint tileCoor) {
    int x = (tileCoor.x * map->getTileSize().width) + map->getTileSize().width/2;
    int y = (map->getContentSize().height * map->getTileSize().height) - (tileCoor.y * map->getTileSize().height) - map->getTileSize().height/2;
    return ccp(x, y);
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


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
