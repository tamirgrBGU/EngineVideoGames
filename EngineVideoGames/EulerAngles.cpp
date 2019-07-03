
#include "EulerAngles.h"
#include <stdio.h>
#include <iostream>


static void printMat(const glm::mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

EulerAngles::EulerAngles()
{
	phi = glm::mat4(1);
	theta = glm::mat4(1);
	psi = glm::mat4(1);
}

EulerAngles::EulerAngles(glm::mat4 &mat)
{
	EulerAngles();
	buildAngMatrices(mat);
}

EulerAngles::EulerAngles(float phif, float thetaf, float psif)
{
	EulerAngles();
	phi = glm::rotate(phi, phif, glm::vec3(0, 0, 1));
	theta = glm::rotate(theta, thetaf, glm::vec3(1, 0, 0));
	psi = glm::rotate(psi, psif, glm::vec3(0, 0, 1));

}

void EulerAngles::printAngles(int ang)
{
	switch (ang)
	{
	case zAxis1:
		printf("Phi matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", phi[j][i]);
			printf("\n");
		}
		break;
	case xAxis1:
		printf("Theta matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", theta[j][i]);
			printf("\n");
		}

		break;
	case zAxis2:
		printf("Psi matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", psi[j] [i]);
			printf("\n");
		}
		break;
	case zAxis12:
		printf("Phi matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", phi[j] [i]);
			printf("\n");
		}
		printf("Theta matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", theta[j][  i]);
			printf("\n");
		}
		printf("Psi matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", psi[j ][ i]);
			printf("\n");
		}
		break;
	case 5:
		glm::mat4 tmp = phi*theta*psi;
		printf("rotation matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", tmp[j][i]);
			printf("\n");
		}
		break;
	}
	
}

float EulerAngles::GetCosAng(int ang)
{
	switch (ang)
	{
	case zAxis1:
		return phi[1][1];
	case xAxis1:
		return theta[1][1];
	case zAxis2:
		return psi[1][1];
	default:
		return 1;
	}
}

void EulerAngles::buildAngMatrix(int XorZ, float c, float s)
{
	switch (XorZ)
	{
	case zAxis1:
		phi[0][0] = c;
		phi[1][1] = c;
		phi[1][0] = -s;
		phi[0][1] = s;
		break;
	case xAxis1:
		theta[1][1] = c;
		theta[2][2] = c;
		theta[2][1] = -s;
		theta[1][2] = s;
		break;
	case zAxis2:
		psi[0][0] = c;
		psi[1][1] = c;
		psi[1][0] = s;
		psi[0][1] = -s;
		break;
	case zAxis12:
		phi[0][0] = c;
		phi[1][1] = c;
		phi[1][0] = -s;
		phi[0][1] = s;
		psi[0][0] = c;
		psi[1][1] = c;
		psi[1][0] = s;
		psi[0][1] = -s;
		break;
	}
}

void EulerAngles::alignedRot(int XorZ,float angle,int mode )
{
	float ang = angle;
	switch (XorZ)
	{
	case zAxis1:
		if(mode == 0)
			phi = glm::rotate(phi,ang,glm::vec3(0,0,1));
		else 
			phi = glm::rotate(glm::mat4(1),ang,glm::vec3(0,0,1)) * phi;
		break;
	case xAxis1:
		if(mode == 0)
			theta = glm::rotate(theta,ang,glm::vec3(1,0,0));
		else
			theta = glm::rotate(glm::mat4(1),ang,glm::vec3(1,0,0)) * theta;
		break;
	case zAxis2:
		if(mode == 0)
			psi =  glm::rotate(psi,ang,glm::vec3(0,0,1));
		else
			psi = glm::rotate(glm::mat4(1),ang,glm::vec3(0,0,1)) * psi;
		break;
	case zAxis12:
		if(mode == 0)
		{			
				phi = glm::rotate(phi, ang,glm::vec3(0,0,1));
				psi = glm::rotate(psi,-ang,glm::vec3(0,0,1));
		}
		else
		{
			phi = glm::rotate(glm::mat4(1), ang,glm::vec3(0,0,1)) * phi;
			psi = glm::rotate(glm::mat4(1),-ang,glm::vec3(0,0,1)) * psi;
		}
		break;
	}
}
	
void EulerAngles::buildAngMatrices(glm::mat4 &mat)
{
	glm::vec3 z = glm::vec3(0,0,1);
	glm::vec3 newZ = glm::vec3(mat*glm::vec4(z,0));
	glm::vec3 n = glm::normalize(glm::cross(z,newZ));
	float c = glm::clamp(glm::dot(n,glm::vec3(1,0,0)),-1.0f,1.0f);
	
	buildAngMatrix(zAxis1,c*glm::sign(newZ.y),sqrt(1.0f-c*c)*glm::sign(n.y*newZ.y));

	c= glm::clamp(glm::dot(z,newZ),-1.0f,1.0f);

	buildAngMatrix(xAxis1,c,-glm::sign(glm::cross(z,n).y)*sqrt(1.0f-c*c));

	
	//psi = glm::transpose(phi*theta)*mat;
	glm::vec3 newX =  glm::vec3(glm::transpose(phi*theta)*mat*glm::vec4(1,0,0,0));
	glm::vec3 x = glm::vec3(1,0,0);//(phi*theta*glm::vec4(1,0,0,0));
 	c = glm::clamp(glm::dot(x,newX),-1.0f,1.0f);

	buildAngMatrix(zAxis2,c,sqrt(1.0f-c*c));
	
}

glm::mat4 EulerAngles::makeRot() const
{
	return phi*theta*psi;
}