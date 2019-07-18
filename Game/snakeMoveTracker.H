#pragma once

#include <vector>
#include "listNode.h"

static float mTepsilon = 1;
struct motionTracker {
	float x;
	float y;
	float z;
	float angleTurn;
};

class snakeMoveTracker
{

public:
	snakeMoveTracker(int size);
	void add(float x, float y, float z, float angle);
	float  getAngle(int Node, float x, float y, float z);
	~snakeMoveTracker(void);

private:
	void  transferToNextNode(int Node);
	void add(int Node, listNode<motionTracker> *a);
	int nodesLen;
	std::vector<listNode<motionTracker> *> firstVec;
};