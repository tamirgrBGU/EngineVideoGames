#pragma once
#include "../KDtree/intersect.h"
#include "levelParser.h"
#include "shape.h"
#include "Game.h"
#include "listNode.h"

struct levelIntersect {
	float x;
	float y;
	int level;
	Shape *myShape;
	int type;
	intersect *model;
};

static const float radiusLenToCheckIntersects = 1.5f*allscale;
class IntersectTracker
{
	Game *mygame;
	std::vector<int> levelObjSize;
	std::vector<listNode<levelIntersect> *> levels;
public:
	IntersectTracker(Game *game);
	~IntersectTracker();
	void addNodeHelper(listNode<levelIntersect> *first, listNode<levelIntersect> *node);
	void addObj(float x, float y, int level, Shape *myShape, int type, intersect *comutedTree);
	intersect *addObj(float x, float y, int level, Shape *myShape, int type, std::vector<glm::vec3> &shape);
	void addSnakeHead(std::vector<glm::vec3> &shape);
	void isIntersectSnakeHead(glm::mat4 tranSnake, float x, float y, int level);
	void printDSDebug();
private:
	inline void appendNode(listNode<levelIntersect> *toAdd);
	void prinLevel(int lvl);
	void addNode(listNode<levelIntersect> *node);
	std::vector<listNode<levelIntersect> *> collect(listNode<levelIntersect> * head, float x, float y);
	listNode<levelIntersect> *findNode(listNode<levelIntersect> *head, float x);
};