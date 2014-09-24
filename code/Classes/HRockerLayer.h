#pragma once

#include "cocos2d.h"
#include "CommonValue.h"
USING_NS_CC;

typedef enum _direction
{
	CONTROL_DIRECT_NULL=0,
	CONTROL_DIRECT_UP = 1,
	CONTROL_DIRECT_DOWN = 2,
	CONTROL_DIRECT_LEFT = 3,
	CONTROL_DIRECT_RIGHT = 4,
} CONTROL_DIRECTION;

class HRockerLayer :public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(HRockerLayer);

	static HRockerLayer* HRockerWithCenter(Vec2 aPoint, float aRadius, Sprite* aJsSprite,Sprite* aJsBg,bool _isFollowRole);
	void Active();
	void Inactive();
	enumOrder getRetation();
	void  update(float delta);
private:
	float getVelocity();
	Vec2 getDirection();
	enumOrder CalculateRetation();
	
private:
	Vec2  _centerPoint;
	Vec2	_currentPoint;
	bool	_active;
	float		_radius;
	Sprite*		jsSprite;
	bool isFollowRole;
	enumOrder	_order;
private:
	
	HRockerLayer* initWithCenter(Vec2 aPoint, float aRadius, Sprite* aJsSprite, Sprite* aJsBg, bool _isFollowRole);
	void AddEventTouch();
	bool TouchBegan(Touch *pTouch, Event *pEvent);
	void TouchMoved(Touch *pTouch, Event *pEvent);
	void TouchEnded(Touch *pTouch, Event *pEvent);
	


};