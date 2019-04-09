#pragma once

#ifndef BEIZER1D_H
#define BEIZER1D_H
#include "Mesh.h"
#define SEG_CON_PTS 4

class Bezier1D
{
	//each line in the matrix represents x,y,z,w of a control point
	std::vector<glm::mat4> segments;
	std::vector<int> binomTree;

public:
	Bezier1D(void);
	IndexedModel GetLine(int resT);						//generates model for rendering using MeshConstructor::initLin
	LineVertex GetVertex(int segment, float t);			//returns point on curve in the requested segment for value of t
	LineVertex GetControlPoint(int segment, int indx);	//returns a control point in the requested segment. indx will be 0,1,2,3, for p0,p1,p2,p3	
	glm::vec3 GetVelosity(int segment, float t);			//returns the derivative of the curve in the requested segment for value of t
	glm::mat4 GetSegment(int segmentT);
	glm::vec4 calc_bezier_point_derivate(float u, glm::mat4 segment) {
		return segment[0]*calc_bezier_factor_derivate(0,u) + segment[1]* calc_bezier_factor_derivate(1, u) +
			   segment[2]*calc_bezier_factor_derivate(2,u) + segment[3]* calc_bezier_factor_derivate(3, u);
	}

	static glm::vec4 calc_bezier_point(float u, glm::mat4 segment) {
		return segment[3]*(pow(u, 3))			+ segment[2]*(3 * pow(u, 2)*(1 - u)) +
			   segment[1]*(3 * pow(1 - u, 2)*u) + segment[0]*(pow(1 - u, 3));
	}

	float calc_bezier_factor(int i, float u) {
		return binomTree[i] * pow(u, i) * pow(1-u, SEG_CON_PTS-1-i);
	}

	float calc_bezier_factor_derivate(int i, float u) {
		return 
			i == 0 ?			 (SEG_CON_PTS-1) * pow(1-u, SEG_CON_PTS-2) :
			i == SEG_CON_PTS-1 ? (SEG_CON_PTS-1) * pow(  u, SEG_CON_PTS-2) :
									binomTree[i] *    (i + (SEG_CON_PTS-1 -2*i)*u) *
									pow(u, i-1)  * pow(1-u, SEG_CON_PTS-2 -i);
	}

	vec3 GetControlPointPos(int seg, int indx) {
		vec4 wanted = segments[seg][indx];;
		vec3 v3(wanted.x, wanted.y, wanted.z);
		return v3;
	}

	void MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition); //change the positon of one control point. when preserveC1 is true it may affect other  control points 

	int segNo() {
		return segments.size();
	}

	static vec4 v3to4(vec3 v) {
		return vec4(v.x, v.y, v.z, 0);
	}

	static vec3 v4to3(vec4 v) {
		return vec3(v.x, v.y, v.z);
	}

	~Bezier1D(void);
private:
	void initBinomTree() {
		if (binomTree.size() == 0) {
			binomTree.push_back(1);
			for (int k = 1; k < SEG_CON_PTS; k++) {
				binomTree.push_back(binomTree[k - 1] * (SEG_CON_PTS - k) / k);
			}
		}
	}
	bool checkSegment(int segmentA, unsigned int segmentB);
	void moveFixedPoint(int segmentA, int segmentB, glm::vec4 newPosition);
	void moveNonFixedPoint(int segmentA, int segmentB, int changedIndx, glm::vec4 newPosition);
	void updateSegment(int segment, int indx, glm::vec4 newPosition);
};

#endif