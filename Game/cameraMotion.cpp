#include "cameraMotion.h"

static inline glm::vec4 v3to4(glm::vec3 v) {
	return glm::vec4(v.x, v.y, v.z, 1);
}

static inline glm::vec4 v3to40(glm::vec3 v) {
	return glm::vec4(v.x, v.y, v.z, 0);
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


static const float angleFrame = 6.f;
static const float bigAngleFrame = 1.5f * angleFrame;

static const float frame = 12.f;
static const float bigframe = 2*frame;
static const float tolerance = 0.07f;
static const float bigTolerance = tolerance*3;

bool snakeviewmode = false;
bool animateMovement = false;

Game *myCam = nullptr;
MovableGLM *tail = nullptr;

inline glm::vec3 getMid() {
	return snakeviewmode ? myCam->headCurLocation : myCam->midCurLocation;
}


static float const heightDown = 7;
float heightTop;
vec3 lastheadDirection;
vec3 realcampos;
vec3 snakeMid;
void initCameraMotion(Game *obj, MovableGLM *tailp, float z) {
	myCam = obj;
	tail = tailp;
	snakeMid = getMid();
	heightTop = z;
}

void setCameraTopView() {
	snakeviewmode = true;
	switchCamMode();
}

void setCameraBottomView() {
	snakeviewmode = false;
	switchCamMode();
}

float mypi = 3.14159265358979323846264338327950288f;
inline static void completeAngle(float &angle) {
	if(angle > 0)
		angle = 2 * mypi - angle;
	else
		angle = -(2 * mypi + angle);
}

//both vecs are normalized: (a*b)/(|a|*|b|) = cos(ang) -> ang = acos(a*b)
static float anglewNonAbs(const vec3& a, const vec3& b) {
	//unormalized vec3 can cause nans...	//vec3 normA = glm::normalize(a);	//vec3 normB = glm::normalize(b);
	//for normalized vectors - v*u/(|v|*|u|) = v*u
	float angleA = glm::acos(glm::clamp(glm::dot(yAx, a), -1.f, 1.f));
	float angleB = glm::acos(glm::clamp(glm::dot(yAx, b), -1.f, 1.f));

	if (a.x < 0)
		completeAngle(angleA);
	if (b.x < 0)
		completeAngle(angleB);

	/*printf("%f %f ", angleA, angleB);
	printVec(a);
	printVec(b);*/

	float angle = angleB - angleA;
	if (angle > mypi || angle < -mypi){
		completeAngle(angle);
		angle = -angle;
	}

	return angle;
}

void rotCam() {
	float dirAngle = glm::degrees(anglewNonAbs(lastheadDirection, myCam->headDirection));
	float angle = abs(dirAngle);
	mat4 rotator(1);

	//printf("%f\n", dirAngle);
	if (angle > bigAngleFrame) {
		myCam->myRotate(dirAngle*bigTolerance, zAx, 4);
		rotator = glm::rotate(dirAngle*bigTolerance, zAx);
	}
	else if (angle > angleFrame) {
		myCam->myRotate(dirAngle*tolerance, zAx, 4);
		rotator = glm::rotate(dirAngle*tolerance, zAx);
	}
	else
		return;
	lastheadDirection = v4to3(v3to40(lastheadDirection)*rotator);

	myCam->myTranslate(-v4to3(myCam->getTraslate()), 0);
	myCam->myTranslate(-v4to3(myCam->GetRot() * v3to4(realcampos)), 0);
}

void animate() {
	glm::vec3 diff = getMid() - snakeMid;
	if (sizeOfVec(diff) > bigframe)
		diff = diff * bigTolerance;
	else {
		if (sizeOfVec(diff) < frame)
			animateMovement = false;
		diff = diff * tolerance;
	}

	snakeMid = snakeMid + diff;
	realcampos = realcampos + diff;
	myCam->myTranslate(-v4to3(myCam->GetRot()*v3to4(diff)), 0);	
}

void updateCam() {
	if (snakeviewmode)
		rotCam();
	
	if (animateMovement)
		animate();	
	else {
		glm::vec3 newMid = getMid();
		glm::vec3 diff = snakeMid - newMid;
		if (sizeOfVec(diff) > frame)
			animateMovement = true;		
	}
}

vec3 orderCamSnakeEyeMode() {
	vec3 camLoc = myCam->headCurLocation;
	camLoc.z += heightDown;

	//printVec(myCam->headCurLocation);
	//printVec(myCam->headDirection);

	float angle = glm::degrees(anglewNonAbs(yAx, myCam->headDirection));
	//printf("%f\n", angle);
	myCam->myRotate(angle, zAx, 4);
	myCam->myRotate(90.f, glm::cross(zAx, myCam->headDirection), 4);

	lastheadDirection = myCam->headDirection;

	realcampos = camLoc;
	return v4to3(myCam->GetRot()*v3to4(camLoc));
}

inline void getCamCorrection(vec3& camLoc) {
	if (snakeviewmode) {
		camLoc = orderCamSnakeEyeMode();
	}
	else {
		myCam->resetEuler();
		myCam->doRotate(mat4(1));
		camLoc = snakeMid;
		camLoc.z += heightTop;
	}
}

void switchCamMode() {
	snakeviewmode = !snakeviewmode;
	vec3 camLoc;
	snakeMid = getMid();
	getCamCorrection(camLoc);
	
	//reset camera to (0,0,0) position, view from z to xy plane
	myCam->myTranslate(-v4to3(myCam->getTraslate()), 0);
	//repositioning
	myCam->myTranslate(-camLoc, 0);
}

//todo validate D
//update on key press when in snake mode and order camera movement