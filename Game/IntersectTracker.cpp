#include "IntersectTracker.h"
#include "../KDtree/intersect.cpp"

IntersectTracker::IntersectTracker(Game *game) {
	mygame = game;
}
IntersectTracker::~IntersectTracker() {}

//return the node that is little then X but next is greater or null
listNode<levelIntersect> *IntersectTracker::findNode(listNode<levelIntersect> *head, float x) {
	listNode<levelIntersect> *prev = head;
	while (head && x > head->value.x) {
		prev = head;
		head = head->next;
	}
	return prev;
}

inline void IntersectTracker::appendNode(listNode<levelIntersect> *toAdd) {
	int i = toAdd->value.level;
	toAdd->next = levels[i];
	levels[i] = toAdd;
}

inline void addNodeAfter(listNode<levelIntersect> *node, listNode<levelIntersect> *toAdd) {
	toAdd->next = node->next;
	node->next = toAdd;
}

void IntersectTracker::addNodeHelper(listNode<levelIntersect> *first, listNode<levelIntersect> *node) {
	if (node->value.x < first->value.x)
		appendNode(node);
	else {
		listNode<levelIntersect> *foundNode = findNode(first, node->value.x);
		addNodeAfter(foundNode,	node);
	}
}

void IntersectTracker::addNode(listNode<levelIntersect> *node) {
	unsigned int lvl = node->value.level;
	while (lvl >= levels.size()) {//levels are positive, starting from zero
		levels.push_back(0);
		levelObjSize.push_back(0);
	}

	if (levelObjSize[lvl] == 0)
		levels[lvl] = node;
	else
		addNodeHelper(levels[lvl], node);

	levelObjSize[lvl]++;
}

void IntersectTracker::prinLevel(int lvl) {
	listNode<levelIntersect> *head = levels[lvl];
	printf("[");
	while (head) {
		printf("<%f %f %d> ", head->value.x, head->value.y, head->value.level);
		head = head->next;
	}
	printf("]\n");
}

void IntersectTracker::printDSDebug() {
	for (unsigned int i = 0; i < levels.size(); i++) {
		printf("%d size %d\n", i, levelObjSize[i]);
		prinLevel(i);
	}
}

inline listNode<levelIntersect> * genNode(float x, float y, int level, Shape *myShape, int type) {
	listNode<levelIntersect> *newNode = new listNode<levelIntersect>();
	newNode->value.x = x;
	newNode->value.y = y;
	newNode->value.level = level;
	newNode->value.myShape = myShape;
	newNode->value.type = type;
	newNode->next = 0;

	/*if (levelObjSize.size() > level) {
		printf("<%f %f %d %d>\n", x, y, level, levelObjSize[level]);
		prinLevel(level);
	}
	else
		printf("<%f %f %d>\n", x, y, level);*/
	return newNode;
}

void IntersectTracker::addObj(float x, float y, int level, Shape *myShape, int type, intersect *comutedTree) {
	listNode<levelIntersect> *node = genNode(x, y, level, myShape, type);
	intersect *model = new intersect(*comutedTree);
	node->value.model = model;
	addNode(node);
}

intersect *IntersectTracker::addObj(float x, float y, int level, Shape *myShape, int type, std::vector<glm::vec3> &shape) {
	listNode<levelIntersect> *node = genNode(x, y, level, myShape, type);
	intersect *model = new intersect(shape);
	node->value.model = model;
	addNode(node);
	return node->value.model;
}

intersect *snakeHead;
void IntersectTracker::addSnakeHead(std::vector<glm::vec3> &shape) {
	snakeHead = new intersect(shape);
}

//collect object around the snake (they are very close to the snake)
std::vector<listNode<levelIntersect> *> IntersectTracker::collect(listNode<levelIntersect> * head, float x, float y) {
	std::vector<listNode<levelIntersect> *> out;

	if (head && (head->value.x < x - radiusLenToCheckIntersects))
		head = head->next;//find gives us a node that smaller then the left boundary

	while (head && (x - radiusLenToCheckIntersects <= head->value.x) & (head->value.x <= x + radiusLenToCheckIntersects)) {
		//printf("%f %f\n", head->value.x, head->value.y);
		if ((y - radiusLenToCheckIntersects <= head->value.y) & (head->value.y <= y + radiusLenToCheckIntersects))
			out.push_back(head);
		head = head->next;
	}

	return out;
}

void IntersectTracker::isIntersectSnakeHead(glm::mat4 tranSnake, float x, float y, int level) {
	if ((signed) levelObjSize.size() <= level || levelObjSize[level] == 0)
		return;
	std::vector<listNode<levelIntersect> *> closeObjects = collect(findNode(levels[level], x - radiusLenToCheckIntersects), x, y);
	//printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	//printf("<%f, %f> %d size %d", x, y, level, closeObjects.size());
	for (unsigned int i = 0; i < closeObjects.size(); i++) {
		Shape *myShape = closeObjects[i]->value.myShape;
		int type = closeObjects[i]->value.type;
		glm::mat4 myshapetrans = myShape->makeTransScale();
		std::vector<IndexedModel> sol = snakeHead->isIntersect(&tranSnake, &myshapetrans, *closeObjects[i]->value.model);
		if (sol.size() > 0)
			mygame->onIntersectSnakeHead(type, myShape);
	}
}
