#pragma once

#include "cocos2d.h"
USING_NS_CC;

class GameOverLayer:public cocos2d::Layer
{
public:
	GameOverLayer();
	~GameOverLayer();
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(GameOverLayer);
	virtual bool init();
	void init_Create();
	void startGame(Ref* pNode);

};

