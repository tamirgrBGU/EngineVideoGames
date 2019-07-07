#include "IntersectTracker.h"
#include "../KDtree/intersect.cpp"

std::vector<int> levelObjSize;
std::vector<listNode<levelIntersect> *> levels;
listNode<levelIntersect> *findNode(listNode<levelIntersect> *head, float x) {
	listNode<levelIntersect> *prev = head;
	while (head && x > head->value.x) {
		prev = head;
		head = head->next;
	}
	return prev;
}

inline void appendNode(std::vector<listNode<levelIntersect> *> levels, int i, listNode<levelIntersect> *toAdd) {
	toAdd->next = levels[i];
	levels[i] = toAdd;
}

inline void addNodeAfter(listNode<levelIntersect> *node, listNode<levelIntersect> *toAdd) {
	toAdd->next = node->next;
	node->next = toAdd;
}

void addNode(listNode<levelIntersect> *node) {
	unsigned int lvl = node->value.level;
	while (lvl >= levels.size()) {//levels are positive, starting from zero
		levels.push_back(0);
		levelObjSize.push_back(0);
	}

	if (levelObjSize[lvl] == 0)
		levels[lvl] = node;
	else {
		listNode<levelIntersect> *foundNode = findNode(levels[lvl], node->value.x);
		if (node->value.x < foundNode->value.x)
			appendNode(
				levels,
				lvl,
				node
			);
		else
			addNodeAfter(
				foundNode,
				node
			);
	}

	levelObjSize[lvl]++;
}

void printFreinds(int lvl) {
	listNode<levelIntersect> *head = levels[lvl];
	printf("[");
	while (head) {
		printf("<%f %f %d> ", head->value.x, head->value.y, head->value.level);
		head = head->next;
	}
	printf("]\n");
}


void printFreinds() {
	for (unsigned int i = 0; i < levels.size(); i++) {
		printf("%d size %d\n", i, levelObjSize[i]);
		printFreinds(i);
	}
}

void addObj(float x, float y, int level, Shape *myShape, void(*onIntersect)(void), std::vector<glm::vec3> &shape) {
	listNode<levelIntersect> *newFriend = new listNode<levelIntersect>();
	newFriend->value.x = x;
	newFriend->value.y = y;
	newFriend->value.level = level;
	newFriend->value.myShape = myShape;
	newFriend->value.onIntersect = onIntersect;
	intersect *model = new intersect(shape);
	newFriend->value.model = model;
	newFriend->next = 0;
	addNode(newFriend);
}

intersect *snakeHead;
void addSnakeHead(std::vector<glm::vec3> &shape) {
	snakeHead = new intersect(shape);
}

//collect object around the snake (they are very close to the snake)
std::vector<listNode<levelIntersect> *> collect(listNode<levelIntersect> * head, float x, float y) {
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

void isIntersectSnakeHead(glm::mat4 tranSnake, float x, float y, int level) {
	std::vector<listNode<levelIntersect> *> closeObjects = collect(findNode(levels[level], x - radiusLenToCheckIntersects), x, y);
	//printf("size %d\n", closeObjects.size());
	for (unsigned int i = 0; i < closeObjects.size(); i++) {
		glm::mat4 myshapetrans = closeObjects[i]->value.myShape->makeTrans();
		std::vector<IndexedModel> sol = snakeHead->isIntersect(&tranSnake, &myshapetrans, *closeObjects[i]->value.model);
		if (sol.size() > 0)
			closeObjects[i]->value.onIntersect();
	}
}
