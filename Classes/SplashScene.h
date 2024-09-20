

#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "Cursor/Cursor.h"
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    CREATE_FUNC(HelloWorld);
private:
    Cursor* _cursor;
};

#endif // __SPLASH_SCENE_H__
