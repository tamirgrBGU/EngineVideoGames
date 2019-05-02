#include "game.h"
#include <iostream>

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}


Game::Game():Scene(){curve = 0;}

Game::Game(glm::vec3 position,float angle,float hwRelation,float near, float far) : Scene(position,angle,hwRelation,near,far)
{ 
	curve = new Bezier1D();
}

void Game::addShape(int type,int parent,unsigned int mode)
{
		chainParents.push_back(parent);
		if(type!=BezierLine && type!=BezierSurface)
			shapes.push_back(new Shape(type,mode));
		else
		{
			if(type == BezierLine)
				shapes.push_back(new Shape(curve,30,30,false,mode));
			else
				shapes.push_back(new Shape(curve,30,30,true,mode));
		}
}

void Game::Init()
{
	addShape(Axis,-1,LINES);
	addShape(Octahedron,-1,TRIANGLES);
	//addShapeFromFile("../res/objs/torus.obj",-1,TRIANGLES);
	addShapeCopy(1,-1,TRIANGLES);
	addShape(Cube,1,LINE_LOOP);
	addShapeCopy(3,2,LINE_LOOP);
	
	
	//translate all scene away from camera
	myTranslate(glm::vec3(0,0,-20),0);

	pickedShape = 0;

	shapeTransformation(yScale,10);
	shapeTransformation(xScale,10);
	shapeTransformation(zScale,10);

	
	ReadPixel();
	
	pickedShape = 2;
	shapeTransformation(zLocalRotate,45);	

	pickedShape = 1;

	shapeTransformation(zGlobalTranslate,-10);
	shapeTransformation(yScale,3.30f);
	shapeTransformation(xScale,3.30f);
	shapeTransformation(zScale,3.30f);

	pickedShape =3;
	shapeTransformation(yScale,3.30f);
	shapeTransformation(xScale,3.30f);
	shapeTransformation(zScale,3.30f);

	pickedShape = -1;
	Activate();
}

	void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.1f,0.8f,0.7f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
	//if(pickedShape>=0)
	//	printMat(GetShapeTransformation());
}

void Game::WhenTranslate()
{
	if(pickedShape>=0)
	{
		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0,0,0,1);
	//	std::cout<<"( "<<pos.x<<", "<<pos.y<<", "<<pos.z<<")"<<std::endl;
	}
}

void Game::Motion()
{
	if(isActive)
	{
		int p = pickedShape;
		pickedShape = 2;
		shapeTransformation(zLocalRotate,0.75);
		pickedShape = p;
	}
}

Game::~Game(void)
{
	delete curve;
}
