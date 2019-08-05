#include "IntersectTracker.h"
#include "../KDtree/intersect.cpp"

IntersectTracker::IntersectTracker(Game *game) {
	mygame = game;
}

void IntersectTracker::flush() {

	for (unsigned int lvl = 0; lvl < levels.size(); lvl++) {
		listNode<levelIntersect> *head = levels[lvl];
		listNode<levelIntersect> *next;
		while (head) {
			next = head->next;
			delete head->value.model;
			delete head;
			head = next;
		}
	}
	if (snakeHead) {
		delete snakeHead;
		snakeHead = nullptr;
	}
	levelObjSize.clear();
	levels.clear();
}

IntersectTracker::~IntersectTracker() {
	flush();
}

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
	listNode<levelIntersect> *curHead = levels[i];
	toAdd->next = curHead;
	if(curHead)
		curHead->prev = toAdd;
	levels[i] = toAdd;
}

inline void addNodeAfter(listNode<levelIntersect> *node, listNode<levelIntersect> *toAdd) {
	if(node->next)
		node->next->prev = toAdd;
	toAdd->prev = node;
	toAdd->next = node->next;
	node->next = toAdd;
}

void IntersectTracker::addNodeHelper(listNode<levelIntersect> *first, listNode<levelIntersect> *node) {
	if (node->value.x < first->value.x)
		appendNode(node);
	else {
		listNode<levelIntersect> *foundNode = findNode(first, node->value.x);
		addNodeAfter(foundNode, node);
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

void IntersectTracker::printLevel(int lvl) {
	listNode<levelIntersect> *head = levels[lvl];
	printf("[");
	while (head) {
		printf("<%f %f %d> ", head->value.x, head->value.y, head->value.level);
		head = head->next;
	}
	printf("]\n");
}

void IntersectTracker::printLevelR(int lvl) {
	listNode<levelIntersect> *head = levels[lvl];
	listNode<levelIntersect> *prev = head;
	printf("[\n");
	while (head) {
		prev = head;
		printf("<%f %f %d> ", head->value.x, head->value.y, head->value.level);
		head = head->next;
	}
	printf("]\n");
	head = prev;
	printf("R[\n");
	while (head) {
		printf("<%f %f %d> ", head->value.x, head->value.y, head->value.level);
		head = head->prev;
	}
	printf("]\n");
}

void IntersectTracker::printDSDebug() {
	for (unsigned int i = 0; i < levels.size(); i++) {
		printf("%d size %d\n", i, levelObjSize[i]);
		printLevel(i);
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
	newNode->prev = 0;

	/*if (levelObjSize.size() > level) {
		printf("<%f %f %d %d>\n", x, y, level, levelObjSize[level]);
		prinLevel(level);
	}
	else
		printf("<%f %f %d>\n", x, y, level);*/
	return newNode;
}

void IntersectTracker::addObj(float x, float y, int level, Shape *myShape, int type, void *comutedTree) {
	listNode<levelIntersect> *node = genNode(x, y, level, myShape, type);
	intersect *model = new intersect((Node *) comutedTree);
	node->value.model = model;
	addNode(node);
}

void *IntersectTracker::addObj(float x, float y, int level, Shape *myShape, int type, std::vector<glm::vec3> &shape) {
	listNode<levelIntersect> *node = genNode(x, y, level, myShape, type);
	intersect *model = new intersect(shape);
	node->value.model = model;
	addNode(node);
	return (void *) model->getKdNode();
}

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


void IntersectTracker::remove(Shape *s) {
	std::vector<listNode<levelIntersect> *> out;	

	listNode<levelIntersect> * head = chacheShape;

	//printLevelR(chacheLvl);
	//printf("%p %p %d\n", s, head->value.myShape, chacheLvl);

	if (head->prev) {
		head->prev->next = head->next;
		if (head->next)
			head->next->prev = head->prev;
	}
	else {
		levels[chacheLvl] = head->next;
		if(head->next)
			head->next->prev = 0;
	}
	delete head->value.model;
	delete head;

	//printLevelR(chacheLvl);
}

void IntersectTracker::isIntersectSnakeHead(glm::mat4 tranSnake, float x, float y, int level) {
	if ((signed) levelObjSize.size() <= level || levelObjSize[level] == 0)
		return;
	std::vector<listNode<levelIntersect> *> closeObjects = collect(findNode(levels[level], x - radiusLenToCheckIntersects), x, y);
	int dontstop = 0;
	for (unsigned int i = 0; !dontstop & (i < closeObjects.size()); i++) {
		Shape *myShape = closeObjects[i]->value.myShape;
		int type = closeObjects[i]->value.type;
		glm::mat4 myshapetrans = myShape->makeTransScale();
		std::vector<IndexedModel> sol = snakeHead->isIntersect(&tranSnake, &myshapetrans, *closeObjects[i]->value.model);
		if (sol.size() > 0) {
			//printf("sol %d\n", i);
			chacheShape = closeObjects[i];
			chacheLvl = level;
			dontstop = mygame->onIntersectSnakeHead(type, myShape);
		}
	}
}

bool IntersectTracker::exist(int level, Shape *myShape) {
	listNode<levelIntersect> *head = levels[level];

	if (head && (head->value.myShape != myShape))
		head = head->next;
	return head && head->value.myShape == myShape;
}