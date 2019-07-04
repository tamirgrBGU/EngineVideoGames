#pragma once
#include "../KDtree/intersect.h"

template<class T> struct listNode
{
	T value;
	listNode<T> *next;
};

struct levelIntersect {
	float x;
	float y;
	int level;
	void(*onIntersect)(void);
	intersect *model;
};

static const int radiusLenToCheckIntersects = 200;
void addObj(float x, float y, int level, void(*onIntersect)(void), std::vector<glm::vec3> shape);
void addSnakeHead(std::vector<glm::vec3> shape);
void isIntersectSnakeHead(glm::mat4 tranSnake, float x, float y, int level);