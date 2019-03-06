#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"

	LineVertex axisVertices[] = 
	{
		LineVertex(glm::vec3(1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(-1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(0,1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,-1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,0,1),glm::vec3(0,0,1)),
		LineVertex(glm::vec3(0,0,-1),glm::vec3(0,0,1)),
	};


	 unsigned int axisIndices[] = 
	{
		0,1,
		2,3,
		4,5
	};

	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};



	unsigned int indices[] =	{0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	                          };

MeshConstructor::MeshConstructor(const int type,VertexArray &vao,unsigned int &ib)
{
	
	switch (type)
	{
	case Axis:
		ib = initLine(6,6,vao);
		break;
	case Cube:
		ib = initMesh(24, 36,vao);
		break;
	default:
		break;
	}
	
}


MeshConstructor::~MeshConstructor(void)
{
}

unsigned int MeshConstructor::initLine(int verticesNum, int indicesNum,VertexArray &vao ){
	IndexedModel model;
	VertexBuffer vbs(axisVertices,sizeof(LineVertex)*6);
	//for(unsigned int i = 0; i < verticesNum; i++)
	//{
	//	model.positions.push_back(*axisVertices[i].GetPos());
	//	model.colors.push_back(*axisVertices[i].GetColor());
	//}
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	
	vao.AddBuffer(vbs,layout);
	
	//for(unsigned int i = 0; i < indicesNum; i++)
    //    model.indices.push_back(axisIndices[i]);
	IndexBuffer *ib = new IndexBuffer(indices,indicesNum);
	//ib = new IndexBuffer(axisIndices,indicesNum);
	return ib->GetRender();
}

unsigned int MeshConstructor::initMesh(int verticesNum, int indicesNum,VertexArray &vao){
    IndexedModel model;
	std::vector<VertexBuffer> vbs;
	for(unsigned int i = 0; i < verticesNum; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
		model.normals.push_back(*vertices[i].GetNormal());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
	}
	vbs.push_back(VertexBuffer(&model.positions[0],verticesNum*sizeof(model.positions[0])));
	vbs.push_back(VertexBuffer(&model.colors[0],verticesNum*sizeof(model.colors[0])));
	vbs.push_back(VertexBuffer(&model.normals[0],verticesNum*sizeof(model.normals[0])));
	vbs.push_back(VertexBuffer(&model.texCoords[0],verticesNum*sizeof(model.texCoords[0])));
	//VertexBufferLayout layout;
	for (int i = 0; i < vbs.size()-1; i++)
	{
		//layout.Push<float>(3);
		vao.AddBuffer(vbs[i],i,3,GL_FLOAT);
	}
	vao.AddBuffer(vbs.back(),vbs.size()-1,2,GL_FLOAT);
	for(unsigned int i = 0; i < indicesNum; i++)
        model.indices.push_back(indices[i]);
	
	IndexBuffer *ib = new IndexBuffer(indices,indicesNum);
	return ib->GetRender();
}