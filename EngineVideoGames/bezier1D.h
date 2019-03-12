#pragma once
#include "Mesh.h"

class Bezier1D
{
	std::vector<glm::mat4> segments;
public:
	Bezier1D(void);
	IndexedModel GetLine(int resT);
	LineVertex GetVertex(int segment,int t);
	LineVertex GetControlPoint(int segment,int indx);
	
	glm::vec3 GetVelosity(int segment,int t);

	void MoveControlPoint(int segment, int indx,bool preserveC1);
	

	~Bezier1D(void);
};

