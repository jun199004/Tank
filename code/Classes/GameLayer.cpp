#include "GameLayer.h"
#include "VisibleRect.h"
#include "HRockerLayer.h"
#include "TileMapInfo.h"
#include "Tank.h"
#include "EnemyAI.h"
#include "GameOverLayer.h"
#include "LevelSwitchLayer.h"


static int enemyTanksCount[] =
{ 0,
1, 10, 12, 14, 16,
10, 10, 10, 10, 10,
10, 10, 10, 10, 10,
10, 10, 10, 10, 10,
};

int GameLayer::_level = 1;
int GameLayer::_iplife = 1;
GameLayer::GameLayer()
{

}


GameLayer::~GameLayer()
{
}


cocos2d::Scene* GameLayer::createScene()
{
	auto thescene = cocos2d::Scene::create();
	auto thelayer = GameLayer::create();
	thescene->addChild(thelayer);
	return thescene;
}

bool GameLayer::init()
{
	do 
	{
		CC_BREAK_IF(!cocos2d::Layer::init());
		init_Create();
		return true;
	} while (0);
	return false;
}

void GameLayer::init_Create()
{
	_enemyCount = enemyTanksCount[_level];
	_leveString = NULL;
	_iplifeString = NULL;
	_enemyCountString = NULL;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blast.plist");
	_listen_TankDeath = EventListenerCustom::create("tankdeath", CC_CALLBACK_1(GameLayer::onEventCustom_TankDeath, this));
	this->_eventDispatcher->addEventListenerWithFixedPriority(_listen_TankDeath, 1);//添加到事件分發器
	_listen_Gameover = EventListenerCustom::create("gameover", CC_CALLBACK_1(GameLayer::onEventCustom_GameOver, this));
	this->_eventDispatcher->addEventListenerWithFixedPriority(_listen_Gameover, 1);//添加到事件分發器
	CreateMapLayer();
	showLevel();
	showEnemy();
	showIpLifeCount();
	this->scheduleUpdate();
	
}



void GameLayer::update(float delta)
{
	_controlLayer->update(delta);
	_enemyAI->update(delta);
	if (_playerTank[0] != NULL)
	{
		_playerTank[0]->update(delta, _controlLayer->getRetation());
	}
	showLevel();
	showEnemy();
	showIpLifeCount();
}

