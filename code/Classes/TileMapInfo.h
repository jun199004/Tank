#pragma once

#include "cocos2d.h"
USING_NS_CC;


//tile类型，草地，钢铁，河流等
enum enumTileType
{
	tileNone, tileGrass,
	tileSteel, tileWall,
	tileRiver, tileKing
};

//根据地图中gid获取对应tile的类型
static enumTileType gidToTileType[] =
{
	tileNone,

	tileNone, tileNone, tileGrass, tileGrass, tileSteel, tileSteel,
	tileNone, tileNone, tileGrass, tileGrass, tileSteel, tileSteel,

	tileWall, tileWall, tileRiver, tileRiver, tileKing, tileKing,
	tileWall, tileWall, tileRiver, tileRiver, tileKing, tileKing,

	tileKing, tileKing, tileNone, tileNone, tileNone, tileNone,
	tileKing, tileKing, tileNone, tileNone, tileNone, tileNone
};

class TileMapInfo
{
public:
	bool collisionTest(CCRect rect);

	static TileMapInfo* createMapInfoWithFile(const char* tmxFile);
	void initMapInfoWithFile(const char* tmxFile);
	CC_SYNTHESIZE(CCTMXTiledMap*, mTMXTileMap, TileMap);

private:
	TMXLayer* mTMXLayers[3];
};

