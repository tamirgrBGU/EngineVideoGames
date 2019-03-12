#pragma once
#include "scene.h"

class Game : public Scene
{
public:
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void Init();
//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void Update( glm::mat4 MVP ,glm::mat4 Normals,Shader* s);
	
	~Game(void);
};

