#pragma once
#include "scene.h"
#include "snakeMoveTracker.H"
#include "levelTheme.h"
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
	const int snakeLength = 30, segs = 5, ends = 10;
	int projMode = -1;
	bool rotRecently = false;
	const float slowSpeed = 1.5f;
	const float speed = 2.f;
	float superSpeedTicks = 0;
	const float superSpeed = 2.5f;
public:
	glm::vec3 tailDirection;	glm::vec3 headDirection;
	glm::vec3 headCurLocation;	glm::vec3 midCurLocation;
	glm::mat4 headTransMAT;
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	~Game(void);
	void Init();

	void addShape(int type, int parent, unsigned int mode);
	void addShape(IndexedModel model, int parent, unsigned int mode, int tex, int shader);

	void updateDrawMode(unsigned int mode);
	void UpdateLinear(const glm::mat4 &lastMVP, const glm::mat4 &MVP, const glm::mat4 &nextMVP, const glm::mat4 &Normal, const int shaderIndx);
	void UpdateQuaternion(const glm::mat2x4 &lastMVP, const glm::mat2x4 &MVP, const glm::mat2x4 &nextMVP, const glm::mat4 &Normal, const int shaderIndx);
	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const int shaderIndx);

	void WhenRotate();
	void WhenTranslate();
	void Motion();

	void Debug();
	void playerInput(bool direction);
	void changeDirPInput(bool dir);
	void changeCameraMode();
	void changeMode() {
		projMode = (projMode + 1) % 6;
		updateDrawMode(projMode);
	};
	
	bool onIntersectSnakeHead(int type, Shape *myShape);

	//to use outside (in main integration with imgui)
	void switchSoundEnable();
	int getCurrentLevel();
	int getTotalLevelCount();
	int getCurrentFruitCount();
	int getTotalFruitCount();
	bool getSoundVar();
	void playWelcome() {
		PlaySoundGame(Hiss);
	}
	void PlayTheme() {
		PlaySoundGame(Theme);
	}

	bool wonGame = false;
	bool isLoading = false;
	int currentTheme = 0;
	void loadNextLevel();

private:
	bool soundEnable = true;

	enum Sounds { ObstecleSound, FruitSound, Win, Hiss, Theme };
	void PlaySoundGame(int type);

	snakeMoveTracker *sMT;
	bool onIntersectCave	(Shape *s);
	void onIntersectObstecle(Shape *s);
	void onIntersectFruit	(Shape *s);
	void onIntersectWalls	(Shape *s);
	void onIntersectFallWall(Shape *s);
	void onIntersectStairs	(Shape *s);

	int currentLvl = 5;
	const int maxGameLvl = 5;
	leveGenerator *lGen;
	void Game::setupEnvironment();
	void genSky(float widthOfMap);
	inline void genSkyCubeHelper(vec3 a1, vec3 a2, vec3 a3, vec3 a4,
		vec3 a5, vec3 a6, vec3 a7, vec3 a8);
	inline void genSkyHelper(float xl, float xh, float yl, float yh, float zl, float zh);

	void resetCurrentLevel();
	void resetSnake();

	void loadThemes();
	void setupCurrentLevel();
	void changeTheme();
	ThemeHolder *themes;
	void updateThemeArrays();
	
	int fruitCounter;
	std::vector<Shape *> fruitsVec;
	void fruitMotion();
	void snakeFaceMotion();

	inline void orderGenObj(vec3 startLoc, float scale, int direction);
	inline void addShapeAndKD(int myIndex, int tex, float x, float y, vec3 pos, int level, float scale, int dir);

	inline void getSegsHelper(std::vector<glm::mat4>& segments, float& lastX, float mult, const float sign, const float jumpX, const float jumpY, const int segs);
	void getSegs(float& lastX, float mult, float sign, float jumpX, float jumpY, int segs);
	void orderSegPart(float segLen);
	void getTailSegs(float& lastX, float jumpX, float jumpY, int segs);
	void getBodySegs(float& lastX, float jumpX, float jumpY, int segs, int amount);
	void getHeadSegs(float& lastX, float jumpX, float jumpY, int segs);
	void genTongue(int pa);
	void genEyes(float width, int pa);

	int snakeLevel;
	int snakeTongue = - 1;
	int snakeNodesShapesStart = -1;
	int snakeNodesShapesEnd = -1;
	float snakeFullLength;

	void setSnakeNodesAngles();
	void updateSnakePosition();

	void genSnake(float x, float y, float z, int direction);
	void putSnakeInPlace(float xLoc, float yLoc, float zLoc, int direction);
	void specialObjHandle(objLocation &obj);
	void genObj(int ptrIndx, int tex, vec3 startLoc, float scale, int direction);

	void addCubes();
	void configSound();
	void orderCameraTop();
	void setUpCamera();

	int fruitMotionCounter;
	int fruitMotionDir;
	vec3 motionJumps;
	void setFruitMotion() {
		fruitMotionCounter = 1;
		fruitMotionDir = -1;
		motionJumps = -vec3(0.8f);
	}
};

