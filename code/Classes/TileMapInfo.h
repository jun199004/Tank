#pragma once

#include "cocos2d.h"
USING_NS_CC;


//tile���ͣ��ݵأ�������������
enum enumTileType
{
	tileNone, tileGrass,
	tileSteel, tileWall,
	tileRiver, tileKing
};

//���ݵ�ͼ��gid��ȡ��Ӧtile������
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

