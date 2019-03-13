#include "game.h"

Game::Game():Scene(){}

Game::Game(glm::vec3 position,float angle,float hwRelation,float near, float far) : Scene(position,angle,hwRelation,near,far)
{ 

}

void Game::Init()
{
	addCube("",-1);

	addCube("",-1);
	//pickedShape = 0;
	//shapeTransformation(xScale,30);
	//shapeTransformation(yScale,30);
	pickedShape = 0;
	shapeTransformation(zGlobalTranslate,-30);
	shapeTransformation(yGlobalRotate,45);
	pickedShape = 1;
	shapeTransformation(zGlobalTranslate,-30);
	shapeTransformation(yGlobalTranslate,5);
	addAxis();
	pickedShape = 2;
	shapeTransformation(zGlobalTranslate,-30);
	shapeTransformation(yScale,10);
	shapeTransformation(xScale,10);
	shapeTransformation(zScale,10);

}

void Game::Update(glm::mat4 MVP,glm::mat4 Normal,Shader *s)
{
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
		s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 1.0f);
	s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	
}

Game::~Game(void)
{
}
