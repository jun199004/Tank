#include "Bullet.h"

Bullet::Bullet() :isFlying(false)
{
}

Bullet::~Bullet()
{

}

Bullet* Bullet::createBulletWithTank(Tank* tank)
{
	Bullet* bullet = new Bullet();
	bullet->setScale(0.75f);
	bullet->initBulletWithTank(tank);
	bullet->autorelease();

	return bullet;
}

void Bullet::initBulletWithTank(Tank* tank)
{
	mTank = tank;
	mTileMapInfo = mTank->getTileMapInfo();

	initWithSpriteFrameName("bullet.png");
	mTileMapInfo->getTileMap()->addChild(this,-1);
	setVisible(false);
}

bool Bullet::fire()
{
	if (!isFlying)
	{
		if (mTank->getObjType() == PlayerTank)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shoot.wav");
		}
		isFlying = true;
		setVisible(true);
		setPosition(mTank->getPosition());
		//�����ӵ����з���
		int Rotation = mTank->getRotation();
		switch (Rotation)
		{
		case 0:
			stepX = 0.0f, stepY = 2.0f;
			break;
		case 90:
			stepX = 2.0f, stepY = 0.0f;
			break;
		case 180:
			stepX = 0.0f, stepY = -2.0f;
			break;
		case 270:
			stepX = -2.0f, stepY = 0.0f;
			break;
		default:
			break;
		}
		scheduleUpdate();
	}

	return isFlying;
}

void Bullet::bulletBoom()
{

	unscheduleUpdate();
	setVisible(false);
	isFlying = false;
	CCRect rect = boundingBox();
	CCSize mapSize = mTileMapInfo->getTileMap()->getContentSize();

	if (rect.getMinX() < 0 || rect.getMaxX() >= mapSize.width ||
		rect.getMinY() < 0 || rect.getMaxY() >= mapSize.height)
		return ;

	CCTMXLayer* tmxLayer = mTileMapInfo->getTileMap()->layerNamed("layer_0");
	CCSize tileSize = tmxLayer->getMapTileSize();

	//����Y��λtmx��ͼ�е�Y��
	float MinY = mapSize.height - rect.getMinY();
	float MaxY = mapSize.height - rect.getMaxY();

	//�����ӵ���ײ��ǽ��tileWallͼ��ɾ��
	CCPoint pt = ccp((int)rect.getMinX() / tileSize.width,(int)(MinY / tileSize.height));
	if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileWall)
		tmxLayer->setTileGID(gidToTileType[tileNone], pt);
	else if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileKing)
	{
		//mTileMapInfo->getTileMap()->layerNamed("layer_2")->setVisible(true);
		this->_eventDispatcher->dispatchCustomEvent("gameover", (void*)this);
	}

	pt = ccp((int)rect.getMinX() / tileSize.width,(int)(MaxY / tileSize.height));
	if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileWall)
		tmxLayer->setTileGID(gidToTileType[tileNone], pt);
	else if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileKing)
	{
		//mTileMapInfo->getTileMap()->layerNamed("layer_2")->setVisible(true);
		this->_eventDispatcher->dispatchCustomEvent("gameover", (void*)this);
	}

	pt = ccp((int)rect.getMaxX() / tileSize.width,(int)(MinY / tileSize.height));
	if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileWall)
		tmxLayer->setTileGID(gidToTileType[tileNone], pt);
	else if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileKing)
	{
		//mTileMapInfo->getTileMap()->layerNamed("layer_2")->setVisible(true);
		this->_eventDispatcher->dispatchCustomEvent("gameover", (void*)this);
	}

	pt = ccp((int)rect.getMaxX() / tileSize.width,(int)(MaxY / tileSize.height));
	if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileWall)
		tmxLayer->setTileGID(gidToTileType[tileNone], pt);
	else if (gidToTileType[tmxLayer->tileGIDAt(pt)] == tileKing)
	{
		//mTileMapInfo->getTileMap()->layerNamed("layer_2")->setVisible(true);
		this->_eventDispatcher->dispatchCustomEvent("gameover", (void*)this);
	}
}

void Bullet::update(float delta)
{
	CCSprite::update(delta);
	//�����ƶ����λ��
	setPosition(ccp(getPositionX() + stepX, getPositionY() + stepY));

	//����Ƿ�����ײ
	CCRect rect = boundingBox();
	if (mTileMapInfo->collisionTest(rect))
	{
		//unscheduleUpdate();
		//setVisible(false);
		//isFlying = false;
		//�����ӵ�
		bulletBoom();
	}
}

