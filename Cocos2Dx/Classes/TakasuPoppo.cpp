#include "TakasuPoppo.h"
#include "SimpleAudioEngine.h"
#include "CCObjectExtension.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

#pragma mark Default

CCScene* TakasuPoppo::scene() {
    CCScene *scene = CCScene::create();
    TakasuPoppo *layer = TakasuPoppo::create();
    scene->addChild(layer);
    return scene;
}

bool TakasuPoppo::init() {
    srand(time(0));
    if (!CCLayer::init()) return false;
    
    colorArray = new CCArray;
    toDestroyArray = new CCArray;
    pickedArray = new CCArray;
    
    debugTilesArray = new CCArray;
    
    TakasuPoppo::addBlocksToArray();
    TakasuPoppo::addTileMap();
    
    CCSprite *background = CCSprite::create("PuzzleBackgroud3.png");
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(background, -2, -2);
    
    TakasuPoppo::setupDebugButton();
    TakasuPoppo::checkTile();
    this->setTouchEnabled(true);
    this->scheduleUpdate();
    return true;
}

void TakasuPoppo::update(float dt) {
    if (swipeRight) {
        CCObject *object = NULL;
        CCARRAY_FOREACH(pickedArray, object) {
            CCObjectExtension *exObject = dynamic_cast<CCObjectExtension*>(object);
            TakasuPoppo::swipedRight(exObject);
            swipeRight = false;
            pickedArray->removeObject(object);
        }      
    }
    if (swipeLeft) {
        CCObject *object = NULL;
        CCARRAY_FOREACH(pickedArray, object) {
            CCObjectExtension *exObject = dynamic_cast<CCObjectExtension*>(object);
            TakasuPoppo::swipedLeft(exObject);
            swipeLeft = false;
            pickedArray->removeObject(object);
        }
    }
    if (swipeUp) {
        CCObject *object = NULL;
        CCARRAY_FOREACH(pickedArray, object) {
            CCObjectExtension *exObject = dynamic_cast<CCObjectExtension*>(object);
            TakasuPoppo::swipedUp(exObject);
            swipeUp = false;
            pickedArray->removeObject(object);
        }

    }
    if (swipeDown) {
        CCObject *object = NULL;
        CCARRAY_FOREACH(pickedArray, object) {
            CCObjectExtension *exObject = dynamic_cast<CCObjectExtension*>(object);
            TakasuPoppo::swipedDown(exObject);
            swipeDown = false;
            pickedArray->removeObject(object);
        }
    }
}

#pragma mark Touches

void TakasuPoppo::ccTouchesBegan(CCSet *touches, CCEvent *event) {
    CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    
    if (TakasuPoppo::touchPosValidation(touchLoc)) {
        unsigned int gid = layer->tileGIDAt(TakasuPoppo::tileCoorForPosition(touchLoc));
        CCObject *object = colorArray->objectAtIndex(gid - 1);
        CCObjectExtension *exObject = dynamic_cast<CCObjectExtension*>(object);
        if (exObject->getSprite()) {
            swipeRecognized = false;
            startSwipePoint = touchLoc;
            pickedArray->addObject(exObject);
            spriteContained = true;
        }
    }
}

void TakasuPoppo::ccTouchesMoved (CCSet *touches, CCEvent *event) {
    CCTouch *touch = (CCTouch*)touches->anyObject();
    if (spriteContained) {
        CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
        movingSwipePoint = touchLoc;
        
        if (!swipeRecognized) swiping = true;
        else swiping = false;
        
        if (swiping == true) {
            CCRect touchRect = CCRect(touchLoc.x, touchLoc.y, 70, 70);
            
            CCRect swipeRightRect = CCRectMake((startSwipePoint.x + 40), startSwipePoint.y, 80, 20);
            CCRect swipeLeftRect = CCRectMake((startSwipePoint.x - 40), startSwipePoint.y, 80, 20);
            CCRect swipeUpRect = CCRectMake(startSwipePoint.x, startSwipePoint.y + (40), 20, 80);
            CCRect swipeDownRect = CCRectMake(startSwipePoint.x, startSwipePoint.y - (40), 20, 80);
            
            if ((movingSwipePoint.x - startSwipePoint.x > 10) && touchRect.intersectsRect(swipeRightRect)) {
                swipeRecognized = true;
                swipeRight = true;
            }
            else if ((startSwipePoint.x - movingSwipePoint.x > 10) && touchRect.intersectsRect(swipeLeftRect)) {
                swipeRecognized = true;
                swipeLeft = true;
            }
            else if ((movingSwipePoint.y - startSwipePoint.y > 10) && touchRect.intersectsRect(swipeUpRect)) {
                swipeRecognized = true;
                swipeUp = true;
            }
            else if ((startSwipePoint.y - movingSwipePoint.y > 10) && touchRect.intersectsRect(swipeDownRect)) {
                swipeRecognized = true;
                swipeDown = true;
            }
            else if (!touchRect.intersectsRect(swipeRightRect) && !touchRect.intersectsRect(swipeLeftRect)
                     && !touchRect.intersectsRect(swipeUpRect) && !touchRect.intersectsRect(swipeDownRect)) swipeRecognized = true;
            
        }
    }
}

