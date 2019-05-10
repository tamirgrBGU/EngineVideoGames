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
	Texture *tex;
	bool isCopy;
	bool toRender;

public:
	unsigned int mode;

	Shape(const IndexedModel& model, unsigned int mode);

	Shape(const Shape& shape,unsigned int mode);

	Shape(const std::string& fileName,unsigned int mode);
	
	Shape(const int SimpleShapeType,unsigned int mode);

	Shape(Bezier1D *curve, unsigned int xResolution,unsigned int yResolution,bool is2D,unsigned int mode);

	void AddTexture(const std::string& textureFileName);

	void Draw( const Shader& shader);

	inline void Hide() {toRender = false;}

	inline void Unhide() {toRender = true;}

	inline bool Is2Render() {return toRender;}

	inline bool Is2D(){return mesh->Is2D();}

	virtual ~Shape(void);
};

