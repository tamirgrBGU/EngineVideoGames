#define _USE_MATH_DEFINES
/*
*   main.cpp
*  
*   k-d Tree Project
*	author: Arvind Rao
*   license: GNU 
*
*/  

#include "kdtree.h"
#include "intersect.h"
#include <iostream>
#include <array>
#include <bezier1D.cpp>
#include <bezier2D.cpp>
#include <mesh.h>
#include <list>


const int _numpts = 10;
const int _numbox = 26;
const int _dim = 3;


Bezier2D b2d = Bezier2D::Bezier2D(Bezier1D::Bezier1D(), 3);
IndexedModel model = b2d.Bezier2D::GetSurface(5, 5);
std::vector<glm::vec3> testpoints2 = model.positions;
Node::vecType testpoints[_numpts] =
	{
		Node::vecType(38.40924, 4.11543, 8.10499,1), 
		Node::vecType(6.50689, 1.3663, 3.43026,1), 
		Node::vecType(9.7614, 9.8382, 0.672512,1), 
		Node::vecType(0.113181, 2.22785, 3.46726,1), 
		Node::vecType(5.23381, 4.69416, 4.74723,1), 
		Node::vecType(9.74655, 0.191659, 1.2064,1), 
		Node::vecType(2.8546, 7.32662, 8.51895,1), 
		Node::vecType(6.21829, 0.779546, 1.82988,1), 
		Node::vecType(8.83612, 8.70544, 2.40537,1), 
		Node::vecType(6.50697, 2.70078, 1.93852,1) 
	};

Node::vecType box[_numbox] =
	{
		Node::vecType(1, 1, 1, 1),
		Node::vecType(1, 1, -1, 1),
		Node::vecType(1, -1, 1, 1),
		Node::vecType(1, -1, -1, 1),
		Node::vecType(1, 0, 1, 1),
		Node::vecType(1, 1, 0, 1),
		Node::vecType(1, 0, -1, 1),
		Node::vecType(1, -1, 0, 1),
		Node::vecType(1, 0, 0, 1),

		Node::vecType(-1, 1, 1, 1),
		Node::vecType(-1, 1, -1, 1),
		Node::vecType(-1, -1, 1, 1),
		Node::vecType(-1, -1, -1, 1),
		Node::vecType(-1, 0, 1, 1),
		Node::vecType(-1, 1, 0, 1),
		Node::vecType(-1, 0, -1, 1),
		Node::vecType(-1, -1, 0, 1),
		Node::vecType(-1, 0, 0, 1),

		Node::vecType(0, 1, 0, 1),
		Node::vecType(0, 0, 1, 1),
		Node::vecType(0, -1, 0, 1),
		Node::vecType(0, 0, -1, 1),
		Node::vecType(0, 1, 1, 1),
		Node::vecType(0, 1, -1, 1),
		Node::vecType(0, -1, 1, 1),
		Node::vecType(0, -1, -1, 1)
	};

Node::vecType box2[_numbox] =
{
	Node::vecType(3, 3, 3, 1),
	Node::vecType(3, 2, 3, 1),
	Node::vecType(2, 3, 3, 1),
	Node::vecType(3, 3, 2, 1),
	Node::vecType(3, 2, 2, 1),
	Node::vecType(2, 3, 2, 1),
	Node::vecType(2, 2, 3, 1),
	Node::vecType(2, 2, 2, 1)
};


void printVector(Node::vecType pt)
{
	
	    std::cout<< pt.x << ", "<< pt.y << ", "<< pt.z<<"\n";
	
}

glm::mat4 I;
int main(int argc, char ** argv)
{
	std::vector<glm::vec3> point_list;
	for (int i = 0; i < _numbox; i++)
	{
		point_list.push_back(Bezier1D::v4to3(box[i]));
	}
	std::vector<glm::vec3> point_list2;
	for (int i = 0; i < _numbox; i++)
	{
		point_list2.push_back(vec3(box[i][0]+1.1, box[i][1], box[i][2]));
	}
	intersect a(point_list);
	intersect b(point_list2);
	std::vector<IndexedModel> sol = a.isIntersect(&I, &I, b);

	int i = 0;
	for (IndexedModel model : sol) {
		printf("\n\n box %d", ++i);
		for (unsigned int i = 0; i<model.positions.size(); i++) {
			if (i % 2 == 0) printf("\n");
			printf("%f %f %f\t\t", model.positions[i][0], model.positions[i][1], model.positions[i][2]);
		}
	}

	printf("\n done");
	int input;	std::cin >> input;
}