void TakasuPoppo::ccTouchesEnded(CCSet *touches, CCEvent *event) {
    CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    swiping = false;
    
    if (TakasuPoppo::touchPosValidation(touchLoc)) {
        CCTMXLayer *layer = map->layerNamed("Grids");
        CCSize layerSize = layer->getLayerSize();
        CCSize tileSize = layer->getMapTileSize();
        CCPoint transPoint = TakasuPoppo::tileCoorForPosition(touchLoc);
        unsigned int m_gid = layer->tileGIDAt(transPoint);
        CCLog("Tile ID at position : %i", m_gid);
        
    }
    pickedArray->removeAllObjects();
    CCRect buttonRect = buttonSprite->boundingBox();
    CCRect clearRect = removeButton->boundingBox();
    CCRect refreshRect = refreshButton->boundingBox();
    if (buttonRect.containsPoint(touchLoc) && !gridOn)TakasuPoppo::switchGrid();
    if (clearRect.containsPoint(touchLoc) && gridOn)TakasuPoppo::remove();
    if (refreshRect.containsPoint(touchLoc) && gridOn)TakasuPoppo::refresh();
}

bool TakasuPoppo::touchPosValidation(CCPoint touchLoc) {
    if (touchLoc.x < 0 ||
        touchLoc.y < 0 ||
        touchLoc.x >= map->getContentSize().width ||
        touchLoc.y >= map->getContentSize().height)
        return false;
    else return true;
}

#pragma mark Swipe Actions

void TakasuPoppo::swipedRight(CCObjectExtension *exObj) {
    unsigned int gid = exObj->getGid();
    if (gid != 7 && gid != 14 && gid != 21 && gid != 28 && gid != 35 && gid != 42 && gid != 49 ) {
        unsigned int swapedGit = exObj->getGid() + 1;
        TakasuPoppo::swapTilesCheck(exObj, swapedGit);
    }
}

void TakasuPoppo::swipedLeft(CCObjectExtension *exObj) {
    unsigned int gid = exObj->getGid();
    if (gid != 1 && gid != 8 && gid != 15 && gid != 22 && gid != 29 && gid != 36 && gid != 43 ) {
        unsigned int swapedGit = exObj->getGid() - 1;
        TakasuPoppo::swapTilesCheck(exObj, swapedGit);
    }
}

void TakasuPoppo::swipedUp(CCObjectExtension *exObj) {
    unsigned int gid = exObj->getGid();
    if (gid >= 7) {
        unsigned int swapedGit = exObj->getGid() - 7;
        TakasuPoppo::swapTilesCheck(exObj, swapedGit);
    }
}

void TakasuPoppo::swipedDown(CCObjectExtension *exObj) {
    unsigned int gid = exObj->getGid();
    if (gid < 43) {
        unsigned int swapedGit = exObj->getGid() + 7;
        TakasuPoppo::swapTilesCheck(exObj, swapedGit);
    }
}

#pragma mark Tile Functions

