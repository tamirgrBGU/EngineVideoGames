#include "bezier2D.h"


Bezier2D::Bezier2D(void)
{
	b = Bezier1D();
	circularSubdivision = 4;
}
Bezier2D::Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision) {
	this->b = b;
	this->circularSubdivision = circularSubdivision;
}

IndexedModel Bezier2D::GetSurface(int resT, int resS) {


}

Vertex Bezier2D::GetVertex(int segmentT, int segmentS, float t, float s) {

}

glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s) {

}

Bezier2D::~Bezier2D(void)
{
}
