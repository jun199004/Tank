#include "Tank.h"
#include "Bullet.h"

Tank::Tank() :mBulletDelta(0.0), IsBlock(false)
{

}

Tank::~Tank()
{

}

Tank* Tank::createTankWithTankType(const char* tankTypeName, TileMapInfo* tileMapInfo)
{
	CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	pCache->addSpriteFramesWithFile("tank.plist");
	initActionSet();

	Tank* tank = new Tank();
	tank->initTankWithTankType(tankTypeName, tileMapInfo);
	tank->autorelease();

	return tank;
}

void Tank::initTankWithTankType(const char* tankTypeName, TileMapInfo* tileMapInfo)
{
	initWithSpriteFrameName(tankTypeName);
	mTileMapInfo = tileMapInfo;

	//将坦克放入地图层中
	mTileMapInfo->getTileMap()->addChild(this);

	//缩放到合适大小
	CCTMXTiledMap* tmxTileMap = mTileMapInfo->getTileMap();
	CCSize tileSize = tmxTileMap->getTileSize();
	CCSize tankSize = getContentSize();
	//比地图上砖块小一点
	setScale((tileSize.height * 2-4) / (tankSize.height));

	//初始化坦克的子弹
	mBullet = Bullet::createBulletWithTank(this);

	//设置放缩大小
	setScale(0.4f);
}

bool Tank::command(enumOrder order)
{
	float stepX = 0.0f;
	float stepY = 0.0f;
	float fRotation = getRotation();

	switch (order)
	{
	case cmdNothing:
		break;
	case cmdGoUP:
		stepY = 1.0f;
		fRotation = 0.0f;
		break;
	case cmdGoDown:
		stepY = -1.0f;
		fRotation = 180.0f;
		break;
	case cmdGoLeft:
		stepX = -1.0f;
		fRotation = 270.0f;
		break;
	case cmdGoRight:
		stepX = 1.0f;
		fRotation = 90.0f;
		break;
	case cmdFire:
		//调用子弹开火
		return mBullet->fire();
	default:
		break;
	}

	//根据运行方向旋转坦克
	setRotation(fRotation);

	CCRect rect = this->boundingBox();
	mMovedRect = CCRectMake(rect.getMinX() + stepX,
		rect.getMinY() + stepY, rect.size.width, rect.size.height);
	//检测地图上的碰撞
	if (!mTileMapInfo->collisionTest(mMovedRect))
	{
		IsBlock = false;
		return true;
	}
	//如果碰撞了就不要移动,设置为阻塞状态
	mMovedRect = rect;
	IsBlock = true;

	return false;
}

void Tank::move()
{
	if (!IsBlock)
	{
		if (!isSameRect(mOldMovedRect,mMovedRect))
		{
			if (mObjType == PlayerTank)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("moving.wav");
			}
			setPosition(ccp(mMovedRect.getMidX(), mMovedRect.getMidY()));
			mOldMovedRect = mMovedRect;
		}
		
	}
}

void Tank::update(float delta, enumOrder order)
{
	command(order);
	move();
}

void Tank::tankBoom()
{
	if (mObjType==PlayerTank)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("fexplosion.wav");
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("eexplosion.wav");
	}
	mBulletDelta = -99999.0f;
	auto animation = AnimationCache::getInstance()->animationByName("tankboom");
	auto action = Animate::create(animation);
	auto repeatdone = CCCallFuncN::create(this, callfuncN_selector(Tank::callback_boomDone));
	auto sequence = CCSequence::create(action, repeatdone, NULL);
	this->runAction(sequence);
}

void Tank::callback_boomDone(Node* pNode)
{
	//this->getParent()->removeChild(this);
	this->_eventDispatcher->dispatchCustomEvent("tankdeath", (void*)this);
}

void Tank::initActionSet()
{
	
	auto frameCache = SpriteFrameCache::getInstance();
	SpriteFrame* frame = NULL;
	//3.0中改用vector 而不是用Array
	Vector<SpriteFrame*>frameVector;

	/* 1.----------------加载跑动的Animation-----------------*/
	for (int i = 1; i <= 8; i++) {
		//从缓存池中加载精灵到Vector
		frame = frameCache->spriteFrameByName(String::createWithFormat("blast%d.gif", i)->getCString());
		frameVector.pushBack(frame);
	}
	//CCAnimate* animate = CCAnimate::create(animation);
	//CCRepeat* repeat = CCRepeat::create(animate, 2);
	//CCCallFuncN* repeatdone = CCCallFuncN::create(this, callfuncN_selector(WelcomeLayer::loadingDone));
	//CCSequence* sequence = CCSequence::create(repeat, repeatdone, NULL);

	//用vector里面的SpriteFrame列表创建Animation  以及设置一些参数
	auto run_animation = Animation::createWithSpriteFrames(frameVector, 0.05f, 1);
	auto animate = CCAnimate::create(run_animation);
	//将跑动的 Animation 取名为 running 
	AnimationCache::getInstance()->addAnimation(run_animation, "tankboom");

	///*4---------------*/
	//frameVector.clear();
	//for (int i = 1; i <= 3; i++){
	//	frame = frameCache->spriteFrameByName(String::createWithFormat("explode-%d.png", i)->getCString());
	//	frameVector.pushBack(frame);
	//}
	//auto jumpUp_animation = Animation::createWithSpriteFrames(frameVector, 0.2);//不设置无限循环
	//AnimationCache::getInstance()->addAnimation(jumpUp_animation, "enemyboom");
}

bool Tank::isSameRect(Rect rectA, Rect rectB)
{
	if (rectA.getMaxX() != rectB.getMaxX()) return false;
	if (rectA.getMaxY() != rectB.getMaxY()) return false;
	if (rectA.getMinX() != rectB.getMinX()) return false;
	if (rectA.getMinY() != rectB.getMinY()) return false;

	return true;
}
