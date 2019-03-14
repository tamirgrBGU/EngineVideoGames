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
	unsigned int mode;
	
public:

	Shape(const Shape& shape,unsigned int mode);

	Shape(const std::string& fileName,unsigned int mode);
	
	Shape(const int SimpleShapeType, unsigned int xResolution,unsigned int yResolution,unsigned int mode);

	void AddTexture(const std::string& textureFileName);

	void Draw( const Shader& shader);

	virtual ~Shape(void);
};

