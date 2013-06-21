#include "CCObjectExtension.h"

using namespace cocos2d;
using namespace std;
USING_NS_CC;

CCObjectExtension::CCObjectExtension(int id, int gid, CCSprite* sprite, CCPoint position, CCPoint coordination) {
    this->colorId = id;
    this->gid = gid;
    this->blockSprite = sprite;
    this->position = position;
    this->coordination = coordination;
}

int CCObjectExtension::getID() {
    return this->colorId;
}

void CCObjectExtension::setID(int id) {
    this->colorId=id;
}

int CCObjectExtension::getGid() {
    return gid;
}

void CCObjectExtension::setGid(int c) {
    this->gid = c;
}

CCSprite *CCObjectExtension::getSprite() {
    return blockSprite;
}

void CCObjectExtension::setSprite(CCSprite *sprite) {
    this->blockSprite = sprite;
}

CCPoint CCObjectExtension::getPosition() {
    return position;
}

void CCObjectExtension::setPosition(CCPoint position) {
    this->position = position;
}

CCPoint CCObjectExtension::getCoordination() {
    return coordination;
}

void CCObjectExtension::setCoordination(CCPoint coordination) {
    this->coordination = coordination;
}



