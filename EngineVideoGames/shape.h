#pragma once
#include "MovableGLM.h"
#include "VertexArray.hpp"
#include "shader.h"
#include "IndexBuffer.hpp"
#include "MeshConstructor.h"
#include "obj_loader.h"
#include "texture.h"

class Shape : public MovableGLM
{
private:

	VertexArray *mesh;
	int verticesNum;
	Texture *tex;

	bool isCopy;
	unsigned int renderID;
public:

	Shape(const Shape& shape);

	Shape(const std::string& fileName);
	
	Shape(const int SimpleShapeType, unsigned int xResolution,unsigned int yResolution);

	void AddTexture(const std::string& textureFileName);

	void Draw(int mode, const Shader& shader);

	virtual ~Shape(void);
};

