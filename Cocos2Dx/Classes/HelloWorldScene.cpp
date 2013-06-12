#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

#pragma mark Default

CCScene* HelloWorld::scene() {
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init() {
    if (!CCLayer::init()) return false;
    spriteArray = new CCArray;
    tileArray = new CCArray;
    HelloWorld::addTileMap();
    CCSprite *background = CCSprite::create("PuzzleBackgroud.png");
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    //this->addChild(background, -2, -2);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt) {
    if (swipeRight) {
        HelloWorld::swipedRight(pickedSprite);
        swipeRight = false;
    }
    if (swipeLeft) {
        HelloWorld::swipedLeft(pickedSprite);
        swipeLeft = false;
    }
    if (swipeUp) {
        HelloWorld::swipedUp(pickedSprite);
        swipeUp = false;
    }
    if (swipeDown) {
        HelloWorld::swipedDown(pickedSprite);
        swipeDown = false;
    }
}

#pragma mark Touches

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    CCObject *object;
    CCARRAY_FOREACH(spriteArray, object) {
        CCSprite *objectSprite = dynamic_cast<CCSprite*>(object);
        CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
        CCRect touchRect = CCRect(touchLoc.x, touchLoc.y, 1, 1);
        CCRect spriteRect = objectSprite->boundingBox();
        if (touchRect.intersectsRect(spriteRect)) {
            swipeRecognized = false;
            startSwipePoint = touchLoc;
            HelloWorld::setSwipeNavigation();
            CCLog("Start Swipe Point -x%f -y%f", startSwipePoint.x, startSwipePoint.y);
            pickedSprite = objectSprite;
            CCLog(("x%f y%f"), pickedSprite->getPositionX(), pickedSprite->getPositionY());
            return true;
        }
        else return false;
    }
    return false;
}

void HelloWorld::ccTouchMoved (CCTouch *touch, CCEvent *event) {
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    movingSwipePoint = touchLoc;
    
    if (!swipeRecognized) swiping = true;
    else swiping = false;
    
    if (swiping == true) {
        CCRect touchRect = CCRect(touchLoc.x, touchLoc.y, 1, 1);
        
        CCRect swipeRightRect = swipeRightSprite->boundingBox();
        CCRect swipeLeftRect = swipeLeftSprite->boundingBox();
        CCRect swipeUpRect = swipeUpSprite->boundingBox();
        CCRect swipeDownRect = swipeDownSprite->boundingBox();

        if ((movingSwipePoint.x - startSwipePoint.x > 20) && touchRect.intersectsRect(swipeRightRect)) {
            swipeRecognized = true;
            swipeRight = true;
            HelloWorld::removeSwipeNavigation();
        }
        else if ((startSwipePoint.x - movingSwipePoint.x > 20) && touchRect.intersectsRect(swipeLeftRect)) {
            swipeRecognized = true;
            swipeLeft = true;
            HelloWorld::removeSwipeNavigation();
        }
        else if ((movingSwipePoint.y - startSwipePoint.y > 20) && touchRect.intersectsRect(swipeUpRect)) {
            swipeRecognized = true;
            swipeUp = true;
            HelloWorld::removeSwipeNavigation();
        }
        else if ((startSwipePoint.y - movingSwipePoint.y > 20) && touchRect.intersectsRect(swipeDownRect)) {
            swipeRecognized = true;
            swipeDown = true;
            HelloWorld::removeSwipeNavigation();
        }        
        else if (!touchRect.intersectsRect(swipeRightRect) && !touchRect.intersectsRect(swipeLeftRect)
                 && !touchRect.intersectsRect(swipeUpRect) && !touchRect.intersectsRect(swipeDownRect)) swipeRecognized = true;
        
    }
}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    HelloWorld::removeSwipeNavigation();
    swiping = false;
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    if (HelloWorld::touchPosValidation(touchLoc)) {
        
        CCTMXLayer *layer = map->layerNamed("Grids");
        CCSize layerSize = layer->getLayerSize();
        CCSize tileSize = layer->getMapTileSize();
        CCPoint transPoint = HelloWorld::tileCoorForPosition(touchLoc);
//        CCLog("Layer Size width%f height%f", layerSize.width, layerSize.height);
//        CCLog("Tile size width%f height%f", tileSize.width, tileSize.height);
//        CCLog("Position x:%f y:%f", touchLoc.x, touchLoc.y);
//        CCLog("Position to Tile Coor x:%f y:%f", transPoint.x, transPoint.y);
        unsigned int m_gid = layer->tileGIDAt(transPoint);
        CCLog("Tile ID at position : %i", m_gid);

    }
    else CCLog("Invalid Position");
}

