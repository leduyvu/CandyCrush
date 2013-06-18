#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CCObjectExtension.h"

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
    srand(time(0));
    if (!CCLayer::init()) return false;
    
    spriteArray = new CCArray;
    tileArray = new CCArray;
    colorArray = new CCArray;
    toDestroyArray = new CCArray;
    
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
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    CCObject *object = NULL;
    CCARRAY_FOREACH(spriteArray, object) {
        CCSprite *objectSprite = dynamic_cast<CCSprite*>(object);
        CCRect spriteRect = objectSprite->boundingBox();
        if (spriteRect.containsPoint(touchLoc)) {
            CCLog("Object ID: %i", objectSprite->getTag());
            swipeRecognized = false;
            startSwipePoint = touchLoc;
            HelloWorld::setSwipeNavigation();
            CCLog("Start Swipe Point -x%f -y%f", startSwipePoint.x, startSwipePoint.y);
            pickedSprite = objectSprite;
            CCLog(("Picked Sprite position -x%f 0y%f"), pickedSprite->getPositionX(), pickedSprite->getPositionY());
            spriteContained = true;
        }
    }
    return true;
}

void HelloWorld::ccTouchMoved (CCTouch *touch, CCEvent *event) {
    if (spriteContained) {
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
        
        CCObject *colorObject;
        CCARRAY_FOREACH(colorArray, colorObject) {
            CCObjectExtension *cp = (CCObjectExtension*)(colorObject);
            if (cp->getLocation() == gid) {
                int colorID = cp->getID();
                if (HelloWorld::isTileMatched(gid+1, colorID)) {
                    CCLog("Combo exist at tile %i", gid+1);
                    CCSprite *toSwapSprite = (CCSprite*)this->getChildByTag(sprite->getTag() + 1);
                    HelloWorld::swapTilesMoving(sprite, toSwapSprite);
                    HelloWorld::swapColorID(gid, gid+1);
                }
                else {
                    CCLog("Combo doesn't exist");
                    CCSprite *toSwapSprite = (CCSprite*)this->getChildByTag(sprite->getTag() + 1);
                    HelloWorld::swapTilesReturn(sprite, toSwapSprite);
                }
            }
        }
    }
}

void HelloWorld::swipedLeft(CCSprite *sprite) {
    CCPoint spritePosition = sprite->getPosition();
    CCPoint translatePosition = HelloWorld::tileCoorForPosition(spritePosition);
    unsigned int gid = layer->tileGIDAt(translatePosition);
    if (gid != 1 && gid != 8 && gid != 15 && gid != 22 && gid != 29 && gid != 36 && gid != 43 ) {
        CCObject *colorObject;
        CCARRAY_FOREACH(colorArray, colorObject) {
            CCObjectExtension *cp = (CCObjectExtension*)(colorObject);
            if (cp->getLocation() == gid) {
                int colorID = cp->getID();
                if (HelloWorld::isTileMatched(gid-1, colorID)) {
                    CCLog("Combo exist at tile %i", gid-1);
                    CCSprite *toSwapSprite = (CCSprite*)this->getChildByTag(sprite->getTag() - 1);
                    HelloWorld::swapTilesMoving(sprite, toSwapSprite);
                    HelloWorld::swapColorID(gid, gid-1);
                }
                else {
                    CCLog("Combo doesn't exist");
                    CCSprite *toSwapSprite = (CCSprite*)this->getChildByTag(sprite->getTag() - 1);
                    HelloWorld::swapTilesReturn(sprite, toSwapSprite);
                }
            }
        }
    }
}

