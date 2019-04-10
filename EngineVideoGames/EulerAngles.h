 #pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EulerAngles
{
public:
	enum axis{zAxis1,xAxis1,zAxis2,zAxis12};
	EulerAngles();
	EulerAngles(glm::mat4 &mat);
	EulerAngles(float phif, float thetaf,float psif);
	virtual ~EulerAngles(){}
    void printAngles(int ang);
	float GetCosAng(int ang);
protected:
	
	void buildAngMatrix(int XorZ, float c, float s);
	void alignedRot(int XorZ,float angle,int mode);
	void buildAngMatrices(glm::mat4 &mat);
	glm::mat4 makeRot() const;

private:
	glm::mat4 phi,theta,psi;
	
};