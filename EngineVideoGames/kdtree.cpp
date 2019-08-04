/**
*
* k-d tree implementation file
*	
* author: Arvind Rao
* license: GNU
* 
*
*/
#include "kdtree.h"
#include <iostream>

// implementation of Node Class

Node::Node(int _n) { left=0; right=0;}
Node::Node(int _n, vecType _data) : data(_data){left = 0; right = 0;}

Node::~Node(){}


// implementation of kdtree

Kdtree::Kdtree(){}
Kdtree::Kdtree(Node *outsider) { Kdtree::root = outsider; }
Kdtree::~Kdtree(){/*if(root) kiil(root)*/}

void killHelper(Node* head) {
	if (head) {
		killHelper(head->left);
		killHelper(head->right);

		delete head;
	}
}

void Kdtree::kill(Node* head) {

	killHelper(head->left);
	killHelper(head->right);

	delete head;
}
/*
*
*
*/
Kdtree::vecType Kdtree::findMedian(std::list<Kdtree::vecType> &plist, std::list<Kdtree::vecType> &left, 
							std::list<Kdtree::vecType> &right )
{
	Kdtree::vecType median;
	int size = plist.size();
	int med = int(ceil( float(size) / float(2) ));
	int count = 0;
	
	if ( size == 1)
		return plist.front();
	
	// Using lambda function here, to define comparison function--parametrized by 'axis'
	//plist.sort( [&](Kdtree::vecType& a, Kdtree::vecType& b){return a[axis] < b[axis];});
	
	for ( auto& x : plist )
	{
		if (count < med)
			left.push_back(x);
		else
			right.push_back(x);
		++count;
	}
	median = left.back();	//left.pop_back();
	return median;
}

void Kdtree::print_data(vecType pt)
{
	for( int i = 0; i < N; i++)
	{
	  std::cout<< pt[i] << ", ";
	}
	std::cout<<"\n";
}

/*
*
*
*/
void Kdtree::printTree( Node* head )
{
	int depth = 0;
	
	std::cout<< "\n**** Print of Tree **********\n";
	std::queue< Node* > current, next; 
	Node * temp = head;
	current.push(temp);

	while( !current.empty() )
	{
		temp = current.front();
		current.pop();

		if (temp == nullptr)
			std::cout<<"NULL\n";
		else
		{
			Kdtree::print_data(temp->data);
			next.push(temp->left);
			next.push(temp->right);
		}
		if(current.empty())
		{
			depth++;
			std::cout<< "level: "<< depth <<"\n";
			std::swap(current, next);
		}
	}
}

/*
* algorithm is based on http://en.wikipedia.org/wiki/Kd_tree
*/
void Kdtree::makeTree(std::vector<glm::vec3>& pvec)
{
	std::list<Kdtree::vecType> point_list;
	glm::vec3 v;
	vecType v4;
	for (unsigned int i = 0; i < pvec.size(); i++)
	{
		v = pvec[i];
		v4 = glm::vec4(v.x, v.y, v.z, 1);
		point_list.push_back(v4);
	}
	Kdtree::makeTree(point_list);
}

void cloneList(std::list<Kdtree::vecType>& plist, std::list<Kdtree::vecType>& writeon) {
	for (Kdtree::vecType& x : plist)
		writeon.push_back(x);
}

void cloneByMed(int axis, float med, std::list<Kdtree::vecType> &plist,
	std::list<Kdtree::vecType> &left,	std::list<Kdtree::vecType> &right)
{
	bool flipflop = true;
	for (Kdtree::vecType x : plist)
	{
		if (x[axis] < med)
			left.push_back(x);
		else if (x[axis] == med) {
			if (flipflop)
				left.push_back(x);
			else
				right.push_back(x);
			flipflop = !flipflop;
		}
		else
			right.push_back(x);
	}
}

