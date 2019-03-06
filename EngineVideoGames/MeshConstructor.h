#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "obj_loader.h"

//class IndexedModel
//{
//public:
//    std::vector<glm::vec3> positions;
//    std::vector<glm::vec2> texCoords;
//    std::vector<glm::vec3> normals;
//	std::vector<glm::vec3> colors;
//	std::vector<glm::vec3> weights;
//	std::vector<glm::ivec3> joint_indices;
//    std::vector<unsigned int> indices;
//    
//    void CalcNormals();
//};

	struct Vertex
	{
	public:
		Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal, const glm::vec3& color)
		{
			this->pos = pos;
			this->texCoord = texCoord;
			this->normal = normal;
			this->color = color;
		}

		glm::vec3* GetPos() { return &pos; }
		glm::vec2* GetTexCoord() { return &texCoord; }
		glm::vec3* GetNormal() { return &normal; }
		glm::vec3* GetColor() { return &color; }
	
	private:
		glm::vec3 pos;
		glm::vec2 texCoord;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec3 weight;
	};

	struct LineVertex
	{
	public:
		LineVertex(const glm::vec3& pos, const glm::vec3& color)
		{
			this->pos = pos;
			this->color = color;
		}

		glm::vec3* GetPos() { return &pos; }
		glm::vec3* GetColor() { return &color; }
	
	private:
		glm::vec3 pos;
		glm::vec3 color;
	};

class MeshConstructor
{
	IndexBuffer *ib;
	IndexedModel model;
	enum SimpleShapes
	{
		Axis,
		Cube,
		Octahedron,
		Tethrahedron
	};
	unsigned int initLine(int verticesNum, int indicesNum, VertexArray &vao);
	unsigned int initMesh(int verticesNum, int indicesNum, VertexArray &vao);
public:
	
	MeshConstructor(const int type, VertexArray &vao,unsigned int &ib);
	inline int GetCount(){return ib->GetCount();}
	~MeshConstructor(void);
};

