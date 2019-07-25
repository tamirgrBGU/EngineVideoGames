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
static const float bigAngleFrame = 2 * angleFrame;

static const float frame = 12.f;
static const float bigframe = 2*frame;

static const float tolerance = 0.07f;
static const float bigTolerance = tolerance*3;

bool snakeviewmode = false;
bool animateMovement = false;

Game *myCam = nullptr;
MovableGLM *tail  = nullptr;

inline glm::vec3 getMid() {
	return snakeviewmode ? myCam->headCurLocation : (myCam->headCurLocation + v4to3(tail->makeTrans()[3]))*0.5f;
}


static float const heightDown = 7;
float heightTop;
vec3 lastheadDirection;
vec3 realcampos;
void initCameraMotion(Game *obj, MovableGLM *tailp, float z) {
	myCam = obj;
	tail = tailp;
	myCam->snakeMid = getMid();
	heightTop = z;
}

float mypi = 3.14159265358979323846264338327950288f;
inline static void completeAngle(float &angle) {
	angle = 2 * mypi - angle;
}

//both vecs are normalized: (a*b)/(|a|*|b|) = cos(ang) -> ang = acos(a*b)
static float anglewNonAbs(const vec3& a, const vec3& b) {
	float angleA = glm::angle(yAx, a);
	float angleB = glm::angle(yAx, b);
	//printf("%f - %f\n", glm::degrees(angleB), glm::degrees(angleA));
	if (a.x < 0)
		completeAngle(angleA);
	if (b.x < 0)
		completeAngle(angleB);
	float angle = angleB - angleA;
	if ((angle > mypi) | (angle < -mypi)){
		completeAngle(angle);
		angle = -angle;
	}
	/*printVec(b);
	printVec(a);
	printf("%f = %f - %f\n", glm::degrees(angle), glm::degrees(angleB), glm::degrees(angleA));*/
	return angle;
}

void rotCam() {
	float dirAngle = glm::degrees(anglewNonAbs(lastheadDirection, myCam->headDirection));
	float angle = abs(dirAngle);
	mat4 rotator(1);
	//printVec(myCam->headDirection);
	//printVec(lastheadDirection);
	//printf("%f %f\n", glm::degrees(glm::angle(lastheadDirection, myCam->headDirection)), dirAngle);
	if (angle > bigAngleFrame) {
		myCam->myRotate(dirAngle*bigTolerance, zAx, 4);
		rotator = glm::rotate(dirAngle*bigTolerance, zAx);
	}
	else if (angle > angleFrame) {
		myCam->myRotate(dirAngle*tolerance, zAx, 4);
		rotator = glm::rotate(dirAngle*tolerance, zAx);
	}
	lastheadDirection = v4to3(v3to40(lastheadDirection)*rotator);
	
	myCam->myTranslate(-v4to3(myCam->getTraslate()), 0);
	myCam->myTranslate(-v4to3(myCam->GetRot() * v3to4(realcampos)), 0);
}

void animate() {	
	if (snakeviewmode) {
		rotCam();
	}
	glm::vec3 diff = getMid() - myCam->snakeMid;
	if (sizeOfVec(diff) > bigframe)
		diff = diff * bigTolerance;
	else {
		if (sizeOfVec(diff) < frame)
			animateMovement = false;
		diff = diff * tolerance;
	}

	myCam->snakeMid = myCam->snakeMid + diff;
	realcampos = realcampos + diff;
	myCam->myTranslate(-v4to3(myCam->GetRot()*v3to4(diff)), 0);	
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

//TODO optimize
void userInputRotHead() {
	if (!snakeviewmode) return;
	vec3 camLoc = v4to3(myCam->getTraslate());
	//go back to mid
	myCam->myTranslate(-camLoc, 0);
	myCam->doRotate(mat4(1));
	camLoc = orderCamSnakeEyeMode();

	myCam->myTranslate(-camLoc, 0);
}

void switchCamMode() {
	snakeviewmode = !snakeviewmode;
	vec3 camLoc;
	myCam->snakeMid = getMid();

	if (snakeviewmode) {
		camLoc = orderCamSnakeEyeMode();
	}
	else {
		myCam->resetEuler();
		myCam->doRotate(mat4(1));
		camLoc = myCam->snakeMid;
		camLoc.z += heightTop;
	}
	//reset camera to (0,0,0) position, view from z to xy plane
	myCam->myTranslate(-v4to3(myCam->getTraslate()), 0);
	//repositioning
	myCam->myTranslate(-camLoc, 0);
}

//todo validate D
//update on key press when in snake mode and order camera movement