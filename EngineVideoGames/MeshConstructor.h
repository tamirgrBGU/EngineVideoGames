#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"


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
	void initMesh(int verticesNum, int indicesNum, VertexArray &vao);
public:
	
	MeshConstructor(const int type, VertexArray &vao,unsigned int &ibNum);
	inline unsigned int GetCount(){return ib->GetCount();}
	~MeshConstructor(void);
};