void GameLayer::CreateMapLayer()
{
	//auto backColor1 = LayerColor::create(ccc4(192, 192, 192, 225));
	//this->addChild(backColor1);
	auto backColor = LayerColor::create(ccc4(0, 0, 0, 225));
	this->addChild(backColor);
	//初始化地图信息
	char szRound[260];
	sprintf(szRound, "Round%d.tmx", _level);
	_tileMapInfo = TileMapInfo::createMapInfoWithFile(szRound);
	CCTMXTiledMap* tmxTileMap = _tileMapInfo->getTileMap();
	//this->addChild(tmxTileMap);
	backColor->addChild(tmxTileMap);
	

	//在已有的地图上，创建玩家坦克
	_playerTank[0] = Tank::createTankWithTankType("player2U.png", _tileMapInfo);
	//设置坦克类型为玩家坦克
	_playerTank[0]->setObjType(PlayerTank);
	_playerTank[0]->getBullet();

	//放到地图中初始化位置
	CCSize tileSize = tmxTileMap->getTileSize();
	CCSize mapSize = tmxTileMap->getContentSize();
	_playerTank[0]->setPosition(ccp(mapSize.width / 2 - tileSize.width * 3, tileSize.height));
	_playerTank[0]->setMovedRect(_playerTank[0]->boundingBox());
	_playerTank[0]->setOldMovedRect(_playerTank[0]->boundingBox());
	//创建敌人坦克的AI
	_enemyAI = EnemyAI::createEnemyAIWithTank(_playerTank[0],enemyTanksCount[_level]);

	//添加开火按钮
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto fireButton = MenuItemImage::create(
		"fire_button_default.png",
		"fire_button_press.png",
		CC_CALLBACK_1(GameLayer::menuCallback_PlayerFire, this));

	fireButton->setPosition(Vec2(origin.x + visibleSize.width - fireButton->getContentSize().width / 2,
		origin.y + fireButton->getContentSize().height / 2) + Vec2(-100, 200));

	auto menu = Menu::create(fireButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	//添加遥控杆
	auto controlSprite = Sprite::create("cen.png");
	auto controlbgSprite = Sprite::create("control_bg.png");
	//controlbgSprite->setOpacity(100);
	//controlSprite->setOpacity(100);
	//controlSprite->setScale(1.5f);
	_controlLayer = HRockerLayer::HRockerWithCenter(Vec2(100.0f, 250.0f), 70.0f, controlSprite, controlbgSprite, false);
	this->addChild(_controlLayer);



	//////////////////////////////////////////////////////////////////////////
	auto menuItem01 = cocos2d::MenuItemFont::create("Next", this, menu_selector(GameLayer::menuCallback_NextLevel));
	//menuItem01->setPosition(VisibleRect::center()+Vec2(0,-150));
	menuItem01->setPosition(Vec2(0, 0));
	menuItem01->setFontSize(45);
	menuItem01->setFontNameObj("Marker Felt");

	auto menuItem02 = cocos2d::MenuItemFont::create("P/C", this, menu_selector(GameLayer::menuCallback_Pause));
	menuItem02->setPosition(Vec2(120, 0));
	//menuItem02->setPosition(Vec2(0, 0));
	menuItem02->setFontSize(45);
	menuItem02->setFontNameObj("Marker Felt");

	auto menu2 = cocos2d::Menu::create(menuItem01, menuItem02, NULL);
	menu2->setPosition(VisibleRect::leftTop() + Vec2(menuItem01->getContentSize().width, -menuItem01->getContentSize().height));
	this->addChild(menu2);
}

void GameLayer::menuCallback_PlayerFire(cocos2d::Ref* pSender)
{
	_playerTank[0]->command(cmdFire);
	//_playerTank[0]->tankBoom();
}

void GameLayer::onEventCustom_TankDeath(EventCustom* event)
{
	auto tank = static_cast<Tank*>(event->getUserData());
	if (tank->getObjType() == PlayerTank)
	{
		_playerTank[0] = NULL;
		_enemyAI->mTank = NULL;
		_tileMapInfo->getTileMap()->removeChild(tank);
		_iplife--;
		if (_iplife <= 0)
		{
			GameOver();
		}

		this->scheduleOnce(schedule_selector(GameLayer::createNewPlayer), 1.0f);
		////在已有的地图上，创建玩家坦克
		//_playerTank[0] = Tank::createTankWithTankType("player2U.png", _tileMapInfo);
		////设置坦克类型为玩家坦克
		//_playerTank[0]->setObjType(PlayerTank);
		//_playerTank[0]->getBullet();

		////放到地图中初始化位置
		//CCSize tileSize = _tileMapInfo->getTileMap()->getTileSize();
		//CCSize mapSize = _tileMapInfo->getTileMap()->getContentSize();
		//_playerTank[0]->setPosition(ccp(mapSize.width / 2 - tileSize.width * 3, tileSize.height));
		//_playerTank[0]->setMovedRect(_playerTank[0]->boundingBox());

		//_enemyAI->mTank = _playerTank[0];
	}
	else
	{
		_enemyAI->mEnemyTanks->removeObject(tank);
		_tileMapInfo->getTileMap()->removeChild(tank);
		_enemyCount--;
		if (_enemyCount<=0)
		{
			NextLevel();
		}
	}

}

void GameLayer::createNewPlayer(float delta)
{

	//在已有的地图上，创建玩家坦克
	_playerTank[0] = Tank::createTankWithTankType("player2U.png", _tileMapInfo);
	//设置坦克类型为玩家坦克
	_playerTank[0]->setObjType(PlayerTank);
	_playerTank[0]->getBullet();

	//放到地图中初始化位置
	CCSize tileSize = _tileMapInfo->getTileMap()->getTileSize();
	CCSize mapSize = _tileMapInfo->getTileMap()->getContentSize();
	_playerTank[0]->setPosition(ccp(mapSize.width / 2 - tileSize.width * 3, tileSize.height));
	_playerTank[0]->setMovedRect(_playerTank[0]->boundingBox());

	_enemyAI->mTank = _playerTank[0];
}


void GameLayer::menuCallback_NextLevel(cocos2d::Ref* pSender)
{
	NextLevel();
}

void GameLayer::showLevel()
{
	if (_leveString==NULL)
	{
		_leveString = cocos2d::LabelTTF::create(__String::createWithFormat("level:%d", _level)->getCString(), "Courier-Bold", 40);
		_leveString->setColor(ccc3(255, 255, 255));
		_leveString->setPosition(Vec2(VisibleRect::getVisibleRect().size.width - 150, VisibleRect::getVisibleRect().size.height - 230 * 2) + Vec2(0, 200));
		this->addChild(_leveString, 1);
	}
	else
	{
		_leveString->setString(__String::createWithFormat("level:%d", _level)->getCString());
	}


}

void GameLayer::showIpLifeCount()
{
	if (_iplifeString==NULL)
	{
		_iplifeString = cocos2d::LabelTTF::create(__String::createWithFormat("iplife:%d", _iplife)->getCString(), "Courier-Bold", 40);
		_iplifeString->setColor(ccc3(255, 255, 255));
		_iplifeString->setPosition(Vec2(VisibleRect::getVisibleRect().size.width - 150, VisibleRect::getVisibleRect().size.height - 230 * 2) + Vec2(0, 300));
		this->addChild(_iplifeString, 1);
	}
	else
	{
		_iplifeString->setString(__String::createWithFormat("iplife:%d", _iplife)->getCString());
	}


}

void GameLayer::showEnemy()
{
	if (_enemyCountString==NULL)
	{
		_enemyCountString = cocos2d::LabelTTF::create(__String::createWithFormat("enemy:%d", _enemyCount)->getCString(), "Courier-Bold", 40);
		_enemyCountString->setColor(ccc3(255, 255, 255));
		_enemyCountString->setPosition(Vec2(VisibleRect::getVisibleRect().size.width - 150, VisibleRect::getVisibleRect().size.height - 230 * 2) + Vec2(0, 400));
		this->addChild(_enemyCountString, 1);
	}
	else
	{
		_enemyCountString->setString(__String::createWithFormat("enemy:%d", _enemyCount)->getCString());
	}


}


void GameLayer::menuCallback_Pause(cocos2d::Ref* pSender)
{
	GamePause();
}

void GameLayer::GamePause()
{
	static bool ispause = false;
	if (ispause)
	{
		ispause = false;
		Director::getInstance()->startAnimation();
	}
	else
	{
		ispause = true;
		Director::getInstance()->stopAnimation();
	}
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pause.wav");
}

void GameLayer::GameOver()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("gameover.wav");
	auto theScene = GameOverLayer::createScene();
	//Director::getInstance()->replaceScene(theScene);

	//
	//CCTransitionMoveInR::transitionWithDuration(t, s);//新场景从右移入覆盖  
	Director::getInstance()->replaceScene(TransitionMoveInB::create(2.0f, theScene));

	//CCTransitionFade::transitionWithDuration(t, s)
	//Director::getInstance()->replaceScene(TransitionFade::create(1.5f, theScene));
}

