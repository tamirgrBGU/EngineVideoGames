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
	//we will save the last occurence of the shape we want to remove
	listNode<levelIntersect> *chacheShape;
	int chacheLvl;
public:
	IntersectTracker(Game *game);
	~IntersectTracker();
	void addNodeHelper(listNode<levelIntersect> *first, listNode<levelIntersect> *node);
	void addObj(float x, float y, int level, Shape *myShape, int type, void *comutedTree);
	bool exist(int level, Shape *myShape);
	void *addObj(float x, float y, int level, Shape *myShape, int type, std::vector<glm::vec3> &shape);
	void addSnakeHead(std::vector<glm::vec3> &shape);
	void isIntersectSnakeHead(glm::mat4 tranSnake, float x, float y, int level);
	void remove(Shape *s);
	void printDSDebug();
private:
	inline void appendNode(listNode<levelIntersect> *toAdd);
	void printLevel(int lvl);
	void IntersectTracker::printLevelR(int lvl);
	void addNode(listNode<levelIntersect> *node);
	std::vector<listNode<levelIntersect> *> collect(listNode<levelIntersect> * head, float x, float y);
	listNode<levelIntersect> *findNode(listNode<levelIntersect> *head, float x);
};