#include "WelcomeLayer.h"
#include "VisibleRect.h"
#include "GameLayer.h"
#include "LevelSwitchLayer.h"



WelcomeLayer::WelcomeLayer()
{
}


WelcomeLayer::~WelcomeLayer()
{
}


cocos2d::Scene* WelcomeLayer::createScene()
{
	auto thescene = cocos2d::Scene::create();
	auto thelayer = WelcomeLayer::create();
	thescene->addChild(thelayer);
	return thescene;
}

bool WelcomeLayer::init()
{
	do 
	{
		CC_BREAK_IF(!cocos2d::Layer::init());
		preLoadMusic();
		init_Create();
		return true;
	} while (0);
	return false;
}

void WelcomeLayer::init_Create()
{
	auto logoSprite = cocos2d::Sprite::create("BattleCity.png");
	logoSprite->setPosition(VisibleRect::center()+Vec2(0,100));
	this->addChild(logoSprite);
	//////////////////////////////////////////////////////////////////////////
	auto menuItem01 = cocos2d::MenuItemFont::create("Start Game", this, menu_selector(WelcomeLayer::startGame));
	menuItem01->setPosition(VisibleRect::center() + Vec2(0, -50));
	menuItem01->setFontSize(55);
	//menuItem01->setFontNameObj("Georgia-Bold");
	
	menuItem01->setFontNameObj("Marker Felt");

	auto menu = cocos2d::Menu::create(menuItem01, NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu);
	

	
}

void WelcomeLayer::startGame(Ref* pNode)
{

	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("levelstarting.wav");
	auto theScene = LevelSwitchLayer::createScene(1);
	//Director::getInstance()->replaceScene(theScene);

	//
	//CCTransitionMoveInR::transitionWithDuration(t, s);//新场景从右移入覆盖  
	//Director::getInstance()->replaceScene(TransitionMoveInR::create(5.0f, theScene));

	//CCTransitionFade::transitionWithDuration(t, s)
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f, theScene));

}

void WelcomeLayer::preLoadMusic()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("start.aif");

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bonus.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("brickhit.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("eexplosion.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("fexplosion.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("gameover.wav");

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("ice.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("levelstarting.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("life.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("moving.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("nmoving.wav");

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pause.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("shieldhit.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("shoot.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("steelhit.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("tbonushit.wav");


	//CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("start.aif", true);
}
