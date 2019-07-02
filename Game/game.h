#pragma once
#include "scene.h"
#include "MeshConstructor.h"
#include "bezier1D.h"
#include "Bezier2D.h"
#include "levelParser.h"

class Game : public Scene
{
	Bezier1D *curve;
	std::vector<int> controlPointsShapesIds;
	vec3 pastLoc[7];
	bool proj2D = false;
	int projMode = -1;
	unsigned int xResolution = 15, yResolution = 15;
	float movement_ = 0;
public:
	bool cameramode = false;
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void genSnake(float x, float y, float z, int direction);
	void specialObjHandle(objLocation &obj);
	void Game::genObj(const char * ptr, int tex, float x, float y, float z, int direction);
	void Init();
	void playerInput(bool direction);
	void addShape(int type,int parent,unsigned int mode);
	void addShape(IndexedModel model, int parent, unsigned int mode, int tex, int shader);
	void updateIntersectors(unsigned int mode);
//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void UpdateLinear(const glm::mat4 &lastMVP, const glm::mat4 &MVP, const glm::mat4 &nextMVP, const glm::mat4 &Normal, const int  shaderIndx);
	void UpdateQuaternion(const glm::mat2x4 &lastMVP, const glm::mat2x4 &MVP, const glm::mat2x4 &nextMVP, const glm::mat4 &Normal, const int  shaderIndx);
	void Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int  shaderIndx);
	void WhenRotate();
	void WhenTranslate();
	Bezier1D *getBezier1D();
	void savePastPositions(int controlPoint);
	void updateControlShapes(int controlPoint, Bezier1D *bez);
	void Motion();
	void changeCameraMode();
	void Game::PlayTheme();
	void Game::PlayPoint();
	void Game::PlayExplosion();
	void Game::changeMode() {
		projMode = (projMode + 1) % 6;
		updateIntersectors(projMode);
	}
	
	void Game::change_movement(float num) {
		movement_ += num;
	}

	~Game(void);
private:

	void Game::getSegs(float *lastX, float mult, float sign, float jumpX, float jumpY, int segs);
	void Game::getHeadSegs(float *lastX, float jumpX, float jumpY, int segs);
	void Game::getTailSegs(float *lastX, float jumpX, float jumpY, int segs);
	void Game::getBodySegs(float *lastX, float jumpX, float jumpY, int segs);
};

