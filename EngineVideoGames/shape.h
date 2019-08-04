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
	int texID;
	int shaderID;
	bool isCopy;
	bool toRender;

public:
	unsigned int mode;

	Shape(const IndexedModel& model, unsigned int mode, int textureID, int shaderID);

	Shape(const Shape& shape, unsigned int mode);

	Shape(const std::string& fileName,unsigned int mode, int textureID, int shaderID);
	
	Shape(const int SimpleShapeType,unsigned int mode);

	Shape(Bezier1D *curve, unsigned int xResolution,unsigned int yResolution,bool is2D,unsigned int mode);

	void Draw( const std::vector<Shader*> shaders, const std::vector<Texture*> textures,bool isPicking);

	inline void Hide() {toRender = false;}

	inline void Unhide() {toRender = true;}

	inline bool Is2Render() {return toRender;}

	inline bool Is2D(){return mesh->Is2D();}

	inline void SetTexture(int id){ texID = id;}

	inline void SetShader(int id){ shaderID = id;}

	inline int GetShader(){return shaderID;}

	virtual ~Shape(void);
};

