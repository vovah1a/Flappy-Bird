/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:   
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES O F MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    CCSize  visibleSize = Director::getInstance()->getVisibleSize();
    CCPoint  origin = Director::getInstance()->getVisibleOrigin();

   
    do {

        //Initialize game time
        _gameTime = 0;

        //Get window width/height
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        _winHeight = winSize.height;
        _winWidth = winSize.width;

        //background sprites
        _background = CCSprite::create("background.png");
        _background->setScale(2.0f);
        _background->setPosition(ccp(_winWidth / 2, _winHeight / 2));
        this->addChild(_background, 1);
        
        //Load Player sprite
        _player = CCSprite::create("fatBird.png");
        _player->setScale(1.5f);
        _playerX = _origin.x + (_player->getScaleY() * _player->getContentSize().width);
        _player->setPosition(ccp(_playerX * 1.5f, _winHeight / 2));
        this->addChild(_player, 10);
        speed = 0;

        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);

        listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        _GG = false;
        startgame = false;

        //pipe
        for (int i = 0; i < 10; i++) {
            _pipe[i] = CCSprite::create("pipe.png");
            _pipe[i]->setScale(1.5f);
            _pipe[i]->setVisible(false);

            if (i % 2 != 0) {
                _pipe[i]->setRotation(180.0f);
                _pipe[i]->setPosition(ccp(_winWidth + (_pipe[i]->getContentSize().height / 2), _winHeight));
            }
            else {
                _pipe[i]->setPosition(ccp(_winWidth + (_pipe[i]->getContentSize().height / 2), 0.0f));
            }
            this->addChild(_pipe[i], 1);
        }
        pipeI = 0;

        points = 0;

        //Initialize game result label
        _gameResultLabel = CCLabelTTF::create("", "Arial", 60);
        _gameResultLabel->setPosition(ccp(_winWidth / 2, _winHeight * 0.9));
        _gameResultLabel->setString(std::to_string(points));
        _gameResultLabel->setVisible(true);
        this->addChild(_gameResultLabel, 1);

        auto myMenu = Menu::create();
        Vector<MenuItem*> MenuItems;

        auto closeItem = MenuItemImage::create("rest.png", "rest1.png",
            CC_CALLBACK_1(HelloWorld::menuRestartCallback, this));
        closeItem->setScale(0.08f);
        MenuItems.pushBack(closeItem);

        auto menu = Menu::createWithArray(MenuItems);
        menu->setPosition(ccp(_winWidth / 2, _winHeight * 0.05f));
        this->addChild(menu, 1);
        //schedule updateGame to update the game at regular intervals
        this->schedule(schedule_selector(HelloWorld::updateGame));
    } while (0);
    return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    startgame = true;
    speed = 0;
    incline = -60;
    auto moveByTouch = MoveBy::create(0.07, Vec2(0, 100));
    _player->runAction(moveByTouch);
    return true;
}

void HelloWorld::updateGame(float dt) {
    _gameTime += dt; //Add dt to game time
    if (!_GG) {
        if (startgame) {
            if (speed >= -20)
            {
                if (incline < 90) { 
                    incline += 2; 
                    _player->setRotation(incline);
                }
                speed -= 0.2;
            }
            auto moveByspeed = MoveBy::create(0.05, Vec2(0, speed));
            _player->runAction(moveByspeed);

            if (_pipeTime == 0) {
                _pipeTime = _gameTime;
                int r = randomPipe() - _pipe[_pipeCount]->getContentSize().height;
                auto moveTo = MoveTo::create(3, Vec2(-(_pipe[_pipeCount]->getContentSize().height / 2), r));
                _pipe[_pipeCount]->stopAllActions();
                _pipe[_pipeCount]->setPosition(ccp(_winWidth + (_pipe[_pipeCount]->getContentSize().height / 2), r));
                _pipe[_pipeCount]->setVisible(true);
                _pipe[_pipeCount]->runAction(CCSequence::create((moveTo), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished)), NULL));

                auto moveTo1 = MoveTo::create(3, Vec2(-(_pipe[_pipeCount + 1]->getContentSize().height / 2), r + _winHeight));
                _pipe[_pipeCount + 1]->stopAllActions();
                _pipe[_pipeCount + 1]->setPosition(ccp(_winWidth + (_pipe[_pipeCount + 1]->getContentSize().height / 2), r + _winHeight));
                _pipe[_pipeCount + 1]->setVisible(true);
                _pipe[_pipeCount + 1]->runAction(CCSequence::create((moveTo1), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished)), NULL));

                _pipeCount += 2;

                if (_pipeCount >= 10) {
                    _pipeCount = 0;
                }

            }
            else {
                if (_pipeTime < _gameTime - 1.2f) {
                    _pipeTime = 0;
                }
            }

            for (int i = 0; i < 10; ++i) {
                if (_pipe[i]->isVisible() && _player->isVisible()) {
                    CCLOG("_pipe = %f, _player = %f", _pipe[i]->getPositionX(), _player->getPositionX());
                    if (_pipe[i]->boundingBox().intersectsRect(_player->boundingBox())) {
                        _player->stopAllActions();
                        _player->setVisible(false);
                        endGame();
                    }
                }
            }

            if ((_player->getPositionY() <= 0.0f) || (_player->getPositionY() >= _winWidth))
            {
                _player->stopAllActions();
                _player->setVisible(false);
                endGame();
            }

            if (_player->getPositionX() >= _pipe[pipeI]->getPositionX()) {
                points++;
                pipeI += 2;
                _gameResultLabel->setString(std::to_string(points));
                if (pipeI >= 10) { pipeI = 0; }
            }
        }
    }
}

void HelloWorld::spriteMoveFinished(cocos2d::CCNode* sender) {
    sender->setVisible(false);
}

int HelloWorld::randomPipe() {
    return rand() % (_winWidth - _winWidth / 4) + _winWidth / 4;
}

void HelloWorld::endGame() {
    startgame = false;
    auto moveToResult = MoveTo::create(2, Vec2(_winWidth / 2, _winHeight * 0.7));
    _gameResultLabel->stopAllActions();
    _gameResultLabel->runAction(moveToResult);
    auto scaleToResult = ScaleTo::create(2.0f, 2.0f);
    _gameResultLabel->runAction(scaleToResult);
    _gameResultLabel->setString(std::to_string(points));
    _GG = true;
}

void HelloWorld::menuRestartCallback(Ref* Scene) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0, this->createScene()));
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
