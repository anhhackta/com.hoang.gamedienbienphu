// Player.h
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
public:
    Player();
    virtual ~Player();

    static Player* createPlayer();
    virtual bool init();

    void onMouseMove(cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void stopVerticalMovement();
    void stopHorizontalMovement();

    void update(float delta);

private:
    cocos2d::Vec2 _mousePos;
    cocos2d::Vec2 _velocity;
    float _speed;
    bool _isMoving; // Add this line
    cocos2d::Animate* _animate; // Add this line
};

#endif // __PLAYER_H__
