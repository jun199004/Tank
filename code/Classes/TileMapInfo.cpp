#include "TileMapInfo.h"
#include "VisibleRect.h"





bool TileMapInfo::collisionTest(CCRect rect)
{
	int gid = 0;
	CCSize mapSize = mTMXTileMap->getContentSize();
	CCSize tileSize = mTMXTileMap->getTileSize();

	//超出地图外面直接返回
	if (rect.getMinX() <= 0.1 || rect.getMaxX() >= mapSize.width ||
		rect.getMinY() <= 0.1 || rect.getMaxY() >= mapSize.height)
		return true;

	//将坦克Y坐标转换为地图上的Y坐标
	float MinY = mapSize.height - rect.getMinY();
	float MaxY = mapSize.height - rect.getMaxY();
	//对坦克四个顶点进行碰撞检测
	gid = mTMXLayers[0]->tileGIDAt(ccp((int)(rect.getMinX() / tileSize.width),
		(int)(MinY / tileSize.height)));
	if (gidToTileType[gid] != tileNone && gidToTileType[gid] != tileGrass)
		return true;
	gid = mTMXLayers[0]->tileGIDAt(ccp((int)(rect.getMinX() / tileSize.width),
		(int)(MaxY / tileSize.height)));
	if (gidToTileType[gid] != tileNone && gidToTileType[gid] != tileGrass)
		return true;
	gid = mTMXLayers[0]->tileGIDAt(ccp((int)(rect.getMaxX() / tileSize.width),
		(int)(MaxY / tileSize.height)));
	if (gidToTileType[gid] != tileNone && gidToTileType[gid] != tileGrass)
		return true;
	gid = mTMXLayers[0]->tileGIDAt(ccp((int)(rect.getMaxX() / tileSize.width),
		(int)(MinY / tileSize.height)));
	if (gidToTileType[gid] != tileNone && gidToTileType[gid] != tileGrass)
		return true;

	return false;
}

TileMapInfo* TileMapInfo::createMapInfoWithFile(const char* tmxFile)
{
	TileMapInfo* tileMapInfo = new TileMapInfo();
	tileMapInfo->initMapInfoWithFile(tmxFile);
	return tileMapInfo;
}

void TileMapInfo::initMapInfoWithFile(const char* tmxFile)
{
	mTMXTileMap = TMXTiledMap::create(tmxFile);
	mTMXLayers[0] = mTMXTileMap->layerNamed("layer_0");
	mTMXLayers[1] = mTMXTileMap->layerNamed("layer_1");
	//mTMXLayers[2] = mTMXTileMap->layerNamed("layer_2");
	//mTMXLayers[2]->setVisible(false);


	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize mapSize = mTMXTileMap->getContentSize();

	//auto scale = VisibleRect::getVisibleRect().size.height / _gameMap->getContentSize().height;
	//缩放地图到合适屏幕大小
	mTMXTileMap->setScale(winSize.height / mTMXTileMap->getContentSize().height);

	//将地图放到屏幕中间
	mTMXTileMap->setPosition(ccp((winSize.width - mapSize.width * mTMXTileMap->getScale()) / 2,(winSize.height - mapSize.height * mTMXTileMap->getScale()) / 2));
}


