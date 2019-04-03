#include "game.h"
#include <iostream>

int lateUpdate = 0;
static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}
bool in_corect = true;
float scale = 5;

Game::Game() :Scene() { curve = 0; }

Game::Game(glm::vec3 position, float angle, float hwRelation, float near, float far) : Scene(position, angle, hwRelation, near, far)
{
	curve = new Bezier1D();
}

void Game::addShape(int type, int parent, unsigned int mode)
{
	chainParents.push_back(parent);
	if (type != BezierLine && type != BezierSurface)
		shapes.push_back(new Shape(type, mode));
	else
	{
		if (type == BezierLine)
			shapes.push_back(new Shape(curve, 30, 30, false, mode));
		else
			shapes.push_back(new Shape(curve, 30, 30, true, mode));
	}
}

void Game::Init()
{

	addShape(Axis, -1, LINES);
	//addShape(Octahedron,-1,TRIANGLES);
	//addShapeFromFile("../res/objs/torus.obj",-1,TRIANGLES);
	//addShapeCopy(1,-1,TRIANGLES);
	addShape(BezierLine, -1, LINES);
	addShape(Octahedron, -1, TRIANGLES);
	for (int i = 0; i < curve->numberOfPoints - 1; i++)
		addShapeCopy(2, -1, TRIANGLES);

	//translate all scene away from camera
	myTranslate(glm::vec3(0, 0, -20), 0);

	pickedShape = 0;

	shapeTransformation(yScale, 10);
	shapeTransformation(xScale, 10);
	shapeTransformation(zScale, 10);



	ReadPixel();

	pickedShape = 1;

	//shapeTransformation(yScale, 1.5);
	//shapeTransformation(xScale, 1.5);
	//shapeTransformation(zScale, 1.5);


	for (int i = 0; i < curve->numberOfPoints; i++)
	{
		pickedShape++;
		LineVertex CP0 = curve->GetControlPoint(i / 3, i % 3);

		//glm::vec3 point = *tmp;
		shapeTransformation(xGlobalTranslate, CP0.GetPos()->x);
		shapeTransformation(yGlobalTranslate, CP0.GetPos()->y);
		shapeTransformation(zGlobalTranslate, CP0.GetPos()->z);

		shapeTransformation(yScale, 1 / scale);
		shapeTransformation(xScale, 1 / scale);
		shapeTransformation(zScale, 1 / scale);
	}


	addShape(BezierSurface, -1, TRIANGLES);

	pickedShape = -1;
	Activate();
}

void Game::Update(glm::mat4 MVP, glm::mat4 Normal, Shader *s)
{

	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 1.0f);
	s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	if (lateUpdate == 100) {
		shapes[shapes.size() - 1] = new Shape(curve, 10, 7, true, QUADS);
		lateUpdate = 0;
	}
	else
	{
		lateUpdate++;
	}

}

void Game::WhenRotate()
{
	//if(pickedShape>=0)
	//	printMat(GetShapeTransformation());
}

void Game::WhenTranslate()
{
	if (pickedShape > 2 && pickedShape < shapes.size() - 1)
	{
		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
		glm::vec4 oldpos(*curve->GetControlPoint((pickedShape - 2) / 3, (pickedShape - 2) % 3).GetPos(), 1);
		curve->MoveControlPoint((pickedShape - 2) / 3, (pickedShape - 2) % 3, in_corect, pos);

		glm::vec4 diff = scale * (pos - oldpos);

		if (in_corect)
		{
			in_corect = false;
			switch ((pickedShape - 2) % 3)
			{
			case 0:
				pickedShape--;
				shapeTransformation(xGlobalTranslate, diff.x);
				shapeTransformation(yGlobalTranslate, diff.y);
				shapeTransformation(zGlobalTranslate, diff.z);
				pickedShape += 2;
				shapeTransformation(xGlobalTranslate, diff.x);
				shapeTransformation(yGlobalTranslate, diff.y);
				shapeTransformation(zGlobalTranslate, diff.z);
				pickedShape--;
				break;
			case 1:
				if ((pickedShape - 2) / 3 != 0) {
					pickedShape -= 2;
					shapeTransformation(xGlobalTranslate, -diff.x);
					shapeTransformation(yGlobalTranslate, -diff.y);
					shapeTransformation(zGlobalTranslate, -diff.z);
					pickedShape += 2;
				}
				break;
			case 2:
				if ((pickedShape - 2) / 3 < ((shapes.size() - 2) / 3) - 1) {
					pickedShape += 2;
					shapeTransformation(xGlobalTranslate, -diff.x);
					shapeTransformation(yGlobalTranslate, -diff.y);
					shapeTransformation(zGlobalTranslate, -diff.z);
					pickedShape -= 2;
				}
				break;
			}
			in_corect = true;
			shapes[1] = new Shape(curve, 30, 30, false, LINES);

		}
		//	std::cout<<"( "<<pos.x<<", "<<pos.y<<", "<<pos.z<<")"<<std::endl;
	}
}

void Game::Motion()
{
	/*if(isActive)
	{
		int p = pickedShape;
		pickedShape = 2;
		shapeTransformation(zLocalRotate,0.45);
		pickedShape = p;
	}*/
}

Game::~Game(void)
{
	delete curve;
}
