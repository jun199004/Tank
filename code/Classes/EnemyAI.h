#ifndef __ENEMY__AI__H__
#define __ENEMY__AI__H__

#include "cocos2d.h"
using namespace cocos2d;
#include "Tank.h"

static const char* enemyTankType[] = 
{
	"normalU.png", "speedU.png", "armor1U.png"
};

class EnemyAI
{
public:
	EnemyAI();
	~EnemyAI();

	static EnemyAI* createEnemyAIWithTank(Tank* tank,int maxenemytank=0);
	void initEnemyAIWithTank(Tank* tank);
	void update(float delta);

	void collisionTest();
	bool IsRectIntersect(CCRect rectA, CCRect rectB);


private:
	void addTank(float delta);
	void tankAction(float delta);

public:
	CCArray* mEnemyTanks;
	Tank* mTank;
	TileMapInfo* mTileMapInfo;
	int			 mEnemyTankCounts;

	//出现地点
	CCPoint bornPoint[3];
};

#endif