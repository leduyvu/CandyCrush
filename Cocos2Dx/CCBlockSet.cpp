//
//  CCBlockSet.cpp
//  Cocos2Dx
//
//  Created by Ace on 2013/06/26.
//
//

#include "CCBlockSet.h"
#include "CCObjectExtension.h"
using namespace cocos2d;
using namespace std;
USING_NS_CC;

CCBlockSet::CCBlockSet (CCArray *blockArray) {
    this->blockArray = new CCArray;
    blockArray = new CCArray;
    this->blockArray = blockArray;
}

bool CCBlockSet::isBlockSetHorizontal() {
    if (this->blockArray->count() > 1) {
        CCObject *object;
        CCARRAY_FOREACH(this->blockArray, object) {
            CCObjectExtension *exObj = dynamic_cast<CCObjectExtension*>(object);
            CCObject *objLast = blockArray->lastObject();
            CCObjectExtension *exLast = dynamic_cast<CCObjectExtension*>(objLast);
            if (exObj->getCoordination().x == exLast->getCoordination().x) return true;
        }
    }
    return  false;
}