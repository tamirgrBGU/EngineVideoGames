#include "MovableGLM.h"
#include <stdio.h>

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

vec3 MovableGLM::findAxis(vec3 vec) const
{

	return vec3(transpose( rotateMat*makeRot())*vec4(vec,1));
}

MovableGLM::MovableGLM()
{
	translateMat[0] = mat4(1);  //translation to general point
	translateMat[1] = mat4(1);  //translation to COM
	rotateMat = mat4(1);     //rotation around general point or (0,0,0) of global coordinate
	scaleFactor = vec3(1);
}

mat4 MovableGLM::makeTransScale() const
{
	return makeTrans() * scale(mat4(1), scaleFactor);
}

mat4 MovableGLM::makeTransScale(mat4 &prevTransformations) const
{
	return prevTransformations * makeTransScale();
}
void MovableGLM::copyTranslations(MovableGLM *mGLM)
{
	for (size_t i = 0; i < 2; i++)
	{
		translateMat[i] = mGLM->translateMat[i];
	}
	//TODO: update rotation 
}


mat4 MovableGLM::makeTrans(mat4 &prevTransformations) const
{
	return prevTransformations * makeTrans();
}

mat4 MovableGLM::makeTrans() const
{
	return  translateMat[0]   *rotateMat * makeRot() * translateMat[1] ;
}


void MovableGLM::myRotate(float ang, glm::vec3 &vec, int indx)
{
	if(indx >=0 && indx <=3)
	{
		alignedRot(indx,ang,0);
	}
	else
	{
		//split(rotate(makeRot(),ang,vec),10);
		rotateMat = rotate(rotateMat,ang,vec);
	}
//	printAngles(zAxis12);
	//else //general rotation
	//{

	//}
}

void MovableGLM::myTranslate(vec3 &vec, int indx)
{
	translateMat[indx] = translate(translateMat[indx], scaleFactor * vec ) ;
	//printMat(translateMat[indx]);
}

void MovableGLM::myScale(vec3 & vec)
{
	scaleFactor = scaleFactor*vec;
}

void MovableGLM::globalSystemRot(float ang, vec3 &vec, int indx)
{
	//ang = ang / 180.0*pi;
	vec3 tmp = vec;
	if(indx >=0 && indx <=3)
	{
		alignedRot(indx,ang,1);
	}
	else
	{
	//	split(rotate(makeRot(),ang,vec),10);
		rotateMat = rotate(mat4(1),ang,vec) * rotateMat;
		
	}
//	printAngles(zAxis12);
//	rotateMat[indx] = rotate(mat4(1),ang,tmp)*rotateMat[indx];
}

void MovableGLM::translateInSystem(const MovableGLM  &system,vec3 &vec, int indx, bool toScale)
{
	vec3 tmp;
	if (toScale)
		tmp = system.findAxis(scaleFactor * vec);
	else
		tmp = system.findAxis(vec);
	translateMat[indx] = translate(translateMat[indx], tmp);	
}

void MovableGLM::changeCenterOfRotation(vec3 & Pvec)
{
	myTranslate(Pvec, 0 );
	myTranslate(-Pvec, 1);
}

void MovableGLM::zeroTrans()
{
	translateMat[0] = mat4(1);
	translateMat[1] = mat4(1);
}

void MovableGLM::zeroRot(bool generalRot)
{
	if(generalRot)
		rotateMat = mat4(1);
}

void MovableGLM::getTraslate(vec4 & vec)
{
	mat4 mat = makeTrans();
	vec = mat[3];
}

void MovableGLM::mergeRot2Euler()
{
	glm::mat4 mat = rotateMat*makeRot();
	buildAngMatrices(mat);
	zeroRot(true);
}
//vec3 MovableGLM::getTipPos(mat4 &prevTransformations)
//{
//	return vec3(makeTrans(prevTransformations) * translateMat[1] *  scale(mat4(1), scaleFactor) * vec4(0,0,0,1));
//}
//
//
//vec3 MovableGLM::getRootPos(mat4 &prevTransformations)
//{
//	return vec3(makeTrans(prevTransformations) * transpose(translateMat[1]) *  scale(mat4(1), scaleFactor) * vec4(0,0,0,1));
//}
//
//vec3 MovableGLM::getXdirection(glm::mat4 &prevTransformations)
//{
//	return vec3(makeTrans(prevTransformations) * vec4(1,0,0,0));
//}
//vec3 MovableGLM::getZdirection(glm::mat4 &prevTransformations)
//{
//	return vec3(makeTrans(prevTransformations) * vec4(0,0,1,0));
//}

vec3 MovableGLM::getCenterOfRotation(glm::mat4 &prevTransformations)
{
	return vec3(makeTrans(prevTransformations) * transpose(translateMat[1])*vec4(0,0,0,1));
}

glm::vec3 MovableGLM::getPointInSystem(glm::mat4 &prevTransformations,glm::vec3 point)
{
	return vec3(makeTransScale(prevTransformations) * vec4(point,1));
}
	
glm::vec3 MovableGLM::getVectorInSystem(glm::mat4 &prevTransformations,glm::vec3 vec)
{
	return vec3(makeTrans(prevTransformations) * vec4(vec,0));
}

void MovableGLM::printRot(bool generalRot)
{
	if(generalRot)
	{
		printf("rotation matrix: \n");
		printMat(rotateMat);
	}
	else 
		printAngles(zAxis12);

}