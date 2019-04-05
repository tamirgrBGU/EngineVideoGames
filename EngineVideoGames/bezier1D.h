
#pragma once

#ifndef BEIZER1D_H
#define BEIZER1D_H
#include "Mesh.h"

class Bezier1D
{

	//each line in the matrix represents x,y,z,w of a control point
	std::vector<glm::mat4> segments;
public:
	Bezier1D(void);
	IndexedModel GetLine(int resT);						//generates model for rendering using MeshConstructor::initLin
	LineVertex GetVertex(int segment, float t);			//returns point on curve in the requested segment for value of t
	LineVertex GetControlPoint(int segment, int indx);	//returns a control point in the requested segment. indx will be 0,1,2,3, for p0,p1,p2,p3	
	glm::vec3 GetVelosity(int segment, float t);			//returns the derivative of the curve in the requested segment for value of t
	glm::mat4 GetSegment(int segmentT);
	static glm::vec4 calc_bezier_point_derivate(float t, glm::mat4 segment) {
		return segment[3].operator*=(pow(t, 2) * 3) + segment[2].operator*=(3 - 12 * t - 9 * pow(t, 2)) +
			segment[1].operator*=(3 - 6 * t + 3 * pow(t, 2)) + segment[0].operator*=(3 * pow(t - 1, 2));
	}

	static glm::vec4 calc_bezier_point(float t, glm::mat4 segment) {
		return segment[3].operator*=(pow(t, 3)) + segment[2].operator*=(3 * pow(t, 2)*(1 - t)) +
			segment[1].operator*=(3 * pow(1 - t, 2)*t) + segment[0].operator*=(pow(1 - t, 3));
	}

	vec3 GetControlPointPos(int seg, int indx){
		vec4 wanted = segments[seg][indx];;
		vec3 v3(wanted.x, wanted.y, wanted.z);
		return v3;
	}

	void MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition); //change the positon of one control point. when preserveC1 is true it may affect other  control points 

	int segNo() {
		return segments.size();
	}

	~Bezier1D(void);
private:
	bool checkSegment(int segmentA, unsigned int segmentB);
	void moveFixedPoint(int segmentA, int segmentB, glm::vec4 newPosition);
	void moveNonFixedPoint(int segmentA, int segmentB, int changedIndx, glm::vec4 newPosition);
	void updateSegment(int segment, int indx, glm::vec4 newPosition);
};

#endif