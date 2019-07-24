#pragma once
#include "scene.h"
#include "snakeMoveTracker.H"
#include "levelParser.h"

static inline void printVec(const glm::vec3 vec)
{
	printf("%f %f %f\n", vec.x, vec.y, vec.z);
}


static inline void printVec(const glm::vec4 vec)
{
	printf("%f %f %f %f\n", vec.x, vec.y, vec.z, vec.w);
}

static void printMat(const glm::mat4 mat4)
{
	printf("[\n");
	for (int i = 0; i < 4; i++)
		printVec(mat4[i]);
	printf("]\n");
}

static const vec3 xAx(1, 0, 0);		static const vec3 yAx(0, 1, 0);		static const vec3 zAx(0, 0, 1);
class Game : public Scene
{
	const int snakeLength = 25, segs = 5, ends = 10;
	int projMode = -1;
public:
	glm::vec3 tailDirection;	glm::vec3 headDirection;
	glm::vec3 headCurLocation;	glm::vec3 snakeMid;
	bool cameramode = false;
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	~Game(void);
	glm::mat4 setSnakeNodesAnglesAndGetHead();
	void genSnake(float x, float y, float z, int direction);
	void specialObjHandle(objLocation &obj);
	void genObj(const char * ptr, int tex, vec3 startLoc, float scale, int direction);
	void Init();
	void playerInput(bool direction);
	void changeDirPInput(bool dir);
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
	void Debug();//will print data of current debbuging
	void changeMode() {
		projMode = (projMode + 1) % 6;
		updateDrawMode(projMode);
	}
private:
	snakeMoveTracker *sMT;

	void getSegs(float *lastX, float mult, float sign, float jumpX, float jumpY, int segs);
	void getHeadSegs(float *lastX, float jumpX, float jumpY, int segs);
	void getTailSegs(float *lastX, float jumpX, float jumpY, int segs);
	void getBodySegs(float *lastX, float jumpX, float jumpY, int segs);

	void addCubes();
	void configSound();
	void orderCamera();
};