void TakasuPoppo::addTileMap () {
    map = CCTMXTiledMap::create("AnotherMap.tmx");
    this->addChild(map, -1, -1);
    layer = map->layerNamed("Grids");
    TakasuPoppo::createFixture();
    
    CCArray *pChildrenArray = map->getChildren();
    CCObject *pObject = NULL;
    CCSpriteBatchNode *child = NULL;
    CCARRAY_FOREACH(pChildrenArray, pObject) {
        child = ((CCSpriteBatchNode *)pObject);
        if (!child) break;
        child->getTexture()->setAntiAliasTexParameters();
    }
}

void TakasuPoppo::createFixture() {
    CCSize layerSize = layer->getLayerSize();
    
    for (int y = 0; y < layerSize.height; y++) {
        for (int x = 0; x < layerSize.width; x++) {
            
            CCSprite *tileSprite = layer->tileAt(ccp(x, y));
            unsigned int m_gid = layer->tileGIDAt(ccp(x, y));

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
            
            while (TakasuPoppo::isTileMatched(m_gid, randomTile)) randomTile = rand() % 7;
            
            sprintf(spriteName, "%s.png", tileName[randomTile]);
            
            CCSprite *randomTileSprite = CCSprite::create(spriteName);
            CCPoint tilePosition = ccp(tileSprite->getPositionX() + tileSprite->getContentSize().width/2,
                                       tileSprite->getPositionY() + tileSprite->getContentSize().height/2);
            CCPoint tileCoordination = TakasuPoppo::tileCoorForPosition(tilePosition);
            randomTileSprite->setPosition(ccp(tilePosition.x, tilePosition.y + 10));
            
            CCObject *object = colorArray->objectAtIndex(m_gid - 1);
            CCObjectExtension *exObj = dynamic_cast<CCObjectExtension*>(object);
            TakasuPoppo::setValuesForExObj(exObj, randomTile, m_gid, randomTileSprite, tilePosition, tileCoordination);
            CCLog("Tile %i added - Color:%i; GID:%i, TileSprite:%s, TilePosition: X%i Y%i, Tile Coordination; X%i Y%i.",
                  colorArray->indexOfObject(exObj), randomTile, m_gid, spriteName, (int)tilePosition.x, (int)tilePosition.y,
                  (int)tileCoordination.x, (int)tileCoordination.y);

            this->addChild(randomTileSprite, 3, 300 + m_gid);
        }
    }
}

CCPoint TakasuPoppo::tileCoorForPosition(CCPoint position) {
    float x = position.x / map->getTileSize().width;
    float y = ((winSize.height - position.y)/(map->getTileSize().height)) -1;
    CCPoint transPos = ccp(floor(x), floor(y));
    return transPos;
}

void TakasuPoppo::swapColorID(CCObjectExtension *exObj, CCObjectExtension *swpObj) {
    int exID = exObj->getID();
    CCSprite *exSprite = exObj->getSprite();
    int swpID = swpObj->getID();
    CCSprite *swpSprite = swpObj->getSprite();
    
    exObj->setID(swpID);
    swpObj->setID(exID);
    exObj->setSprite(swpSprite);
    swpObj->setSprite(exSprite);
    CCLog("Tile %i's color %i got swaped with tile %i's color %i.",
          exObj->getGid(), exObj->getID(), swpObj->getGid(), swpObj->getID());
}

#pragma mark Tile Interactions
void TakasuPoppo::swapTilesCheck(CCObjectExtension *exObj, int swpGid) {
    CCObject *object;
    CCARRAY_FOREACH(colorArray, object) {
        CCObjectExtension *objectExtension = dynamic_cast<CCObjectExtension*>(object);
        if (objectExtension->getGid() == swpGid && exObj && objectExtension) {
            TakasuPoppo::swapColorID(exObj, objectExtension);
            if (TakasuPoppo::isBlockMatched(exObj->getGid(), exObj->getID()) ||
                TakasuPoppo::isBlockMatched(objectExtension->getGid(), objectExtension->getID())) {
                TakasuPoppo::swapColorID(exObj, objectExtension);
                TakasuPoppo::swapTilesMoving(exObj, objectExtension);
            }
            else {
                TakasuPoppo::swapColorID(exObj, objectExtension);
                TakasuPoppo::swapTilesReturn(exObj, objectExtension);
            }
        }
    }
}