bool HelloWorld::touchPosValidation(CCPoint touchLoc) {
    if (touchLoc.x < 0 ||
        touchLoc.y < 0 ||
        touchLoc.x >= map->getContentSize().width ||
        touchLoc.y >= map->getContentSize().height) return  false;
    else return true;
}

#pragma mark Swipe Actions

void HelloWorld::swipedRight(CCSprite *sprite) {
    CCPoint spritePosition = sprite->getPosition();
    CCPoint translatePosition = HelloWorld::tileCoorForPosition(spritePosition);
    unsigned int gid = layer->tileGIDAt(translatePosition);
    if (gid != 7 && gid != 14 && gid != 21 && gid != 28 && gid != 35 && gid != 42 && gid != 49 ) {
        CCPoint destination = ccp(spritePosition.x + map->getTileSize().width, spritePosition.y);
        sprite->runAction(CCMoveTo::create(0.3, destination));
    }
}

void HelloWorld::swipedLeft(CCSprite *sprite) {
    CCPoint spritePosition = sprite->getPosition();
    CCPoint translatePosition = HelloWorld::tileCoorForPosition(spritePosition);
    unsigned int gid = layer->tileGIDAt(translatePosition);
    CCLog("id %i", gid);
    if (gid != 1 && gid != 8 && gid != 15 && gid != 22 && gid != 29 && gid != 36 && gid != 43 ) {
        CCPoint destination = ccp(spritePosition.x - map->getTileSize().width, spritePosition.y);
        sprite->runAction(CCMoveTo::create(0.3, destination));
    }
}

void HelloWorld::swipedUp(CCSprite *sprite) {
    CCPoint spritePosition = sprite->getPosition();
    CCPoint translatePosition = HelloWorld::tileCoorForPosition(spritePosition);
    unsigned int gid = layer->tileGIDAt(translatePosition);
    if (gid >= 7) {
        CCPoint destination = ccp(spritePosition.x, spritePosition.y + map->getTileSize().height);
        sprite->runAction(CCMoveTo::create(0.3, destination));
    }
}

void HelloWorld::swipedDown(CCSprite *sprite) {
    CCPoint spritePosition = sprite->getPosition();
    CCPoint translatePosition = HelloWorld::tileCoorForPosition(spritePosition);
    unsigned int gid = layer->tileGIDAt(translatePosition);
    if (gid < 43) {
        CCPoint destination = ccp(spritePosition.x, spritePosition.y - map->getTileSize().height);
        sprite->runAction(CCMoveTo::create(0.3, destination));
    }
}

#pragma mark Tile Functions

void HelloWorld::addTileMap () {
    map = CCTMXTiledMap::create("AnotherMap.tmx");
    this->addChild(map, -1, -1);
    layer = map->layerNamed("Grids");
    HelloWorld::createFixture(layer);
    
    CCArray *pChildrenArray = map->getChildren();
    CCObject *pObject = NULL;
    CCSpriteBatchNode *child = NULL;
    CCARRAY_FOREACH(pChildrenArray, pObject) {
        child = ((CCSpriteBatchNode *)pObject);
        if (!child) break;
        child->getTexture()->setAntiAliasTexParameters();
    }
    CCLog("Map width: %f length: %f; Map position X:%f Y:%f",map->getContentSize().width, map->getContentSize().height, map->getPositionX(), map->getPositionY());
    HelloWorld::letThereBeACandy(layer);
}

