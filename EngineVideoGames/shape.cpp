#define GLEW_STATIC
#include <GL\glew.h>
#include "shape.h"
#include "Log.hpp"


Shape::Shape(const Shape& shape)
{
	if(shape.mesh)
		mesh = shape.mesh;
	indicesNum = shape.indicesNum;
	//tex = shape.tex;
	isCopy = true;
	mode = shape.mode;
}

Shape::Shape(const std::string& fileName){
	mesh = new MeshConstructor(0,vao, &indicesNum);
	isCopy = false;
	mode = GL_TRIANGLES;
}

Shape::Shape(const int SimpleShapeType, unsigned int xResolution,unsigned int yResolution)
{
	mesh = new MeshConstructor(SimpleShapeType,vao,&indicesNum);
	//mesh->Bind();
	if(SimpleShapeType == 0)
		mode = GL_LINES;
	else
		mode = GL_TRIANGLES;
	//mesh=new Mesh();
//	mesh->Unbind();
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

