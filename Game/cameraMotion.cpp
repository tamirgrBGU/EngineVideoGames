#include "cameraMotion.h"

static inline glm::vec4 v3to4(glm::vec3 v) {
	return glm::vec4(v.x, v.y, v.z, 1);
}

static inline glm::vec3 v4to3(glm::vec4 v) {
	return glm::vec3(v.x, v.y, v.z);
}

static inline float sizeOfVec(glm::vec3 v) {
	return abs(v.x) + abs(v.y) + abs(v.z);
}

/*static inline float abs(float a) {
	return a < 0 ? -a : a;
}*/


static const float frame = 20.f;
static const float bigframe = 2*frame;
static const float tolerance = 0.04f;
static const float bigTolerance = tolerance*3;

bool snakeviewmode = false;
bool animateMovement = false;

Game *myCam = nullptr;
MovableGLM *tail  = nullptr;

inline glm::vec3 getMid() {
	return (myCam->headCurLocation + v4to3(tail->makeTrans()[3]))*0.5f;
}


static float const heightDown = 10;
float heightTop;
void initCameraMotion(Game *obj, MovableGLM *tailp, float z) {
	myCam = obj;
	tail = tailp;
	myCam->snakeMid = getMid();
	heightTop = z;
}

inline void animate() {
	if (snakeviewmode)
		myCam->myTranslate(glm::vec3(0, 0, 1), 0);
	else {
		glm::vec3 diff = getMid() - myCam->snakeMid;
		if (sizeOfVec(diff) > bigframe)
			diff = diff * bigTolerance;
		else {
			if (sizeOfVec(diff) < frame)
				animateMovement = false;
			diff = diff * tolerance;
		}

		myCam->snakeMid = myCam->snakeMid + diff;
		myCam->myTranslate(-diff, 0);
	}
}
void updateCam() {
	if (animateMovement) {
		animate();
	}
	else {
		glm::vec3 newMid = getMid();
		glm::vec3 diff = myCam->snakeMid - newMid;
		if (sizeOfVec(diff) > frame)
			animateMovement = true;		
	}
}

void switchCamMode() {
	snakeviewmode = !snakeviewmode;

	//myCam->printRot(true);
	//myCam->printRot(false);

	vec3 camLoc = v4to3(myCam->getTraslate());

	printf("switch pressed\n");
	printVec(camLoc);
	printVec(myCam->headDirection);

	myCam->myTranslate(-camLoc, 0);
	myCam->resetEuler();
	myCam->doRotate(mat4(1));
	if (snakeviewmode) {
		camLoc = myCam->headCurLocation;
		float angle = glm::degrees(glm::angle(yAx, myCam->headDirection));
		if (myCam->headDirection.x < 0)
			angle = -angle;
		myCam->myRotate(angle, zAx, 4);		printf("%f\n",angle);
		myCam->myRotate(90.f, glm::cross(zAx, myCam->headDirection), 4);
		float tempZ = camLoc.z;
		camLoc.z = 0;
		camLoc.z = -glm::length(camLoc);
		camLoc.x = 0;
		camLoc.y = tempZ + heightDown;
		//camLoc.z += heightDown;
	}
	else {
		camLoc = myCam->snakeMid;
		camLoc.z += heightTop;
	}
	printVec(camLoc);
	myCam->myTranslate(-camLoc, 0);
}

//todo validate D
//update on key press when in snake mode and order camera movement