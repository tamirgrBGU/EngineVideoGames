#define GLEW_STATIC
#include <GL\glew.h>
#include "shape.h"
#include "Log.hpp"


Shape::Shape(const Shape& shape,unsigned int mode)
{
	
	mesh = new MeshConstructor(*shape.mesh);
	//tex = shape.tex;
	isCopy = true;
	this->mode = mode;
}

Shape::Shape(const std::string& fileName, unsigned int mode){
	mesh = new MeshConstructor(fileName);
	isCopy = false;
	this->mode = mode;
}

Shape::Shape(const int SimpleShapeType,unsigned int mode)
{
	mesh = new MeshConstructor(SimpleShapeType);
	//mesh->Bind();
	this->mode = mode;
	isCopy = false;
}

Shape::Shape(Bezier1D *curve, unsigned int xResolution,unsigned int yResolution,bool is2D,unsigned int mode)
{
	mesh = new MeshConstructor(curve,is2D,xResolution,yResolution);
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
	mesh->Bind();
	/*if(isCopy)
		glDrawArrays(GL_TRIANGLES, 0, indicesNum);
	else*/
	GLCall(glDrawElements(mode,mesh->GetIndicesNum(), GL_UNSIGNED_INT, 0));
	mesh->Unbind();
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

