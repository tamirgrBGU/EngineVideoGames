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
Kdtree::~Kdtree(){}

/*
*
*
*/
Kdtree::vecType Kdtree::findMedian(int axis, std::list<Kdtree::vecType> &plist, std::list<Kdtree::vecType> &left, 
							std::list<Kdtree::vecType> &right )
{
	Kdtree::vecType median;
	int size = plist.size();
	int med = int(ceil( float(size) / float(2) ));
	int count = 0;
	
	if ( size == 1)
		return plist.front();
	
	// Using lambda function here, to define comparison function--parametrized by 'axis'
	plist.sort( [&](Kdtree::vecType& a, Kdtree::vecType& b){return a[axis] < b[axis];});
	
	for ( auto& x : plist )
	{
		if (count < med)
			left.push_back(x);
		else
			right.push_back(x);
		++count;
	}
	median = left.back();
	left.pop_back();
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
			std::cout<< "level: "<<  depth <<"\n";
			std::swap(current, next);
		}
	}
}

/*
*  algorithm is based on http://en.wikipedia.org/wiki/Kd_tree
*/
void Kdtree::makeTree(std::vector<glm::vec3>& pvec)
{
	Node* head = new Node(3);
	std::list<Kdtree::vecType> point_list;
	glm::vec3 v;
	vecType v4;
	for (unsigned int i = 0; i < pvec.size(); i++)
	{
		v = pvec[i];
		v4 = glm::vec4(v.x, v.y, v.z, 1);
		point_list.push_back(v4);
	}
	Kdtree::_makeTree(head, point_list, 0);
	Kdtree::root = head;
}

void Kdtree::makeTree(std::list<Kdtree::vecType>& plist)
{
	Node* head = new Node(3);
	Kdtree::_makeTree( head, plist, 0 );
	Kdtree::root = head;
}

void Kdtree::_makeTree( Node* head, std::list<Kdtree::vecType>& plist, int depth )
{	
	if( !plist.empty() ) 
	{
		int axis = depth % N;
		
		std::list<Kdtree::vecType> left_list;
		std::list<Kdtree::vecType> right_list;
		Kdtree::vecType median = Kdtree::findMedian(axis, plist, left_list, right_list); 
		head->data = median;
		
		Node* left_node = new Node(N);
		Node* right_node = new Node(N);
		
		Kdtree::_makeTree( left_node, left_list, depth+1);
		if (!left_list.empty()) head->left = left_node;
		
		Kdtree::_makeTree( right_node, right_list, depth+1);
		if (!right_list.empty()) head->right = right_node;
	}
} 