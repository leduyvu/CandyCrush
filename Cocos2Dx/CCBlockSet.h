//
//  CCBlockSet.h
//  Cocos2Dx
//
//  Created by Ace on 2013/06/26.
//
//

#ifndef __Cocos2Dx__CCBlockSet__
#define __Cocos2Dx__CCBlockSet__

#include <CCObjectExtension.h>
#include "cocos2d.h"

class CCBlockSet : public CCObject {
private:
    CCArray *blockArray;
public:
    CCBlockSet(CCArray *blockArray);
    bool isBlockSetHorizontal();
    int blockCount();
};
using namespace std;
USING_NS_CC;

#endif /* defined(__Cocos2Dx__CCBlockSet__) */
