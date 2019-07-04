#include "IntersectTracker.h"

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

void addNodeAfter(listNode<levelIntersect> *node, listNode<levelIntersect> *toAdd) {
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
	else
		addNodeAfter(
			findNode(levels[lvl], node->value.x),
			node
		);

	levelObjSize[lvl]++;
}

void addObj(float x, float y, int level, void(*onIntersect)(void), std::vector<glm::vec3> shape) {
	listNode<levelIntersect> *newFriend = new listNode<levelIntersect>();
	newFriend->value.x = x;
	newFriend->value.y = y;
	newFriend->value.level = level;
	newFriend->value.onIntersect = onIntersect;
	intersect *model = new intersect(shape);
	newFriend->value.model = model;
	newFriend->next = 0;
	addNode(newFriend);
}

intersect *snakeHead;
void addSnakeHead(std::vector<glm::vec3> shape) {
	snakeHead = new intersect(shape);
}

//collect object around the snake (they are very close to the snake)
std::vector<listNode<levelIntersect> *> collect(listNode<levelIntersect> * head, float x, float y) {
	std::vector<listNode<levelIntersect> *> out;
	while (head && (x - radiusLenToCheckIntersects <= head->value.x) & (x + radiusLenToCheckIntersects >= head->value.x)) {
		if ((y - radiusLenToCheckIntersects <= head->value.y) & (y + radiusLenToCheckIntersects >= head->value.y))
			out.push_back(head);
	}
	return out;
}

glm::mat4 I(1);
void isIntersectSnakeHead(glm::mat4 tranSnake, float x, float y, int level) {
	std::vector<listNode<levelIntersect> *> closeObjects = collect(findNode(levels[level], x - radiusLenToCheckIntersects), x, y);
	for (unsigned int i = 0; i < closeObjects.size(); i++) {
		std::vector<IndexedModel> sol = snakeHead->isIntersect(&tranSnake, &I, *closeObjects[i]->value.model);
		if (sol.size() > 0)
			closeObjects[i]->value.onIntersect();
	}
}