//we will use it to avoid box that are point or lines
inline void completeToBox(std::list<Kdtree::vecType>& xlist, std::list<Kdtree::vecType>& ylist, std::list<Kdtree::vecType>& zlist) {
	Kdtree::vecType boxCompliter(0);
	bool needHelp = false;
	if (xlist.front().x == xlist.back().x) {
		boxCompliter.x = xlist.back().x + 10;
		needHelp = true;
	}
	if (ylist.front().y == ylist.back().y) {
		boxCompliter.y = ylist.back().y + 10;
		needHelp = true;
	}
	if (zlist.front().z == zlist.back().z) {
		boxCompliter.z = zlist.back().z + 10;
		needHelp = true;
	}
	xlist.push_back(boxCompliter);
	ylist.push_back(boxCompliter);
	zlist.push_back(boxCompliter);
	xlist.sort([&](Kdtree::vecType& a, Kdtree::vecType& b) {return a[0] < b[0]; });
	ylist.sort([&](Kdtree::vecType& a, Kdtree::vecType& b) {return a[1] < b[1]; });
	zlist.sort([&](Kdtree::vecType& a, Kdtree::vecType& b) {return a[2] < b[2]; });
}

inline void prepareLists(std::list<Kdtree::vecType>& plist, std::list<Kdtree::vecType>& xlist, std::list<Kdtree::vecType>& ylist, std::list<Kdtree::vecType>& zlist)
{
	cloneList(plist, xlist); cloneList(plist, ylist); cloneList(plist, zlist);
	printf(".");
	xlist.sort([&](Kdtree::vecType& a, Kdtree::vecType& b) {return a[0] < b[0]; });
	printf(".");
	ylist.sort([&](Kdtree::vecType& a, Kdtree::vecType& b) {return a[1] < b[1]; });
	printf(".");
	zlist.sort([&](Kdtree::vecType& a, Kdtree::vecType& b) {return a[2] < b[2]; });

	completeToBox(xlist, ylist, zlist);

	printf("\b\b\b, sorted\n");
	//printf("%f %f %f %f %f %f\n", xlist.front().x, xlist.back().x, ylist.front().y, ylist.back().y, zlist.front().z, zlist.back().z);
}


void Kdtree::makeTree(std::list<Kdtree::vecType>& plist)
{
	root = new Node(3);
	max_depth = (unsigned int)log2((plist.size() >> 2));

	printf("size %d max depth %d", plist.size(), max_depth);

	std::list<Kdtree::vecType> xlist, ylist, zlist;
	prepareLists(plist, xlist, ylist, zlist);

	std::list<Kdtree::vecType> lists[3] = { xlist, ylist, zlist };
	Kdtree::_makeTree(root, lists, 0);
}

void Kdtree::_makeTree( Node* head, std::list<Kdtree::vecType> plist[], unsigned int depth )
{	
	int axis = depth % N;
	if( !plist[0].empty() & !plist[1].empty() & !plist[2].empty())
	{
		std::list<Kdtree::vecType> plistNEXT[2][3];
		std::list<Kdtree::vecType> l1, l2, l3, l4, l5, l6;
		plistNEXT[0][0] = l1;		plistNEXT[0][1] = l2;		plistNEXT[0][2] = l3;
		plistNEXT[1][0] = l4;		plistNEXT[1][1] = l5;		plistNEXT[1][2] = l6;
		Kdtree::vecType median =Kdtree::findMedian(plist[axis], plistNEXT[0][axis], plistNEXT[1][axis]);
		head->data = median;
		head->boundbox.push_back(plist[0].back() [0]);
		head->boundbox.push_back(plist[0].front()[0]);
		head->boundbox.push_back(plist[1].back() [1]);
		head->boundbox.push_back(plist[1].front()[1]);
		head->boundbox.push_back(plist[2].back() [2]);
		head->boundbox.push_back(plist[2].front()[2]);
			int axis2 = (axis + 1) % N, axis3 = (axis + 2) % N;
		cloneByMed(axis, median[axis], plist[axis2], plistNEXT[0][axis2], plistNEXT[1][axis2]);
		cloneByMed(axis, median[axis], plist[axis3], plistNEXT[0][axis3], plistNEXT[1][axis3]);

		Node* left_node = new Node(N);
		Node* right_node = new Node(N);

		if ((depth < max_depth) & (plist[0].size()) > 5 & (plist[1].size() > 5) & (plist[2].size() > 5)) {
			Kdtree::_makeTree(left_node, plistNEXT[0], depth + 1);
			if (!plistNEXT[0][axis].empty()) head->left = left_node;

			Kdtree::_makeTree(right_node, plistNEXT[1], depth + 1);
			if (!plistNEXT[1][axis].empty()) head->right = right_node;
		}		
	}
} 