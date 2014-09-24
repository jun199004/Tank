#pragma once

#include "cocos2d.h"
USING_NS_CC;


class HRockerLayer;
class Tank;
class EnemyAI;
class TileMapInfo;

#define  MAX_LEVEL			20

class GameLayer:public cocos2d::Layer
{
public:
	GameLayer();
	~GameLayer();
public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(GameLayer);
	virtual bool init();

	void init_Create();
	void CreateMapLayer();

	void update(float delta);

	void menuCallback_PlayerFire(cocos2d::Ref* pSender);
	void menuCallback_NextLevel(cocos2d::Ref* pSender);
	void menuCallback_Pause(cocos2d::Ref* pSender);

	void onEventCustom_TankDeath(EventCustom* event);
	void onEventCustom_GameOver(EventCustom* event);

	void createNewPlayer(float delta);
	void GamePause();
	void NextLevel();
	void GameOver();
	void ClearAllItem();


	void showLevel();
	void showEnemy();
	void showIpLifeCount();


public:
	HRockerLayer*		_controlLayer;
	Tank*				_playerTank[2];
	EnemyAI*			_enemyAI;
	TileMapInfo*		_tileMapInfo;
	static int					_level;
	int					_enemyCount;
	static int					_iplife;

	cocos2d::LabelTTF *_iplifeString;
	cocos2d::LabelTTF* _leveString;
	cocos2d::LabelTTF* _enemyCountString;

	EventListenerCustom*   _listen_TankDeath;
	EventListenerCustom*   _listen_Gameover;

};