void HelloWorld::createFixture(CCTMXLayer* layer) {
    CCSize layerSize = layer->getLayerSize();
    for (int y = 0; y < layerSize.height; y++) {
        for (int x = 0; x < layerSize.width; x++) {
            tileSprite = layer->tileAt(ccp(x, y));
            
            CCLog("Tile Sprite tag %i", tileSprite->getTag());
            CCLog("Tile position [x%i, y%i]x:%f ,y%f", x, y, tileSprite->getPositionX(), tileSprite->getPositionY());
            unsigned int m_gid = layer->tileGIDAt(ccp(x, y));
            tileSprite->setTag(x);
            tileArray->addObject(tileSprite);
            CCLog("Tile position [x%i, y%i] has id:%i", x, y, m_gid);
            char stringID[10];
            sprintf(stringID, "%d", m_gid);
            CCLabelTTF *labelID = CCLabelTTF::create(stringID, "Arial", 20);
            labelID->setPosition(ccp(tileSprite->getPositionX() + tileSprite->getContentSize().width/2,
                                     tileSprite->getPositionY() + tileSprite->getContentSize().height/2));
            this->addChild(labelID, 0, 99);
        }
    }
}

void HelloWorld::letThereBeACandy(CCTMXLayer *layer) {
    CCSprite *candy = CCSprite::create("Candy.png");
    this->addChild(candy, 1, 101);
    int randomPos = rand() % 7 + 1;

    CCSprite *tempSprite = CCSprite::create();
    tempSprite = layer->tileAt(ccp(randomPos, randomPos));
    CCLog("Temp Sprite tag %i", tempSprite->getTag());

    candy->setPosition(ccp(tempSprite->getPositionX() + (tempSprite->getContentSize().width /2),
                           tempSprite->getPositionY() + (tempSprite->getContentSize().height /2)));
    spriteArray->addObject(candy);
}

CCPoint HelloWorld::tileCoorForPosition(CCPoint position) {
    float x = position.x / map->getTileSize().width;
    float y = ((winSize.height - position.y)/(map->getTileSize().height)) -1;
    CCPoint transPos = ccp(floor(x), floor(y));
    return transPos;
}

CCPoint HelloWorld::positionForTileCoor(CCPoint tileCoor) {
    int x = (tileCoor.x * map->getTileSize().width) + map->getTileSize().width/2;
    int y = (map->getContentSize().height * map->getTileSize().height) - (tileCoor.y * map->getTileSize().height) - map->getTileSize().height/2;
    return ccp(x, y);
}

void HelloWorld::setSwipeNavigation() {
    swipeRightSprite = CCSprite::create("SwipeLeftRight.png");
    swipeRightSprite->setPosition(ccp((startSwipePoint.x + swipeRightSprite->getContentSize().width / 2), startSwipePoint.y));
    swipeRightSprite->setOpacity(0);
    swipeRightSprite->setTag(200);
    this->addChild(swipeRightSprite);
    
    swipeLeftSprite = CCSprite::create("SwipeLeftRight.png");
    swipeLeftSprite->setPosition(ccp((startSwipePoint.x - swipeLeftSprite->getContentSize().width / 2), startSwipePoint.y));
    swipeLeftSprite->setOpacity(0);
    swipeLeftSprite->setTag(201);
    this->addChild(swipeLeftSprite);
    
    swipeUpSprite = CCSprite::create("SwipeUpDown.png");
    swipeUpSprite->setPosition(ccp(startSwipePoint.x, startSwipePoint.y + (swipeUpSprite->getContentSize().height / 2)));
    swipeUpSprite->setOpacity(0);
    swipeUpSprite->setTag(202);
    this->addChild(swipeUpSprite);
    
    swipeDownSprite = CCSprite::create("SwipeUpDown.png");
    swipeDownSprite->setPosition(ccp(startSwipePoint.x, startSwipePoint.y - (swipeUpSprite->getContentSize().height / 2)));
    swipeDownSprite->setOpacity(0);
    swipeDownSprite->setTag(203);
    this->addChild(swipeDownSprite);

}

void HelloWorld::removeSwipeNavigation() {
    if (this->getChildByTag(200))this->removeChildByTag(200);
    if (this->getChildByTag(201))this->removeChildByTag(201);
    if (this->getChildByTag(202))this->removeChildByTag(202);
    if (this->getChildByTag(203))this->removeChildByTag(203);
}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
