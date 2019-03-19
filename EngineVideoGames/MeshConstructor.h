#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"
#include "bezier1D.h"


class MeshConstructor
{
	IndexBuffer *ib;
	IndexedModel model;
	
	int initLine(IndexedModel &model,VertexArray &vao);
	int initMesh(IndexedModel &model,VertexArray &vao);
public:
	enum SimpleShapes
	{
		Axis,
		Cube,
		Octahedron,
		Tethrahedron,
		BezierLine,
		BezierSurface,
	};
	MeshConstructor(const int type, VertexArray &vao,int *indicesNum);
	MeshConstructor(Bezier1D *curve,bool isSurface,unsigned int resT,unsigned int resS,VertexArray &vao,int *indicesNums);
	inline unsigned int GetCount(){return ib->GetCount();}
	~MeshConstructor(void);

};

