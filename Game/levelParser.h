#pragma once
#include "bezier1D.h"

static const char * levelDirName = "levels";
static const float allscale = 50;
static const float zscale = 35;

struct objLocation {
	float x;
	float y;
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

struct objMap {
	std::vector<IndexedModel>* levelGround;
	std::vector<IndexedModel>* stairs;
	std::vector<IndexedModel>* walls;
	std::vector<struct objLocation>* specialObj;
};

class leveGenerator
{
private:
	std::vector<IndexedModel>* levelGround;
	std::vector<IndexedModel>* stairs;
	std::vector<IndexedModel>* walls;
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

