 #pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define M_PI 3.14159265359

class EulerAngles
{
public:
	enum axis{zAxis1,xAxis1,zAxis2,zAxis12};
	EulerAngles();
	EulerAngles(glm::mat4 &mat, int iterations);
	EulerAngles(float phif, float thetaf,float psif);
protected:
	void printAngles(int ang);
	void buildAngMatrix(int XorZ, float c, float s);
	void alignedRot(int XorZ,float angle,int mode);
	void buildAngMatrices(glm::mat4 &mat);
	glm::mat4 makeRot() const;
	void EulerAngles::split(glm::mat4 &mat,int iterations);
private:
	glm::mat4 phi,theta,psi;
	

};