void TakasuPoppo::swapTilesMoving(CCObjectExtension *exObj, CCObjectExtension *swpObj) {
    CCSprite *sprite = (CCSprite*)exObj->getSprite();
    CCSprite *swpSprite = (CCSprite*)swpObj->getSprite();
    sprite->runAction(CCMoveTo::create(0.1, swpObj->getPosition()));
    swpSprite->runAction(CCMoveTo::create(0.1, exObj->getPosition()));
    
    TakasuPoppo::swapColorID(exObj, swpObj);
    if (gridOn)TakasuPoppo::refresh();
}

void TakasuPoppo::swapTilesReturn(CCObjectExtension *exObj, CCObjectExtension *swpObj) {
    CCSprite *sprite = (CCSprite*)exObj->getSprite();
    CCSprite *swpSprite = (CCSprite*)swpObj->getSprite();
    sprite->runAction(CCSequence::create(CCMoveTo::create(0.1, swpObj->getPosition()),
                                         CCMoveTo::create(0.1, exObj->getPosition()), NULL));
    swpSprite->runAction(CCSequence::create(CCMoveTo::create(0.1, exObj->getPosition()),
                                            CCMoveTo::create(0.1, swpObj->getPosition()), NULL));
}


#pragma mark Matching Control

bool TakasuPoppo::isTileMatched(int gid, int typeID) {
    int leftTile = gid -1, leftLeftTile = gid - 2,
    rightTile = gid +1, rightRightTile = gid + 2,
    topTile = gid - 7, topTopTile = gid - 14,
    bottomTile = gid +7, bottomBottomTile = gid + 14;
    
    CCObject *tileObject;
    CCARRAY_FOREACH(colorArray, tileObject) {
        CCObjectExtension *cp = (CCObjectExtension*)(tileObject);
        CCObject *thisObject;
        if (gid != 1 && gid != 2 &&
            gid != 8 && gid != 9 &&
            gid != 15 && gid != 16 &&
            gid != 22 && gid != 23 &&
            gid != 29 && gid != 30 &&
            gid != 36 && gid != 37 &&
            gid != 43 &&gid != 44 ) {
            if (cp->getGid() == leftTile && cp->getID() == typeID) {
                CCObject *thisObject = colorArray->objectAtIndex(leftLeftTile);
                CCARRAY_FOREACH(colorArray, thisObject) {
                    CCObjectExtension *thisCp = (CCObjectExtension*)(thisObject);
                    if (thisCp->getGid() == leftLeftTile && thisCp->getID() == typeID) return true; continue;
                }
            }
        }
        if (gid != 6 && gid != 7 &&
            gid != 13 && gid != 14 &&
            gid != 20 && gid != 21 &&
            gid != 27 && gid != 28 &&
            gid != 34 && gid != 35 &&
            gid != 41 && gid != 42 &&
            gid != 48 && gid != 49 ) {
            if (cp->getGid() == rightTile && cp->getID() == typeID) {
                CCARRAY_FOREACH(colorArray, thisObject) {
                    CCObjectExtension *thisCp = (CCObjectExtension*)(thisObject);
                    if (thisCp->getGid() == rightRightTile && thisCp->getID() == typeID) return true; continue;
                }
            }
        }
        if (gid >= 15) {
            if (cp->getGid()  == topTile && cp->getID() == typeID) {
                CCARRAY_FOREACH(colorArray, thisObject) {
                    CCObjectExtension *thisCp = (CCObjectExtension*)(thisObject);
                    if (thisCp->getGid()  == topTopTile && thisCp->getID() == typeID) return true; continue;
                }
            }
        }
        if (gid <= 35) {
            if (cp->getGid()  == bottomTile && cp->getID() == typeID) {
                CCARRAY_FOREACH(colorArray, thisObject) {
                    CCObjectExtension *thisCp = (CCObjectExtension*)(thisObject);
                    if (thisCp->getGid() == bottomBottomTile && thisCp->getID() == typeID) return true; continue;
                }
            }
        }
    }
    return false;
}

