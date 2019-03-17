#define GLEW_STATIC
#include <GL\glew.h>
#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "bezier2D.h"


MeshConstructor::MeshConstructor(const int type,VertexArray &vao,int *indicesNum)
{
	
	switch (type)
	{
	case Axis:
		
		*indicesNum = initLine(AxisGenerator(),vao);
		break;
	case Cube:
		*indicesNum = initMesh(CubeTriangles(),vao);
	case BezierLine:
		//Bezier1D line();
		//*indicesNum = initLine( line.GetLine(30,30),vao);
		break;
	case BezierSurface:
		//Bezier1D line();
		//Bezier2D surface(line,glm::vec3(0,0,1),4);
		//*indicesNum = initMesh( surface.GetSurface(30,30),vao);
		break;
	default:
		break;
	}
	
}

MeshConstructor::~MeshConstructor(void)
{
	if(ib)
		delete ib;
}

int MeshConstructor::initLine(IndexedModel &model,VertexArray &vao){
	
	int verticesNum = model.positions.size();
	int indicesNum = model.indices.size();
	std::vector<VertexBuffer*> vbs;
	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i),verticesNum*sizeof(model.positions[0])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	
	ib = new IndexBuffer((unsigned int*)model.GetData(4),indicesNum);
	
	vao.Unbind();

	for (int i = 0; i < vbs.size(); i++)
	{
		delete vbs[i];
	}
	return indicesNum;
}

int MeshConstructor::initMesh( IndexedModel &model,VertexArray &vao){

	int verticesNum = model.positions.size();
	int indicesNum = model.indices.size();
	std::vector<VertexBuffer*> vbs;
	vao.Bind();

	for (int i = 0; i < 3; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i),verticesNum*sizeof(model.positions[0])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	vbs.push_back(new VertexBuffer(model.GetData(3),verticesNum*sizeof(model.texCoords[0])));
	vao.AddBuffer(*vbs.back(),vbs.size()-1,2,GL_FLOAT);
	
	ib = new IndexBuffer((unsigned int*)model.GetData(4),indicesNum);
	
	vao.Unbind();

	for (int i = 0; i < vbs.size(); i++)
	{
		delete vbs[i];
	}
	return indicesNum;
}

