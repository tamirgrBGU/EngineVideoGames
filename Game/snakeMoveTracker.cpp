#include "snakeMoveTracker.H"

snakeMoveTracker::snakeMoveTracker(int size) {
	nodesLen = size-2;
	for (int i = 0; i < size-1; i++)
		firstVec.push_back(nullptr);
}

listNode<motionTracker> *getLast(listNode<motionTracker> *node) {
	listNode<motionTracker> *prev = node;
	while (node) {
		prev = node;
		node = node->next;
	}
	return prev;
}

inline listNode<motionTracker> *genNode(float x, float y, float z, float angleTurn) {
	listNode<motionTracker> *mTnode = new listNode<motionTracker>();
	mTnode->value.x = x;
	mTnode->value.y = y;
	mTnode->value.z = z;
	mTnode->value.angleTurn = angleTurn;
	mTnode->next = nullptr;
	return mTnode;
}

//primitive in bound check
bool inBound(float x1, float y1, float z1, float x2, float y2, float z2) {
	float diff = abs(x2 - x1);
	diff += abs(y2 - y1);
	diff += abs(z2 - z1);
	return diff < mTepsilon ? true : false;
}

void snakeMoveTracker::add(int Node, listNode<motionTracker> *a) {
	listNode<motionTracker> *mTnode = firstVec[Node];
	if (mTnode) {
		motionTracker *mTval = &(mTnode->value);		motionTracker *mTval2 = &(a->value);
		if (inBound(mTval->x, mTval->y, mTval->z, mTval2->x, mTval2->y, mTval2->z))
			mTval->angleTurn = mTval->angleTurn + mTval2->angleTurn;
		else {
			listNode<motionTracker> *last = getLast(mTnode);
			last->next = a;
			a->next = nullptr;
		}
	}
	else	
		firstVec[Node] = a;	
}

void snakeMoveTracker::add(float x, float y, float z, float angleTurn) {
	add(nodesLen, genNode(x, y, z, angleTurn));
}

// assume it has pointer to valid 
void snakeMoveTracker::transferToNextNode(int Node) {
	listNode<motionTracker> *mTnode = firstVec[Node];
	firstVec[Node] = mTnode->next;
	if (Node > 0)
		add(Node - 1, mTnode);
	else 
		delete mTnode;	
}

float snakeMoveTracker::getAngle(int Node, float x, float y, float z) {
	listNode<motionTracker> *mTnode = firstVec[Node];
	float outangle = 0;
	if (mTnode) {
		motionTracker *mTval = &(mTnode->value);

		if (inBound(mTval->x, mTval->y, mTval->z, x, y, z)) {
			outangle = mTval->angleTurn;
			transferToNextNode(Node);
		}
	}

	return outangle;
}

snakeMoveTracker::~snakeMoveTracker(void) {
	for (int i = 0; i < (signed)firstVec.size(); i++) {
		listNode<motionTracker> *next;
		listNode<motionTracker> *node = firstVec[i];
		while (node) {
			next = node->next;
			delete node;
			node = next;
		}
	}
	firstVec.clear();	firstVec.~vector();
}

// todo check ds
// whenever user not pressing you should move the head back
// with the same func of user moving!