#pragma once

#include <vector>
#include "listNode.h"

static float mTepsilon = 15;
static int baseTicks = 9;
struct motionTracker {
	int ticks;
	float angleTurn;
};

class snakeMoveTracker
{

public:
	snakeMoveTracker(int size);
	void add(float angle);
	float getAngle(int Node);
	float getSumOfAllAngles(int Node);
	void printDS();//for debugging
	~snakeMoveTracker(void);

private:
	void  transferToNextNode(int Node);
	void add(int Node, listNode<motionTracker> *a);
	int nodesLen;
	std::vector<listNode<motionTracker> *> firstVec;
};