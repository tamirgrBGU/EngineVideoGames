#include "bezier2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


Bezier2D::Bezier2D(void)
{
	b = Bezier1D();
	circularSubdivision = 4;
}
Bezier2D::Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision) {
	this->b = b;
	this->circularSubdivision = circularSubdivision;
}

//IndexedModel Bezier2D::GetSurface(int resT, int resS) {
//	
//
//}

Vertex Bezier2D::GetVertex(int segmentT, int segmentS, float t, float s) {
	LineVertex t1 = this->b.GetVertex(segmentT,t);
	glm::vec4 loc = glm::vec4(*b.GetControlPoint(segmentT,t).GetPos(), 1);
	float angle = (360 / circularSubdivision)*segmentS + (360 / circularSubdivision)*s;
	glm::vec4 newloc = loc * glm::rotate(angle, glm::vec3(1, 0, 0));
	return Vertex(glm::vec3(newloc), glm::vec2(0, 0), GetNormal(segmentT, segmentS, t, s), *t1.GetColor());
}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s) {
	glm::vec4 normal_2d = glm::vec4(this->b.GetVelosity(segmentT,t),0)*glm::vec4(0, 0, 1, 0);
	float angle = (360 / circularSubdivision)*segmentS + (360 / circularSubdivision)*s;
	glm::vec4 normal_3d = normal_2d * glm::rotate(angle, glm::vec3(1, 0, 0));
	return glm::vec3(normal_3d);
}

Bezier2D::~Bezier2D(void)
{
}
