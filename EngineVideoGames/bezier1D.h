#pragma once
#include "Mesh.h"

class Bezier1D
{
	//each line in the matrix represents x,y,z,w of a control point
	std::vector<glm::mat4> segments;
public:
	Bezier1D(void);
	IndexedModel GetLine(int resT);						//generates model for rendering using MeshConstructor::initLin
	LineVertex GetVertex(int segment,int t);			//returns point on curve in the requested segment for value of t
	LineVertex GetControlPoint(int segment,int indx);	//returns a control point in the requested segment. indx will be 0,1,2,3, for p0,p1,p2,p3
	
	glm::vec3 GetVelosity(int segment,int t);			//returns the derivative of the curve in the requested segment for value of t

	void MoveControlPoint(int segment, int indx,bool preserveC1, glm::vec4 newPosition); //change the positon of one control point. when preserveC1 is true it may affect other  control points 
	

	~Bezier1D(void);
};

