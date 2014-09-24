#include "WelcomeLayer.h"
#include "VisibleRect.h"
#include "GameOverLayer.h"
#include "GameLayer.h"



GameOverLayer::GameOverLayer()
{
}


GameOverLayer::~GameOverLayer()
{
}


cocos2d::Scene* GameOverLayer::createScene()
{
	auto thescene = cocos2d::Scene::create();
	auto thelayer = GameOverLayer::create();
	thescene->addChild(thelayer);
	return thescene;
}

bool GameOverLayer::init()
{
	do 
	{
		CC_BREAK_IF(!cocos2d::Layer::init());
		init_Create();
		return true;
	} while (0);
	return false;
}

void GameOverLayer::init_Create()
{
	auto backColor = LayerColor::create(ccc4(0, 0, 0, 225));
	this->addChild(backColor);
	auto logoSprite = cocos2d::Sprite::create("over.png");
	logoSprite->setPosition(VisibleRect::center()+Vec2(0,100));
	this->addChild(logoSprite);
	//////////////////////////////////////////////////////////////////////////
	auto menuItem01 = cocos2d::MenuItemFont::create("ReStart Game", this, menu_selector(GameOverLayer::startGame));
	menuItem01->setPosition(VisibleRect::center() + Vec2(0, -50));
	menuItem01->setFontSize(55);
	//menuItem01->setFontNameObj("Georgia-Bold");
	
	menuItem01->setFontNameObj("Marker Felt");

	auto menu = cocos2d::Menu::create(menuItem01, NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu);
	

	
}

void GameOverLayer::startGame(Ref* pNode)
{
	GameLayer::_iplife = 1;
	auto theScene = GameLayer::createScene();
	//Director::getInstance()->replaceScene(theScene);

	//
	//CCTransitionMoveInR::transitionWithDuration(t, s);//新场景从右移入覆盖  
	//Director::getInstance()->replaceScene(TransitionMoveInR::create(5.0f, theScene));

	//CCTransitionFade::transitionWithDuration(t, s)
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f, theScene));

}
