#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"
#include "bezier1D.h"
#include "kdtree.h"


class MeshConstructor
{
	VertexArray vao;
	IndexBuffer *ib;
	//TO DO: add bounding box data base and build it in the constructor 
	std::vector<VertexBuffer*> vbs;
	bool is2D;
	int unsigned indicesNum;
	
	void InitLine(IndexedModel &model);
	void InitMesh(IndexedModel &model);
	void CopyMesh(const MeshConstructor &mesh);
	void CopyLine(const MeshConstructor &mesh);
	
	
public:
	//TO DO: add collision detection function which get other MeshConstructor and Mat4 of related transformasions. The function may return a pointer to the relevant Bounding Box when collide
	enum SimpleShapes
	{
		Axis,
		Cube,
		Octahedron,
		Tethrahedron,
		BezierLine,
		BezierSurface,
	};
	MeshConstructor(const int type);
	MeshConstructor(Bezier1D *curve,bool isSurface,unsigned int resT,unsigned int resS);
	MeshConstructor(const MeshConstructor &mesh);
	MeshConstructor(const std::string& fileName);

	void Bind() {vao.Bind();}
	void Unbind() {vao.Unbind();}
	inline unsigned int GetIndicesNum(){return indicesNum;}
	~MeshConstructor(void);

};

