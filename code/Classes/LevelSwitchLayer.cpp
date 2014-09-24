#include "LevelSwitchLayer.h"
#include "VisibleRect.h"
#include "GameOverLayer.h"
#include "GameLayer.h"



LevelSwitchLayer::LevelSwitchLayer()
{

}


LevelSwitchLayer::~LevelSwitchLayer()
{
}


cocos2d::Scene* LevelSwitchLayer::createScene(int level)
{
	auto thescene = cocos2d::Scene::create();
	auto thelayer = LevelSwitchLayer::create();
	thelayer->_level = level;
	thelayer->init_Create();
	thescene->addChild(thelayer);
	return thescene;
}

bool LevelSwitchLayer::init()
{
	do 
	{
		CC_BREAK_IF(!cocos2d::Layer::init());
		_level = 1;
		_leveString = NULL;
		return true;
	} while (0);
	return false;
}

void LevelSwitchLayer::init_Create()
{
	auto backColor = LayerColor::create(ccc4(0, 0, 0, 225));
	this->addChild(backColor);

	showLevel();
	this->scheduleOnce(schedule_selector(LevelSwitchLayer::startGame),2.0f);


	

	
}

void LevelSwitchLayer::startGame(float delta)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("levelstarting.wav");
	auto theScene = GameLayer::createScene();
	//Director::getInstance()->replaceScene(theScene);

	//
	//CCTransitionMoveInR::transitionWithDuration(t, s);//新场景从右移入覆盖  
	//Director::getInstance()->replaceScene(TransitionMoveInR::create(5.0f, theScene));

	//CCTransitionFade::transitionWithDuration(t, s)
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f, theScene));

}


void LevelSwitchLayer::showLevel()
{
	if (_leveString == NULL)
	{
		_leveString = cocos2d::LabelTTF::create(__String::createWithFormat("level %d", _level)->getCString(), "Courier-Bold", 32);
		_leveString->setColor(ccc3(255, 255, 255));
		_leveString->setPosition(VisibleRect::center());
		this->addChild(_leveString, 1);
	}
	else
	{
		_leveString->setString(__String::createWithFormat("level %d", _level)->getCString());
	}


}