void HelloWorld::swipedUp(CCSprite *sprite) {
    CCPoint spritePosition = sprite->getPosition();
    CCPoint translatePosition = HelloWorld::tileCoorForPosition(spritePosition);
    unsigned int gid = layer->tileGIDAt(translatePosition);
    if (gid >= 7) {
        CCObject *colorObject;
        CCARRAY_FOREACH(colorArray, colorObject) {
            CCObjectExtension *cp = (CCObjectExtension*)(colorObject);
            if (cp->getLocation() == gid) {
                int colorID = cp->getID();
                if (HelloWorld::isTileMatched(gid-7, colorID)) {
                    CCLog("Combo exist at tile %i", gid-7);
                    CCSprite *toSwapSprite = (CCSprite*)this->getChildByTag(sprite->getTag() - 7);
                    HelloWorld::swapTilesMoving(sprite, toSwapSprite);
                    HelloWorld::swapColorID(gid, gid-7);
                }
                else {
                    CCLog("Combo doesn't exist");
                    CCSprite *toSwapSprite = (CCSprite*)this->getChildByTag(sprite->getTag() - 7);
                    HelloWorld::swapTilesReturn(sprite, toSwapSprite);
                }
            }
        }

    }
}

void HelloWorld::swipedDown(CCSprite *sprite) {
    CCPoint spritePosition = sprite->getPosition();
    CCPoint translatePosition = HelloWorld::tileCoorForPosition(spritePosition);
    unsigned int gid = layer->tileGIDAt(translatePosition);
    if (gid < 43) {
        CCObject *colorObject;
        CCARRAY_FOREACH(colorArray, colorObject) {
            CCObjectExtension *cp = (CCObjectExtension*)(colorObject);
            if (cp->getLocation() == gid) {
                int colorID = cp->getID();
                if (HelloWorld::isTileMatched(gid+7, colorID)) {
                    CCLog("Combo exist at tile %i", gid+7);
                    CCSprite *toSwapSprite = (CCSprite*)this->getChildByTag(sprite->getTag() + 7);
                    HelloWorld::swapTilesMoving(sprite, toSwapSprite);
                    HelloWorld::swapColorID(gid, gid+7);
                }
                else {
                    CCLog("Combo doesn't exist");
                    CCSprite *toSwapSprite = (CCSprite*)this->getChildByTag(sprite->getTag() + 7);
                    HelloWorld::swapTilesReturn(sprite, toSwapSprite);
                }
            }
        }

    }
}

#pragma mark Tile Interactions

void HelloWorld::swapTilesMoving(CCSprite *sprite, CCSprite *swapSprite) {
    sprite->runAction(CCMoveTo::create(0.3, swapSprite->getPosition()));
    swapSprite->runAction(CCMoveTo::create(0.3, sprite->getPosition()));
    
    int thisSpriteTag = sprite->getTag();
    sprite->setTag(thisSpriteTag+1);
    swapSprite->setTag(thisSpriteTag);
}

void HelloWorld::swapTilesReturn(CCSprite *sprite, CCSprite *swapSprite) {
    sprite->runAction(CCSequence::create(CCMoveTo::create(0.3, swapSprite->getPosition()),
                                         CCMoveTo::create(0.3, sprite->getPosition())));
    swapSprite->runAction(CCSequence::create(CCMoveTo::create(0.3, sprite->getPosition()),
                                             CCMoveTo::create(0.3, swapSprite->getPosition())));
}

#pragma mark Tile Functions

void HelloWorld::addTileMap () {
    map = CCTMXTiledMap::create("AnotherMap.tmx");
    this->addChild(map, -1, -1);
    layer = map->layerNamed("Grids");
    HelloWorld::createFixture();
    
    CCArray *pChildrenArray = map->getChildren();
    CCObject *pObject = NULL;
    CCSpriteBatchNode *child = NULL;
    CCARRAY_FOREACH(pChildrenArray, pObject) {
        child = ((CCSpriteBatchNode *)pObject);
        if (!child) break;
        child->getTexture()->setAntiAliasTexParameters();
    }
}