void GameLayer::NextLevel()
{
	_level += 1;
	if (_level > MAX_LEVEL||_iplife<=0)
	{
		_level--;
		GameOver();
	}
	else
	{
		//ClearPreLevel();
		//_enemyTankCount = _gameLevel;
		//initCreate();
		//showLeve();
		//showLife();
		//showEnemyCount();
		//this->scheduleUpdate();
		//////////////////////////////////////////////////////////////////////////
		ClearAllItem();
		//ClearAllItem();
		auto theScene = LevelSwitchLayer::createScene(_level);
		//Director::getInstance()->replaceScene(theScene);

		//
		//CCTransitionMoveInR::transitionWithDuration(t, s);//新场景从右移入覆盖  
		//Director::getInstance()->replaceScene(TransitionMoveInB::create(2.0f, theScene));

		//CCTransitionFade::transitionWithDuration(t, s)
		Director::getInstance()->replaceScene(TransitionFade::create(1.5f, theScene));
		//////////////////////////////////////////////////////////////////////////

		//ClearAllItem();
		//init_Create();
		//this->showLevel();
		//this->showIpLifeCount();
		//this->showEnemy();
	}
}

void GameLayer::ClearAllItem()
{
	this->_eventDispatcher->removeEventListener(_listen_TankDeath);
	this->_eventDispatcher->removeEventListener(_listen_Gameover);

	this->removeAllChildren();
	this->_leveString = NULL;
	this->_enemyCountString = NULL;
	this->_iplifeString = NULL;
	this->unscheduleUpdate();
}

void GameLayer::onEventCustom_GameOver(EventCustom* event)
{
	GameOver();
}
