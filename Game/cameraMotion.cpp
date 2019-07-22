#include "cameraMotion.h";

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


void initCameraMotion(Game *obj, MovableGLM *tailp) {
	myCam = obj;
	tail = tailp;
	myCam->snakeMid = getMid();
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
}