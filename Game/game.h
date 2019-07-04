#pragma once
#include "scene.h"
#include "MeshConstructor.h"
#include "bezier1D.h"
#include "Bezier2D.h"
#include "levelParser.h"
//#include "IntersectTracker.h"
#include <windows.h>
#include <iostream>  
#include <thread>

class Game : public Scene
{
	int projMode = -1;
public:
	bool cameramode = false;
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	~Game(void);
	void setSnakeNodesAngles();
	void genSnake(float x, float y, float z, int direction);
	void specialObjHandle(objLocation &obj);
	void genObj(const char * ptr, int tex, vec3 startLoc, float scale, int direction);
	void Init();
	void playerInput(bool direction);
	void addShape(int type, int parent, unsigned int mode);
	void addShape(IndexedModel model, int parent, unsigned int mode, int tex, int shader);
	void updateDrawMode(unsigned int mode);
	//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void UpdateLinear(const glm::mat4 &lastMVP, const glm::mat4 &MVP, const glm::mat4 &nextMVP, const glm::mat4 &Normal, const int  shaderIndx);
	void UpdateQuaternion(const glm::mat2x4 &lastMVP, const glm::mat2x4 &MVP, const glm::mat2x4 &nextMVP, const glm::mat4 &Normal, const int  shaderIndx);
	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx);
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	void changeCameraMode();
	void PlayTheme();
	void PlayPoint();
	void PlayExplosion();
	void changeMode() {
		projMode = (projMode + 1) % 6;
		updateDrawMode(projMode);
	}
private:
	void getSegs(float *lastX, float mult, float sign, float jumpX, float jumpY, int segs);
	void getHeadSegs(float *lastX, float jumpX, float jumpY, int segs);
	void getTailSegs(float *lastX, float jumpX, float jumpY, int segs);
	void getBodySegs(float *lastX, float jumpX, float jumpY, int segs);
};