void HelloWorld::createFixture() {
    CCSize layerSize = layer->getLayerSize();
    
    for (int y = 0; y < layerSize.height; y++) {
        for (int x = 0; x < layerSize.width; x++) {
            tileSprite = layer->tileAt(ccp(x, y));
            unsigned int m_gid = layer->tileGIDAt(ccp(x, y));
            tileSprite->setTag(x);
            tileArray->addObject(tileSprite);
            
            char stringID[10];
            sprintf(stringID, "%d", m_gid);
            CCLabelTTF *labelID = CCLabelTTF::create(stringID, "Arial", 20);
            
            labelID->setPosition(ccp(tileSprite->getPositionX() + tileSprite->getContentSize().width/2,
                                     tileSprite->getPositionY() + tileSprite->getContentSize().height/2));
            this->addChild(labelID, 0, 99);
            
            char tileName[7][20];
            strcpy(tileName[0], "Candy");
            strcpy(tileName[1], "Candy2");
            strcpy(tileName[2], "Candy3");
            strcpy(tileName[3], "Candy4");
            strcpy(tileName[4], "Candy5");
            strcpy(tileName[5], "Candy6");
            strcpy(tileName[6], "Candy7");
            int randomTile = rand() % 7;
            char spriteName[100];
            
            while (HelloWorld::isTileMatched(m_gid, randomTile)) {
                randomTile = rand() % 7;
            }
            
            sprintf(spriteName, "%s.png", tileName[randomTile]);
            CCSprite *randomTileSprite = CCSprite::create(spriteName);
            randomTileSprite->setPosition(ccp(tileSprite->getPositionX() + tileSprite->getContentSize().width/2,
                                              tileSprite->getPositionY() + tileSprite->getContentSize().height/2));
            
            CCObjectExtension *cp =new CCObjectExtension(randomTile, m_gid);
            colorArray->addObject(cp);
            
            int mgidTag = (300 + m_gid);
            this->addChild(randomTileSprite, 3, mgidTag);
            
            spriteArray->addObject(randomTileSprite);
        }
    }
}

