
#include "EulerAngles.h"
#include <stdio.h>
#include <iostream>

using namespace glm;

static void printMat(const mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

vec4 normXY(glm::vec4 &vec, int iterations)
{
	glm::vec4 v = vec,res = vec;
	float tmp = 1 - vec[2] * vec[2];
	res[2] = 0;
	if (tmp > 0)
	{
		for (int i = 0; i < iterations; i++)
		{
			res[0] = res[0] / tmp;
			res[1] = res[1] / tmp;
			tmp = 0.5 + (res[0] * res[0] + res[1] * res[1])*0.5;
		}
	}
	return res;
}

EulerAngles::EulerAngles()
{
	phi = glm::mat4(1);
	theta = glm::mat4(1);
	psi = glm::mat4(1);
}

EulerAngles::EulerAngles(glm::mat4 &mat, int iterations)
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
			phi =  rotate(phi,ang,vec3(0,0,1));
		else 
			phi = rotate(mat4(1),ang,vec3(0,0,1)) * phi;
		break;
	case xAxis1:
		if(mode == 0)
			theta = rotate(theta,ang,vec3(1,0,0));
		else
			theta = rotate(mat4(1),ang,vec3(1,0,0)) * theta;
		break;
	case zAxis2:
		if(mode == 0)
			psi =  rotate(psi,ang,vec3(0,0,1));
		else
			psi = rotate(mat4(1),ang,vec3(0,0,1)) * psi;
		break;
	case zAxis12:
		if(mode == 0)
		{
			
				phi =  rotate(phi,ang,vec3(0,0,1));
				psi =  rotate(psi,-ang,vec3(0,0,1));
			
		}
		else
		{
			phi = rotate(mat4(1),ang,vec3(0,0,1)) * phi;
			psi = rotate(mat4(1),-ang,vec3(0,0,1)) * psi;
		}
		break;
	}
}

void EulerAngles::split(glm::mat4 &mat, int iterations)
{
	
	    glm::mat4 tmp;
		float size;

		if (mat[0][0] >= 0.9999999 && mat[1][1] >= 0.9999999 && mat[2][2] >= 0.9999999)
		{
			phi = glm::mat4(1);
			theta = glm::mat4(1);
			psi = glm::mat4(1);
			return;
		}
		float angle = 90.0f;
		if (psi[0][0] * mat[0][0] + mat[0][1] * psi[1][0] == 0 && psi[0][0] * mat[1][0] + psi[0][1] * mat[1][1] == 0)
		{
			
			psi = glm::rotate(psi,angle,glm::vec3(0,0,1)); 

		}
		glm::mat4 rot90(1);
		rot90 = glm::rotate(rot90, angle, glm::vec3(0, 0, 1));
		//printMat(rot90); 
		for (int i = 0; i<iterations; i++)
		{
			tmp = mat * glm::transpose(psi); 
			phi[0] = normXY(tmp[0], 10);
			phi[1] = rot90*phi[0];

			tmp = glm::transpose(mat) * phi;
			psi[0] = normXY(tmp[0], 10);
			psi[1] = psi[0]*rot90;
		}
	
		tmp = glm::transpose(phi) * mat * glm::transpose(psi);
		theta = glm::matrixCompMult(tmp, glm::mat4(1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1));
		theta[0][0] = 1.0f;
	}
	
void EulerAngles::buildAngMatrices(glm::mat4 &mat)
{
	vec3 z = vec3(0,0,1);
	vec3 newZ = vec3(mat*vec4(z,0));
	vec3 n = normalize(cross(z,newZ));
	float c = dot(n,vec3(1,0,0));
	
	if(n.y*glm::sign(newZ.y)>0)
		buildAngMatrix(zAxis12,c*glm::sign(newZ.y),sqrt(1.0f-c*c));
	else
		buildAngMatrix(zAxis12,c*glm::sign(newZ.y),-sqrt(1.0f-c*c));
	c= dot(z,newZ);
	if(cross(z,n).y<0)
		buildAngMatrix(xAxis1,c,sqrt(1.0f-c*c));
	else
		buildAngMatrix(xAxis1,c,-sqrt(1.0f-c*c));
	
}

mat4 EulerAngles::makeRot() const
{
	return phi*theta*psi;
}