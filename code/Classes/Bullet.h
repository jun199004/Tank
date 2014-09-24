#ifndef __BULLET__H__
#define __BULLET__H__

#include "cocos2d.h"
using namespace cocos2d;
#include "Tank.h"

class Bullet : public CCSprite
{
public:
	Bullet();
	~Bullet();

	static Bullet* createBulletWithTank(Tank* tank);
	void initBulletWithTank(Tank* tank);
	bool fire();
	virtual void update(float delta);
	void bulletBoom();

public:
	bool isFlying;
	Tank* mTank;
	TileMapInfo* mTileMapInfo;
	float stepX, stepY;

	CC_SYNTHESIZE(CCRect, mMovedRect, MovedRect);
};

#endif