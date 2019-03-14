#define GLEW_STATIC
#include <GL\glew.h>
#include "shape.h"
#include "Log.hpp"


Shape::Shape(const Shape& shape,unsigned int mode)
{
	if(shape.mesh)
		mesh = shape.mesh;
	indicesNum = shape.indicesNum;
	//tex = shape.tex;
	isCopy = true;
	this->mode = mode;
}

Shape::Shape(const std::string& fileName, unsigned int mode){
	mesh = new MeshConstructor(0,vao, &indicesNum);
	isCopy = false;
	this->mode = mode;
}

Shape::Shape(const int SimpleShapeType, unsigned int xResolution,unsigned int yResolution,unsigned int mode)
{
	mesh = new MeshConstructor(SimpleShapeType,vao,&indicesNum);
	//mesh->Bind();
	this->mode = mode;
	isCopy = false;
}

void Shape::AddTexture(const std::string& textureFileName)
{
	tex = new Texture(textureFileName);
}


void Shape::Draw( const Shader& shader)
{
//	if(tex)
//		tex->Bind();

	shader.Bind();
	vao.Bind();

	GLCall(glDrawElements(mode,indicesNum, GL_UNSIGNED_INT, 0));
	vao.Unbind();
}

Shape::~Shape(void)
{
	if(!isCopy)
	{
		if(mesh)
 			delete mesh;
		
		if(tex)
			delete tex;
	}
}

