#pragma once
#include "bezier1D.h"

static const char * levelDirName = "levels";
static const float allscale = 200;
static const float zscale = allscale/3;

struct objLocation {
	float x;
	float y;
	float z;
	int level;
	int direction;
	int type;
};

struct objConnected {
	struct objLocation me;
	struct objConnected * left;
	struct objConnected * right;
	struct objConnected * up;
	struct objConnected * down;
};

struct modelWrapper {
	float x;
	float y;
	float z;
	int level;
	IndexedModel model;
};

struct objMap {
	std::vector<modelWrapper>* levelGround;
	std::vector<modelWrapper>* stairs;
	std::vector<modelWrapper>* walls;
	std::vector<struct objLocation>* specialObj;
};

class leveGenerator
{
private:
	std::vector<modelWrapper>* levelGround;
	std::vector<modelWrapper>* stairs;
	std::vector<modelWrapper>* stairsWalls;
	std::vector<modelWrapper>* walls;
	std::vector<modelWrapper>* fallWalls;
	std::vector<std::string*>* levelName;
	std::vector<struct objLocation>* specialObj;
	int parseLevel(int i);
	int currentLevel;
	void init(const char *);
public:
	struct objMap getLevel(int i);
	leveGenerator(int i);
	~leveGenerator(void);
};
