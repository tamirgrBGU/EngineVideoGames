#pragma once

#include <vector>
#include <glm\vec3.hpp>
#include "listNode.h"

static float mTepsilon = 15;
static int baseTicksForSpeed1 = 5;
struct motionTracker {
	int ticks;
	glm::vec3 rotationAxis;
	float angleTurn;
};

class snakeMoveTracker
{

int baseTicks;
public:
	snakeMoveTracker(int size, float speed);
	void add(glm::vec3 rotationAxis, float angle);
	motionTracker getAngleAndAxis(int Node);
	void flush();
	float getSumOfAllAngles(int Node);
	void printDS();//for debugging
	~snakeMoveTracker(void);

private:
	void transferToNextNode(int Node);
	void add(int Node, listNode<motionTracker> *a);
	int nodesLen;
	std::vector<listNode<motionTracker> *> firstVec;
};