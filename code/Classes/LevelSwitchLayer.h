#pragma once

#include "cocos2d.h"
USING_NS_CC;

class LevelSwitchLayer:public cocos2d::Layer
{
public:
	LevelSwitchLayer();
	~LevelSwitchLayer();
public:
	static cocos2d::Scene* createScene(int level=1);
	CREATE_FUNC(LevelSwitchLayer);
	virtual bool init();
	void init_Create();
	void startGame(float delta);
	void showLevel();
private:
	cocos2d::LabelTTF*		_leveString;
	int						_level;

};