bool TakasuPoppo::isBlockMatched(int gid, int typeID) {
    CCLog("GID %i & Color %i", gid, typeID);
    
    if (gid != 1 && gid != 2 && gid != 6 && gid !=7 &&
        gid != 8 && gid != 9 && gid != 13 &&  gid !=14 &&
        gid != 15 && gid != 16 && gid != 20 && gid !=21 &&
        gid != 22 && gid != 23 && gid != 27 && gid !=28 &&
        gid != 29 && gid != 30 && gid != 34 &&  gid !=35 &&
        gid != 36 && gid != 37 && gid != 41 && gid !=42 &&
        gid != 43 && gid != 44 && gid != 48 && gid !=49) {
        if (TakasuPoppo::isPentaHorMatch(gid, typeID)) {
            CCLog("Penta Hor");
            return true;
        }
    }
    
    if (gid >= 15 && gid <= 35) {
        if (TakasuPoppo::isPentaVerMatch(gid, typeID)) {
            CCLog("Penta Ver");
            return true;
        }
    }
    
    if (gid != 1 && gid != 2 && gid != 7 &&
        gid != 8 && gid != 9 && gid != 14 &&
        gid != 15 && gid != 16 && gid != 21 &&
        gid != 22 && gid != 23 && gid != 28 &&
        gid != 29 && gid != 30 && gid != 35 &&
        gid != 36 && gid != 37 && gid != 42 &&
        gid != 43 && gid != 44 && gid != 49) {
        if (TakasuPoppo::isLeftQuadMatch(gid, typeID)) {
            CCLog("Left Quad");
            return true;
        }
    }
    
    if (gid != 1 && gid != 6 && gid != 7 && 
        gid != 8 && gid != 13 && gid != 14 && 
        gid != 15 && gid != 20 && gid != 21 && 
        gid != 22 && gid != 27 && gid != 28 &&
        gid != 29 && gid != 34 && gid != 35 &&
        gid != 36 && gid != 41 && gid != 42 &&
        gid != 43 && gid != 48 && gid != 48) {
        if (TakasuPoppo::isRightQuadMatch(gid, typeID)) {
            CCLog("Right Quad");
            return true;
        }
    }
    
    if (gid >= 15 && gid <= 42) {
        if (TakasuPoppo::isUpQuadMatch(gid, typeID)) {
            CCLog("Top Quad");
            return true;
        }
    }
    
    if (gid >= 8 && gid <= 35) {
        if (TakasuPoppo::isDownQuadMatch(gid, typeID)) {
            CCLog("Bottom Quad");
            return true;
        }
    }
    
    if (gid != 1 &&  gid !=7 &&
        gid != 8 &&  gid !=14 &&
        gid != 15 &&  gid !=21 &&
        gid != 22 &&  gid !=28 &&
        gid != 29 &&  gid !=35 &&
        gid != 36 &&  gid !=42 &&
        gid != 43 &&  gid !=49) {
        if (TakasuPoppo::isMidHorMatch(gid, typeID)) {
            CCLog("Mid Hor");
            return true;
        }
    }
    
    if (gid >= 8 && gid <= 42) {
        if (TakasuPoppo::isMidVerMatch(gid, typeID)) {
            CCLog("Mid Ver");
            return true;
        }
    }
    
    if (gid != 1 && gid != 2 &&
        gid != 8 && gid != 9 &&
        gid != 15 && gid != 16 &&
        gid != 22 && gid != 23 &&
        gid != 29 && gid != 30 &&
        gid != 36 && gid != 37 &&
        gid != 43 && gid != 44 ) {
        if (TakasuPoppo::isLeftTriMatch(gid, typeID)) {
            CCLog("Left Tri");
            return true;
        }
    }
    
    if (gid != 6 && gid != 7 &&
        gid != 13 && gid != 14 &&
        gid != 20 && gid != 21 &&
        gid != 27 && gid != 28 &&
        gid != 34 && gid != 35 &&
        gid != 41 && gid != 42 &&
        gid != 48 && gid != 49 ) {
        if (TakasuPoppo::isRightTriMatch(gid, typeID)) {
            CCLog("Right Tri");
            return true;
        }
    }
    
    if (gid >= 15) {
        if (TakasuPoppo::isTopTriMatch(gid, typeID)) {
            CCLog("Top Tri");
            return true;
        }
    }
    
    if (gid <= 35) {
        if (TakasuPoppo::isBottomTriMatch(gid, typeID)) {
            CCLog("Bottom Tri");
            return true;
        }
    }
    return false;
}

