#pragma once
#include "MovableGLM.h"
#include "mesh.h"
#include "lineMesh.h"
#include "texture.h"
#include "VertexArray.hpp"
class Shape : public MovableGLM
{
	Mesh *mesh;
	LineMesh *lineMesh;
	Texture *tex;
	VertexArray *vao;
	bool isCopy;
public:
	enum{triangles,lines};
	Shape(const Shape& shape);
	Shape(const std::string& fileName,VertexArray *_vao);
	Shape(const std::string& fileName,const std::string& textureFileName,VertexArray *_vao);
	Shape(int CylParts, int linkPosition,VertexArray *_vao);
	Shape(int CylParts, int linkPosition,const std::string& textureFileName,VertexArray *_vao);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,VertexArray *_vao);
	Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,VertexArray *_vao);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string& textureFileName,VertexArray *_vao);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void draw(int mode);

	virtual ~Shape(void);
};

