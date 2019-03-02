#include "shape.h"
#include "Log.hpp"
#include "MeshConstructor.h"

Shape::Shape(const Shape& shape)
{
	if(shape.mesh)
		mesh = shape.mesh;
	verticesNum = shape.verticesNum;
	tex = shape.tex;
	isCopy = true;
}

Shape::Shape(const std::string& fileName){
	mesh = new VertexArray();
	isCopy = false;
}

Shape::Shape(const int SimpleShapeType, unsigned int xResolution,unsigned int yResolution)
{
	mesh = new VertexArray();
	mesh->Bind();
	verticesNum = MeshConstructor(SimpleShapeType,*mesh).GetCount();
	mesh->Unbind();
	isCopy = false;
}

void Shape::AddTexture(const std::string& textureFileName)
{
	tex = new Texture(textureFileName);
}


void Shape::Draw(int mode, const Shader& shader)
{
	if(tex)
		tex->Bind();

	shader.Bind();
	mesh->Bind();
	GLCall(glDrawElements(mode,verticesNum, GL_UNSIGNED_INT, nullptr));
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

