#include <iostream>
#include "Eigen/Dense"
#include "EulerAngles.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace Eigen;

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

int main(int argc, char** argv)
{
//	glm::mat4 R = glm::rotate(glm::mat4(1),-34.0f,glm::vec3(-1.0f,-0.7f,0.0f));
	
	MatrixXd A = MatrixXd::Identity(4, 3);
	A(0,1) = 4;
	A(3,1) = 0.5;
	cout << "Here is the matrix A:\n" << A << endl;
	VectorXd b = VectorXd(4);
	b = VectorXd::Ones(4);
	cout << "Here is the right hand side b:\n" << b << endl;
    
	VectorXd x = A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);
	cout << "The least-squares solution is:\n"<< x <<endl;
	cout << "A*x is:\n" << A*x << endl;
	
	//EulerAngles E = EulerAngles(R);
	//E.printAngles(5);
//	printMat(R);
	getchar();
	return 0;
}