void HelloWorld::letThereBeACandy(CCTMXLayer *layer) {
    CCSprite *candy = CCSprite::create("Candy.png");
    this->addChild(candy, 1, 101);
    
    CCSprite *candy2 = CCSprite::create("Candy.png");
    this->addChild(candy2, 1, 102);
    
    CCSprite *tempSprite = CCSprite::create();
    tempSprite = layer->tileAt(ccp(3, 3));
    CCLog("Temp Sprite tag %i", tempSprite->getTag());

    CCSprite *tempSprite2 = CCSprite::create();
    tempSprite2 = layer->tileAt(ccp(5, 5));
    CCLog("Temp Sprite tag %i", tempSprite2->getTag());
    
    candy->setPosition(ccp(tempSprite->getPositionX() + (tempSprite->getContentSize().width /2),
                           tempSprite->getPositionY() + (tempSprite->getContentSize().height /2)));
    
    candy2->setPosition(ccp(tempSprite2->getPositionX() + (tempSprite2->getContentSize().width /2),
                            tempSprite2->getPositionY() + (tempSprite2->getContentSize().height /2)));
    spriteArray->addObject(candy2);
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

void HelloWorld::swapColorID(int baseTileID, int swapTileID) {
    int baseColor;
    int swapColor;
    
    CCObject *colorObject;
    CCARRAY_FOREACH(colorArray, colorObject) {
        CCObjectExtension *ex = dynamic_cast<CCObjectExtension*>(colorObject);
        if (ex->getLocation() == baseTileID) {
            baseColor = ex->getID();
            
            CCObject *swapObject;
            CCARRAY_FOREACH(colorArray, swapObject) {
                CCObjectExtension *expSwap = dynamic_cast<CCObjectExtension*>(swapObject);
                if (expSwap->getLocation() == swapTileID) {
                    swapColor = expSwap->getID();
                    ex->setID(swapColor);
                    expSwap->setID(baseColor);
                    CCLog("Tile %i's color %i got swaped with tile %i's color %i.", baseTileID, baseColor + 1, swapTileID, swapColor + 1);
                }
            }
        }
    }
}

void HelloWorld::checkTiles() {
    
}

bool HelloWorld::isTileMatched(int tileID, int colorID) {
    int leftTile = tileID -1, rightTile = tileID +1, topTile = tileID - 7, bottomTile = tileID +7;
    CCObject *tileObject;
    
    CCARRAY_FOREACH(colorArray, tileObject) {
        CCObjectExtension *cp = (CCObjectExtension*)(tileObject);
        if (tileID != 1 && tileID != 2 && tileID != 8 && tileID != 9 && tileID != 15 && tileID != 16 && tileID != 22 && tileID != 23 &&
            tileID != 29 && tileID != 30 && tileID != 36 && tileID != 37 && tileID != 43 &&tileID != 44 ) {
            if (cp->getLocation() == leftTile && cp->getID() == colorID) {
                int thisLeftTile = cp->getLocation() -1;
                CCObject *thisObject;
                CCARRAY_FOREACH(colorArray, thisObject) {
                    CCObjectExtension *thisCp = (CCObjectExtension*)(thisObject);
                    if (thisCp->getLocation() == thisLeftTile && thisCp->getID() == colorID) {
                        CCLog("Left combo possiblity detected with tiles: %i, %i and %i.", tileID, thisLeftTile + 1, thisLeftTile);
                        return true; continue;
                    }
                }
            }
        }
        if (tileID != 6 && tileID != 7 && tileID != 13 && tileID != 14 && tileID != 20 && tileID != 21 && tileID != 27 && tileID != 28 &&
            tileID != 34 && tileID != 35 && tileID != 41 && tileID != 42 && tileID != 48 && tileID != 49 ) {
            if (cp->getLocation() == rightTile && cp->getID() == colorID) {
                int thisRightTile = cp->getLocation() +1;
                CCObject *thisObject;
                CCARRAY_FOREACH(colorArray, thisObject) {
                    CCObjectExtension *thisCp = (CCObjectExtension*)(thisObject);
                    if (thisCp->getLocation() == thisRightTile && thisCp->getID() == colorID) {
                        CCLog("Right combo possiblity detected with tiles: %i, %i and %i.", tileID, thisRightTile + 1, thisRightTile);
                        return true; continue;
                    }
                }
            }
        }
        if (tileID >= 15) {
            if (cp->getLocation() == topTile && cp->getID() == colorID) {
                int thisTopTile = cp->getLocation() - 7;
                CCObject *thisObject;
                CCARRAY_FOREACH(colorArray, thisObject) {
                    CCObjectExtension *thisCp = (CCObjectExtension*)(thisObject);
                    if (thisCp->getLocation() == thisTopTile && thisCp->getID() == colorID) {
                        CCLog("Top combo possiblity detected with tiles: %i, %i and %i.", tileID, thisTopTile + 7, thisTopTile);
                        return true; continue;
                    }
                }
            }
        }
        if (tileID <= 35) {
            if (cp->getLocation() == bottomTile && cp->getID() == colorID) {
                int thisBottomTile = cp->getLocation() + 7;
                CCObject *thisObject;
                CCARRAY_FOREACH(colorArray, thisObject) {
                    CCObjectExtension *thisCp = (CCObjectExtension*)(thisObject);
                    if (thisCp->getLocation() == thisBottomTile && thisCp->getID() == colorID) {
                        CCLog("Bottom combo possiblity detected with tiles: %i, %i and %i.", tileID, thisBottomTile - 7, thisBottomTile);
                        return true; continue;
                    }
                }
            }
        }
    }
    return false;
}

#pragma mark Swipe Navigation

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

#pragma mark Debug

void HelloWorld::propertiesCheck(CCDictionary *singleTile) {

}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
