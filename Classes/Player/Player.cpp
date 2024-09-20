// Player.cpp
#include "Player.h"
#include "cocos2d.h"

USING_NS_CC;

Player::Player() : _mousePos(Vec2::ZERO), _velocity(Vec2::ZERO), _speed(100.0f), _isMoving(false) {}
Player::~Player() {}

Player* Player::createPlayer()
{
    Player* player = new (std::nothrow) Player();
    if (player && player->init())
    {
        player->autorelease();
        return player;
    }
    else
    {
        delete player;
        return nullptr;
    }
}

bool Player::init()
{
    // Load the sprite frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("asset/maingame.plist");

    // Check if the sprite frame exists in the cache
    auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Walk_riffle_000.png");
    if (!spriteFrame)
    {
        CCLOG("Sprite frame 'Walk_riffle_000.png' not found in the cache");
        return false;
    }

    // Initialize the sprite with the sprite frame
    if (!Sprite::initWithSpriteFrame(spriteFrame))
    {
        return false;
    }
    
    this->setPosition(Vec2(100, 100));
    this->setScale(0.25f);
    this->setAnchorPoint(Vec2(0.5, 0.5)); // Set anchor point at the head of the character

    // Add mouse event listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(Player::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // Add keyboard event listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // Schedule update method
    this->scheduleUpdate();

    // Create animation
    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 10; i++) // Assuming you have 10 frames for the walk animation
    {
        std::string frameName = "Walk_riffle_" + StringUtils::format("%03d", i) + ".png";
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            animFrames.pushBack(frame);
        }
        else
        {
            CCLOG("Frame %s not found", frameName.c_str());
        }
    }

    if (!animFrames.empty())
    {
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        _animate = Animate::create(animation);
        _animate->retain();
    }
    else
    {
        CCLOG("No frames found for animation");
    }

    return true;
}

void Player::onMouseMove(Event* event)
{
    EventMouse* e = (EventMouse*)event;
    _mousePos = Vec2(e->getCursorX(), e->getCursorY());
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_W:
        moveUp();
        break;
    case EventKeyboard::KeyCode::KEY_S:
        moveDown();
        break;
    case EventKeyboard::KeyCode::KEY_A:
        moveLeft();
        break;
    case EventKeyboard::KeyCode::KEY_D:
        moveRight();
        break;
    default:
        break;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_S:
        stopVerticalMovement();
        break;
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
        stopHorizontalMovement();
        break;
    default:
        break;
    }
}

void Player::moveUp()
{
    _velocity.y = 1;
    _isMoving = true;
}

void Player::moveDown()
{
    _velocity.y = -1;
    _isMoving = true;
}

void Player::moveLeft()
{
    _velocity.x = -1;
    _isMoving = true;
}

void Player::moveRight()
{
    _velocity.x = 1;
    _isMoving = true;
}

void Player::stopVerticalMovement()
{
    _velocity.y = 0;
    if (_velocity.x == 0)
    {
        _isMoving = false;
    }
}

void Player::stopHorizontalMovement()
{
    _velocity.x = 0;
    if (_velocity.y == 0)
    {
        _isMoving = false;
    }
}

void Player::update(float delta)
{
    // Cập nhật vị trí dựa trên vận tốc
    Vec2 position = this->getPosition();
    position += _velocity * _speed * delta;
    this->setPosition(position);

    // Chuyển đổi vị trí chuột từ hệ tọa độ màn hình sang hệ tọa độ OpenGL
    auto mousePos = Director::getInstance()->convertToGL(_mousePos);
    Vec2 playerPos = this->getPosition();
    Vec2 direction = mousePos - playerPos;

    // Tính toán góc quay từ hướng
    float angle = CC_RADIANS_TO_DEGREES(atan2(direction.y, direction.x));

    // Điều chỉnh góc quay để đảm bảo nhân vật luôn hướng về phía chuột
    this->setRotation(-angle + 90);

    // Lật hình ảnh nếu cần thiết
    if (angle > 90 || angle < -90)
    {
        this->setFlippedY(true);
    }
    else
    {
        this->setFlippedY(false);
    }

    // Chạy hoạt ảnh khi di chuyển
    if (_isMoving)
    {
        if (!this->getActionByTag(1))
        {
            auto animateAction = RepeatForever::create(_animate);
            animateAction->setTag(1);
            this->runAction(animateAction);
        }
    }
    else
    {
        this->stopActionByTag(1);
    }
}





