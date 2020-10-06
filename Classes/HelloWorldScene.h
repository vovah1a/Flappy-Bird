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
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuRestartCallback(cocos2d::Ref* Scene);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    //Window width/height
    int _winWidth;
    int _winHeight;
    //Origin
    cocos2d::CCPoint _origin;

    //Keep track of game time in seconds
    float _gameTime;
    float _pipeTime;

    cocos2d::CCSprite* _background;

    cocos2d::CCSprite* _player;
    float _playerX;
    float speed;
    bool startgame;
    float incline;
    bool _GG;


    cocos2d::CCSprite* _pipe[10];
    int _pipeCount;
    int pipeI;

    //Display Game Result
    cocos2d::CCLabelTTF* _gameResultLabel;
    int points;

    void updateGame(float dt);
    int randomPipe();
    void spriteMoveFinished(cocos2d::CCNode* sender);
    //Handle lose
    void endGame();
};

#endif // __HELLOWORLD_SCENE_H__