bool TakasuPoppo::isRightTriMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *rightObject = colorArray->objectAtIndex(gidToIndex + 1);
    CCObjectExtension *rightExObj = dynamic_cast<CCObjectExtension*>(rightObject);
    CCObject *rightRightObject = colorArray->objectAtIndex(gidToIndex + 2);
    CCObjectExtension *rightRightEx = dynamic_cast<CCObjectExtension*>(rightRightObject);
    if (rightExObj->getID() == typeID && rightRightEx->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isLeftTriMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *leftObject = colorArray->objectAtIndex(gidToIndex - 1);
    CCObjectExtension *leftExObj = dynamic_cast<CCObjectExtension*>(leftObject);
    CCObject *leftLeftObject = colorArray->objectAtIndex(gidToIndex - 2);
    CCObjectExtension *leftLeftEx = dynamic_cast<CCObjectExtension*>(leftLeftObject);
    CCLog("Left Object ID: %i ; Left Left Object ID: %i", leftExObj->getID(), leftLeftEx->getID());
    CCLog("Left Object GID:%i ; Left Left Object GID:%i", leftExObj->getGid(), leftLeftEx->getGid());
    if (leftExObj->getID() == typeID && leftLeftEx->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isTopTriMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *topObject = colorArray->objectAtIndex(gidToIndex - 7);
    CCObjectExtension *topExObj = dynamic_cast<CCObjectExtension*>(topObject);
    CCObject *topTopObj = colorArray->objectAtIndex(gidToIndex - 14);
    CCObjectExtension *topTopEx = dynamic_cast<CCObjectExtension*>(topTopObj);
    if (topExObj->getID() == typeID && topTopEx->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isBottomTriMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *bottomObject = colorArray->objectAtIndex(gidToIndex + 7);
    CCObjectExtension *bottomExObj = dynamic_cast<CCObjectExtension*>(bottomObject);
    CCObject *bottomBottomObject = colorArray->objectAtIndex(gidToIndex + 14);
    CCObjectExtension *bottomBottomEx = dynamic_cast<CCObjectExtension*>(bottomBottomObject);
    if (bottomExObj->getID() == typeID && bottomBottomEx->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isLeftQuadMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *leftObject = colorArray->objectAtIndex(gidToIndex - 1);
    CCObjectExtension *leftExObj = dynamic_cast<CCObjectExtension*>(leftObject);
    CCObject *leftLeftObject = colorArray->objectAtIndex(gidToIndex - 2);
    CCObjectExtension *leftLeftEx = dynamic_cast<CCObjectExtension*>(leftLeftObject);
    CCObject *rightObject = colorArray->objectAtIndex(gidToIndex + 1);
    CCObjectExtension *rightExObj = dynamic_cast<CCObjectExtension*>(rightObject);
    if (leftExObj->getID() == typeID && leftLeftEx->getID() == typeID && rightExObj->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isRightQuadMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *rightObject = colorArray->objectAtIndex(gidToIndex + 1);
    CCObjectExtension *rightExObj = dynamic_cast<CCObjectExtension*>(rightObject);
    CCObject *rightRightObject = colorArray->objectAtIndex(gidToIndex + 2);
    CCObjectExtension *rightRightEx = dynamic_cast<CCObjectExtension*>(rightRightObject);
    CCObject *leftObject = colorArray->objectAtIndex(gidToIndex - 1);
    CCObjectExtension *leftExObj = dynamic_cast<CCObjectExtension*>(leftObject);
    if (rightExObj->getID() == typeID && rightRightEx->getID() == typeID && leftExObj->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isUpQuadMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *topObject = colorArray->objectAtIndex(gidToIndex - 7);
    CCObjectExtension *topExObj = dynamic_cast<CCObjectExtension*>(topObject);
    CCObject *topTopObj = colorArray->objectAtIndex(gidToIndex - 14);
    CCObjectExtension *topTopEx = dynamic_cast<CCObjectExtension*>(topTopObj);
    CCObject *bottomObject = colorArray->objectAtIndex(gidToIndex + 7);
    CCObjectExtension *bottomExObj = dynamic_cast<CCObjectExtension*>(bottomObject);
    if (topExObj->getID() == typeID && topTopEx->getID() == typeID && bottomExObj->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isDownQuadMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *bottomObject = colorArray->objectAtIndex(gidToIndex + 7);
    CCObjectExtension *bottomExObj = dynamic_cast<CCObjectExtension*>(bottomObject);
    CCObject *bottomBottomObject = colorArray->objectAtIndex(gidToIndex + 14);
    CCObjectExtension *bottomBottomEx = dynamic_cast<CCObjectExtension*>(bottomBottomObject);
    CCObject *topObject = colorArray->objectAtIndex(gidToIndex - 7);
    CCObjectExtension *topExObj = dynamic_cast<CCObjectExtension*>(topObject);
    if (bottomExObj->getID() == typeID && bottomBottomEx->getID() == typeID && topExObj->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isMidHorMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *leftObject = colorArray->objectAtIndex(gidToIndex - 1);
    CCObjectExtension *leftExObj = dynamic_cast<CCObjectExtension*>(leftObject);
    CCObject *rightObject = colorArray->objectAtIndex(gidToIndex + 1);
    CCObjectExtension *rightExObj = dynamic_cast<CCObjectExtension*>(rightObject);
    if (leftExObj->getID() == typeID && rightExObj->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isMidVerMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *topObject = colorArray->objectAtIndex(gidToIndex - 7);
    CCObjectExtension *topExObj = dynamic_cast<CCObjectExtension*>(topObject);
    CCObject *bottomObject = colorArray->objectAtIndex(gidToIndex + 7);
    CCObjectExtension *bottomExObj = dynamic_cast<CCObjectExtension*>(bottomObject);
    if (topExObj->getID() == typeID && bottomExObj->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isPentaHorMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *rightObject = colorArray->objectAtIndex(gidToIndex + 1);
    CCObjectExtension *rightExObj = dynamic_cast<CCObjectExtension*>(rightObject);
    CCObject *rightRightObject = colorArray->objectAtIndex(gidToIndex + 2);
    CCObjectExtension *rightRightEx = dynamic_cast<CCObjectExtension*>(rightRightObject);
    CCObject *leftObject = colorArray->objectAtIndex(gidToIndex - 1);
    CCObjectExtension *leftExObj = dynamic_cast<CCObjectExtension*>(leftObject);
    CCObject *leftLeftObject = colorArray->objectAtIndex(gidToIndex - 2);
    CCObjectExtension *leftLeftEx = dynamic_cast<CCObjectExtension*>(leftLeftObject);
    if (leftExObj->getID() == typeID && leftLeftEx->getID() == typeID &&
        rightExObj->getID() == typeID && rightRightEx->getID() == typeID) return true;
    return false;
}

bool TakasuPoppo::isPentaVerMatch(int gid, int typeID) {
    int gidToIndex = gid - 1;
    CCObject *topObject = colorArray->objectAtIndex(gidToIndex - 7);
    CCObjectExtension *topExObj = dynamic_cast<CCObjectExtension*>(topObject);
    CCObject *topTopObj = colorArray->objectAtIndex(gidToIndex - 14);
    CCObjectExtension *topTopEx = dynamic_cast<CCObjectExtension*>(topTopObj);
    CCObject *bottomObject = colorArray->objectAtIndex(gidToIndex + 7);
    CCObjectExtension *bottomExObj = dynamic_cast<CCObjectExtension*>(bottomObject);
    CCObject *bottomBottomObject = colorArray->objectAtIndex(gidToIndex + 14);
    CCObjectExtension *bottomBottomEx = dynamic_cast<CCObjectExtension*>(bottomBottomObject);
    if (topExObj->getID() == typeID && topTopEx->getID() == typeID &&
        bottomExObj->getID() == typeID && bottomBottomEx->getID() == typeID) return true;
    return false;
}
#pragma mark Array

void TakasuPoppo::addBlocksToArray() {
    for (int i = 0; i < 49; i ++) {
        CCObjectExtension *exObj = new CCObjectExtension(0, 0, NULL, ccp(0, 0), ccp(0, 0));
        colorArray->addObject(exObj);
    }
}

void TakasuPoppo::setValuesForExObj(CCObjectExtension *exObj, int colorID, int gid, CCSprite *sprite, CCPoint position, CCPoint coordination) {
    exObj->setID(colorID);
    exObj->setGid(gid);
    exObj->setSprite(sprite);
    exObj->setPosition(position);
    exObj->setCoordination(coordination);
    
    CCSprite *toMoveSprite = exObj->getSprite();
    toMoveSprite->runAction(CCMoveTo::create(0.1, exObj->getPosition()));
}

void TakasuPoppo::checkAndAddToRemove() {
    CCObject *object;
    CCARRAY_FOREACH(colorArray, object) {
        CCObjectExtension *exObj = dynamic_cast<CCObjectExtension*>(object);
        int gid = exObj->getGid();
        int id = exObj->getID();
        if (TakasuPoppo::isBlockMatched(gid, id)) {
            toDestroyArray->addObject(exObj);
        }
    }
}

void TakasuPoppo::removeObjectsFromDestroyArray() {
    if (toDestroyArray->count() > 0) {
        CCObject *object;
        CCARRAY_FOREACH(toDestroyArray, object) {
            CCObjectExtension *exObj = dynamic_cast<CCObjectExtension*>(object);
            CCSprite *toRemoveSprite = exObj->getSprite();
            exObj->setID(7);
            exObj->setSprite(NULL);
            if (toRemoveSprite)toRemoveSprite->removeFromParentAndCleanup(true);
            TakasuPoppo::onRemoveMoveTiles(exObj);
            if (gridOn)TakasuPoppo::refresh();
        }
    }
}

void TakasuPoppo::onRemoveMoveTiles(CCObjectExtension *exObj) {
    
    
}

#pragma mark Debug

void TakasuPoppo::setupDebugButton() {
    buttonSprite = CCSprite::create("ShowGridButton.png");
    buttonSprite->setPosition(ccp(buttonSprite->getContentSize().width/2 + 15, winSize.height - buttonSprite->getContentSize().height/2 - 15));
    this->addChild(buttonSprite, 3);
    
    refreshButton = CCSprite::create("RefreshButton.png");
    refreshButton->setPosition(ccp(refreshButton->getContentSize().width/2 + 230, winSize.height - refreshButton->getContentSize().height/2 - 15));
    this->addChild(refreshButton, 3);
    
    removeButton = CCSprite::create("ClearButton.png");
    removeButton->setPosition(ccp(removeButton->getContentSize().width/2 + 445, winSize.height - removeButton->getContentSize().height/2 - 15));
    this->addChild(removeButton, 3);
}

void TakasuPoppo::switchGrid() {
    if (!gridOn) {
        CCSize layerSize = layer->getLayerSize();
        CCObject *object = NULL;
        CCARRAY_FOREACH(colorArray, object) {
            CCObjectExtension *exObject = dynamic_cast<CCObjectExtension*>(object);
            CCPoint spritePos = exObject->getPosition();
            CCSprite *backgroundTile = CCSprite::create("Background.png");
            CCString *idString = CCString::createWithFormat("%i", exObject->getID());
            CCLabelTTF *stringLabel = CCLabelTTF::create(idString->getCString(), "Arial", 30);
            stringLabel->setPosition(spritePos);
            backgroundTile->setPosition(spritePos);
            
            this->addChild(backgroundTile, 3 , 801);
            this->addChild(stringLabel, 4 , 802);
            
            debugTilesArray->addObject(stringLabel);
            debugTilesArray->addObject(backgroundTile);
            gridOn = true;
        }
    }
}

void TakasuPoppo::remove() {
    CCObject *object;
    CCARRAY_FOREACH(debugTilesArray, object) {
        CCSprite *sprite = dynamic_cast<CCSprite*>(object); 
        this->removeChild(sprite);
        gridOn = false;
    }
}

void TakasuPoppo::refresh() {
    TakasuPoppo::remove();
    TakasuPoppo::switchGrid();
}

void TakasuPoppo::checkTile() {
    CCObject* object = colorArray->objectAtIndex(25);
    CCObjectExtension *objEx = dynamic_cast<CCObjectExtension*>(object);
    CCObject* object2 = colorArray->objectAtIndex(24);
    CCObjectExtension *objEx2 = dynamic_cast<CCObjectExtension*>(object2);
    CCLog("Color id of tile 25 and 24 are %i and %i", objEx->getID(), objEx2->getID());
}

void TakasuPoppo::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
