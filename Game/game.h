#pragma once
#include "scene.h"
#include "bezier1D.h"

class Game : public Scene
{
	Bezier1D *curve;
public:
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void Init();
	void addShape(int type,int parent,unsigned int mode);
//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void Update( glm::mat4 MVP ,glm::mat4 Normals,Shader* s);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	~Game(void);
};

