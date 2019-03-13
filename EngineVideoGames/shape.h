#pragma once
#include "MovableGLM.h"
#include "VertexArray.hpp"
#include "shader.h"
#include "MeshConstructor.h"

#include "texture.h"

class Shape : public MovableGLM
{
private:

	MeshConstructor *mesh;
	int indicesNum;
	Texture *tex;
	VertexArray vao;
	bool isCopy;
	unsigned int renderID;
	int mode;
public:

	Shape(const Shape& shape);

	Shape(const std::string& fileName);
	
	Shape(const int SimpleShapeType, unsigned int xResolution,unsigned int yResolution);

	void AddTexture(const std::string& textureFileName);

	void Draw( const Shader& shader);

	virtual ~Shape(void);
};

