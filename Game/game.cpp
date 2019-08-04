#include "game.h"
#include "bezier1D.h"
#include "Bezier2D.h"
#include "MeshConstructor.h"
#include "IntersectTracker.h"
#include "cameraMotion.h"
#include <windows.h>
#include <iostream> 
#include <thread>

IntersectTracker *IT;
Game::Game(vec3 position,float angle,float hwRelation,float near1, float far1): 
	Scene(position,angle,hwRelation,near1,far1){
	IT = new IntersectTracker(this);
	lGen = new leveGenerator(currentLvl);
	sMT = new snakeMoveTracker(snakeLength, speed);
}

Game::~Game(void) {
	delete sMT;
	delete lGen;
	delete IT;
	delete themes;
}

void Game::addShape(IndexedModel model, int parent, unsigned int mode, int tex, int shader)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(model, mode, tex, shader));
}

void Game::addShape(int type,int parent,unsigned int mode)
{
	chainParents.push_back(parent);
	if (type != BezierLine && type != BezierSurface)
		shapes.push_back(new Shape(type, mode));
	else
		printf("Bezier1d/2d direct adding is not supported any more\n");
}

void Game::updateDrawMode(unsigned int mode){
	for(unsigned int i = 1; i < shapes.size(); i++)
		shapes[i]->mode= mode;
}

int snakeLevel;
int snakeNodesShapesStart = -1;
int snakeNodesShapesEnd = -1;
float snakeFullLength = 0;
float jumpy = 0.4f, jumpx = 0.38f;
float lastYext = 0;
static const int bezierRes = 10, cirSubdiv = 4;
void Game::getSegs(float *lastX, float mult, float sign, float jumpX, float jumpY, int segs) {
	std::vector<glm::mat4> segments;
	float lastY = 0;
	mat4 seg0 = mat4(0);
	float segPart = 1 / float(segs);
	for (int i = 0; i < segs; i++) {
		seg0[0] = vec4(*lastX, lastY, 0, 1);
		*lastX = *lastX + jumpX;	lastY = lastY + jumpY*mult;
		seg0[1] = vec4(*lastX, lastY, 0, 1);
		*lastX = *lastX + jumpX;	lastY = lastY + jumpY*mult;
		seg0[2] = vec4(*lastX, lastY, 0, 1);
		*lastX = *lastX + jumpX;	lastY = lastY + jumpY*mult;
		seg0[3] = vec4(*lastX, lastY, 0, 1);
		mult += sign * segPart;
		segments.push_back(seg0);
	}
	Bezier1D body(segments);
	vec3 axisFrom = *(body.GetControlPoint(0, 0).GetPos());
	Bezier2D b(body, cirSubdiv, yAx, vec3(0, axisFrom.y, 0)); 
	addShape(b.GetSurface(bezierRes, bezierRes), -1, TRIANGLES, themes->getTex(4), 1);
	orderSegPart(lastY);
}

void Game::orderSegPart(float segLen) {
	shapeTransformation(yGlobalTranslate, lastYext);
	snakeFullLength += segLen;
	lastYext = segLen;
	pickedShape++;
}

void Game::getTailSegs(float *lastX, float jumpX, float jumpY, int segs) {
	getSegs(lastX, 0, 1, jumpX, jumpY, segs);
}

void Game::getBodySegs(float *lastX, float jumpX, float jumpY, int segs, int amount) {
	getSegs(lastX, 1, 0, jumpX, jumpY, segs);
	Shape *bodySeg = shapes.back();
	for (int i = 0; i < amount - 1; i++) {
		chainParents.push_back(-1);
		shapes.push_back(new Shape(*bodySeg, TRIANGLES));
		orderSegPart(lastYext);
	}
}

void Game::getHeadSegs(float *lastX, float jumpX, float jumpY, int segs) {
	getSegs(lastX, 1 - 1 / float(segs), -1, jumpX, jumpY, segs);
}

vec3 myDir(int direction) {
	if (direction == 0)
		return yAx;
	if (direction == 2)
		return -yAx;
	if (direction == 1)
		return xAx;
	//if (direction == 3)
	return -xAx;
}

/*The vertebral column of a snake consists of anywhere between 200 and 400 (or more) vertebrae.*/
void Game::genSnake(float xLoc, float yLoc, float zLoc, int direction) {
	std::vector<Bezier2D> b1vec;
	float x = 0; float rounding = float(segs) / ends;

	snakeNodesShapesStart = shapes.size();
	pickedShape = snakeNodesShapesStart;//chaining!

	getTailSegs(&x, jumpx * rounding, 2.f*jumpy, ends);
	getBodySegs(&x, 0, jumpy, 4, snakeLength -2);
	//round head generated here
	getHeadSegs(&x, -jumpx * rounding, 2.5f*jumpy, ends);

	snakeNodesShapesEnd = shapes.size() - 1;
	
	//big obj head
	//addShapeFromFile("../res/objs/snake_head.obj", -1, TRIANGLES, themes->getTex(4), 4);
	//orderGenObj(vec3(0, 0, 0), 0.02f * allscale, (direction + 2)%4);

	for (unsigned int i = snakeNodesShapesStart + 1; i < shapes.size(); i++) {
		setParent(i, i-1);
	}
	
	//still have direction but make it adapt just to one position to the right - todo FIX THIS (- maybe double d)
	if (direction == 0)
		shapeTransformation(snakeNodesShapesStart, GlobalTranslate, vec3(0, snakeFullLength, 0));
	else if (direction == 1)
		shapeTransformation(snakeNodesShapesStart, GlobalTranslate, vec3(- snakeFullLength, 0, 0));
	else if (direction == 2)
		shapeTransformation(snakeNodesShapesStart, GlobalTranslate, vec3(0, - snakeFullLength, 0));
	else// if (direction == 3)
		shapeTransformation(snakeNodesShapesStart, GlobalTranslate, vec3(snakeFullLength, 0, 0));
	shapeTransformation(snakeNodesShapesStart, GlobalTranslate, vec3(xLoc + allscale * .5f, yLoc + allscale * .5f, zLoc + 5.f));
	shapes[snakeNodesShapesStart]->myRotate(180.f + 90.f * direction, zAx, zAxis1);
	//printf("%d %f\n", direction, 180.f + 90.f * direction);
}

char	  **filePath;
Shape	  **uploadedFiles;
intersect **computedKDtrees;
inline void Game::updateThemeArrays()
{
	theme *tempTheme = themes->getCurrentTheme();
	filePath	    = tempTheme->filepath;
	uploadedFiles   = (Shape **) tempTheme->uploadedObj;
	computedKDtrees = (intersect **) tempTheme->computedKDtrees;
}

void Game::loadThemes() {
	themes = new ThemeHolder(this, 3, firstTheme);
	updateThemeArrays();
}

void Game::changeTheme(int nextTheme) {
	themes->swapThemes(nextTheme);
	updateThemeArrays();
}

inline void Game::orderGenObj(vec3 startLoc, float scale, int direction) {
	shapes.back()->myTranslate(startLoc, 0);
	if (scale != -1)
		shapeTransformation(shapes.size() - 1, Scale, vec3(scale, scale, scale));
	shapes.back()->myRotate(90.f * direction, zAx, zAxis1);
}

void Game::genObj(int ptrIndx, int tex, vec3 startLoc, float scale, int direction) {
	if (uploadedFiles[ptrIndx] == nullptr) {
		addShapeFromFile(filePath[ptrIndx], -1, TRIANGLES, tex, 4);//using the basic shader
		uploadedFiles[ptrIndx] = shapes.back();
	}
	else {
		chainParents.push_back(-1);
		shapes.push_back(new Shape(*uploadedFiles[ptrIndx], TRIANGLES));
	}
	orderGenObj(startLoc, scale, direction);
}

void Game::onIntersectCave(Shape *s) {
	printf("reach to seafty END GAME\n");
	if (fruitCounter == 0) {
		Deactivate();
		PlayWin();
	}
}

void Game::onIntersectFruit(Shape *s) {
	printf("got fruit\n");
	s->Hide();
	IT->remove(s);
	PlayPoint();
	fruitCounter--;
}

void Game::onIntersectObstecle(Shape *s) {
	printf("bump into obstecle\n");
	PlayExplosion();
	Deactivate();
}

float climbAngle = glm::atan(zscale / allscale);
void Game::onIntersectWalls(Shape *s) {
	printf("wall  \n");
	PlayExplosion();
	Deactivate();
}

void Game::onIntersectFallWall(Shape *s){
	printf("fall wall\n");
}

void Game::onIntersectStairs(Shape *s) {
	printf("stairs\n");
}

enum MapObjTypes { NOTUSED, Snake, Cave, Obstecle, Fruit };
enum IntersectFuncTypes { CaveF, ObstecleF, FruitF, StairF, WallF, FallWallF };
inline void Game::addShapeAndKD(int myIndex, int tex, float x, float y, vec3 pos, int level, float scale, int dir) {
	genObj(myIndex, tex, pos, scale, dir);
	if (computedKDtrees[myIndex])
		IT->addObj(x, y, level, shapes.back(), myIndex, computedKDtrees[myIndex]);
	else
		computedKDtrees[myIndex] = IT->addObj(x, y, level, shapes.back(), myIndex, MeshConstructor::getlastInitMeshPositions());
}

/*
direction map
	0
3		1
	2
*/
void Game::specialObjHandle(objLocation &obj) {
	float x = obj.x, y = obj.y, z = obj.z;
	int dir = obj.direction;
	//printf("%f %f %f %d %d\n", x, y, z, dir, obj.type);
	switch (obj.type) {
	case Snake:
		genSnake(x, y, z, dir);
		snakeLevel = obj.level;
		IT->addSnakeHead(MeshConstructor::getlastInitMeshPositions());
		//printf("added SnakeHead\n");
		break;
	case Cave:
		addShapeAndKD(CaveF, themes->getTex(1), x, y, vec3(x + allscale / 2, y + allscale / 2, z - 22), obj.level, 0.05f * allscale, dir);
		break;
	case Obstecle:
		addShapeAndKD(ObstecleF, themes->getTex(2), x, y, vec3(x + allscale / 2, y + allscale / 2, z), obj.level, 0.01f * allscale, dir);
		break;
	case Fruit:
		fruitCounter++;
		addShapeAndKD(FruitF, themes->getTex(3), x, y, vec3(x + allscale / 2, y + allscale / 2, z), obj.level, 0.003f * allscale, dir);
		fruitsVec.push_back(shapes.back());
		break;
	default:
		printf("unknown special obj <%d>\n", obj.type);
		break;
	}
}

void Game::onIntersectSnakeHead(int type, Shape *myShape) {
	switch (type) {
		case WallF:
			Game::onIntersectWalls(myShape);
			break;
		case FallWallF:
			Game::onIntersectFallWall(myShape);
			break;
		case CaveF:
			Game::onIntersectCave(myShape);
			break;
		case ObstecleF:
			Game::onIntersectObstecle(myShape);
			break;
		case FruitF:
			Game::onIntersectFruit(myShape);
			break;
		case StairF:
			Game::onIntersectStairs(myShape);
			break;
		default:
			printf("unknown special obj <%d>\n", type);
			break;
	}
};

void Game::addCubes() {
	addShape(Cube, -1, TRIANGLES);
	addShape(Cube, -1, TRIANGLES);
	addShape(Cube, -1, TRIANGLES);
	pickedShape = shapes.size() - 1;
	this->shapeTransformation(this->xGlobalTranslate, 10.f);
	pickedShape--;
	this->shapeTransformation(this->yGlobalTranslate, 20.f);
	pickedShape--;
	this->shapeTransformation(this->zGlobalTranslate, 30.f);
}

//PLAYING THEME MUSIC
void Game::configSound() {
	//PlayTheme();
}

void Game::PlayTheme()
{
	PlaySound("../res/sounds/theme.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void Game::PlayPoint()
{
	PlaySound("../res/sounds/eat_apple.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NOWAIT | SND_RING);
}

void Game::PlayWin()
{
	PlaySound("../res/sounds/Cheering.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NOWAIT | SND_RING);
}

void Game::PlayExplosion()
{
	PlaySound("../res/sounds/explosion.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NOWAIT | SND_RING);
}

void Game::updateSnakePosition()
{
	int pShape = snakeNodesShapesStart;
	mat4 root = shapes[pShape++]->makeTrans();
	tailDirection = Bezier1D::v4to3(root * vec4(0, 1, 0, 0));

	for (; pShape < snakeNodesShapesStart + snakeLength/2; pShape++)
		root *= shapes[pShape]->makeTrans();
	midCurLocation = Bezier1D::v4to3(root[3]);
	for (; pShape < snakeNodesShapesStart + snakeLength; pShape++)
		root *= shapes[pShape]->makeTrans();

	headCurLocation = Bezier1D::v4to3(root[3]);
	headDirection = Bezier1D::v4to3(root * vec4(0, 1, 0, 0));
	headTransMAT = root;
}

void Game::orderCamera() {
	updateSnakePosition();
	float zView = -1.5f*snakeFullLength;
	initCameraMotion(this, shapes[snakeNodesShapesStart], abs(zView));
	setCameraTopView();
}

void Game::setupCurrentLevel() {
	printf("parsing files");
	struct objMap map = lGen->getLevel(currentLvl);
	fruitCounter = 0;
	fruitsVec.clear();
	printf("level:%d walls:%d stairs:%d\n", currentLvl, map.walls->size(), map.stairs->size());

	if (map.levelGround != nullptr) {
		for (modelWrapper &obj : *map.levelGround) {
			addShape(obj.model, -1, TRIANGLES, themes->getTex(0), 4);
			shapes.back()->myTranslate(vec3(obj.x, obj.y, obj.z), 0);
		}
		for (modelWrapper &obj : *map.walls) {
			addShape(obj.model, -1, TRIANGLES, themes->getTex(1), 4);
			shapes.back()->myTranslate(vec3(obj.x, obj.y, obj.z), 0);
			IT->addObj(obj.x, obj.y, obj.level, shapes.back(),
				WallF, MeshConstructor::getlastInitMeshPositions());
		}
		for (modelWrapper &obj : *map.stairs) {
			addShape(obj.model, -1, TRIANGLES, themes->getTex(0), 4);
			shapes.back()->myTranslate(vec3(obj.x, obj.y, obj.z), 0);
		}
		for (modelWrapper &obj : *map.stairsWalls) {
			addShape(obj.model, -1, TRIANGLES, themes->getTex(0), 4);
			shapes.back()->myTranslate(vec3(obj.x, obj.y, obj.z), 0);
			shapes.back()->Hide();
			IT->addObj(obj.x, obj.y, obj.level, shapes.back(),
				StairF, MeshConstructor::getlastInitMeshPositions());
		}
		for (modelWrapper &obj : *map.fallWalls) {
			addShape(obj.model, -1, TRIANGLES, themes->getTex(0), 4);
			shapes.back()->myTranslate(vec3(obj.x, obj.y, obj.z), 0);
			shapes.back()->Hide();
			IT->addObj(obj.x, obj.y, obj.level, shapes.back(),
				FallWallF, MeshConstructor::getlastInitMeshPositions());
		}
		for (objLocation &obj : *map.specialObj)
			specialObjHandle(obj);
	}
	else
		printf("level did not been loaded!");
}

void Game::setupEnvironment() {
	setupCurrentLevel();
	orderCamera();
	configSound();
	pickedShape = -1;
}

void Game::Init()
{
	loadThemes();
	setupEnvironment();

	//addCubes();
	ReadPixel();
}

void finUpdate(Shader *s, const int shaderIndx, const int pickedShape) {
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	else
		s->SetUniform4f("lightColor", 0.1f, 0.8f, 0.7f, 1.0f);
	s->Unbind();
}


void Game::UpdateLinear(const glm::mat4 &lastMVP, const glm::mat4 &MVP, const glm::mat4 &nextMVP, const glm::mat4 &Normal, const int shaderIndx) {
	Shader *s = shaders[shaderIndx];
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("lastMVP", lastMVP);
	s->SetUniformMat4f("nextMVP", nextMVP);
	s->SetUniformMat4f("Normal", Normal);
	finUpdate(s, shaderIndx, pickedShape);
}

void Game::UpdateQuaternion(const glm::mat2x4 &lastQuaternion, const glm::mat2x4 &Quaternion, const glm::mat2x4 &nextQuaternion, const glm::mat4 &Normal, const int shaderIndx) {
	//printf("<%f %f %f %f>", Quaternion[0].x, Quaternion[0].y, Quaternion[0].z, Quaternion[0].w);
	//printf(" <%f %f %f %f>\n", Quaternion[1].x, Quaternion[1].y, Quaternion[1].z, Quaternion[1].w);
	Shader *s = shaders[shaderIndx];
	s->Bind();
	s->SetUniformMat2x4f("Quaternion", Quaternion);
	s->SetUniformMat2x4f("lastQuaternion", lastQuaternion);
	s->SetUniformMat2x4f("nextQuaternion", nextQuaternion);
	s->SetUniformMat4f("Normal", Normal);
	finUpdate(s, shaderIndx, pickedShape);
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	finUpdate(s, shaderIndx, pickedShape);
}

void Game::WhenRotate() {}
void Game::WhenTranslate() {}

//speed also depends on user frame rate
int arrowKeyPL = 0;
void Game::setSnakeNodesAngles() 
{
	pickedShape = snakeNodesShapesStart;

	for (int i = 0; i < snakeLength - 1; i++) {//the last node(head) is turned only by the user
		motionTracker mT = sMT->getAngleAndAxis(i);
		float angle = mT.angleTurn;
		if (angle != 0) {
			vec3 axis = mT.rotationAxis;
			shapes[pickedShape]->myRotate(angle, axis, 4);
			pickedShape++;
			shapes[pickedShape]->myRotate(-angle, axis, 4);		
		}
		else
			pickedShape++;
	}
}

void Game::Debug() {
	Deactivate();
	//IT->printDSDebug();
	//sMT->printDS();
}

const int maxFmotion = 10;
float counter = -maxFmotion;
float dir = 1;
float angleFmotion = 10.f;
void Game::fruitMotion() {
	counter += dir;
	if (counter == maxFmotion)
		dir = -1;
	else if (counter == -maxFmotion)
		dir = 1;
	vec3 tempTR = dir*zAx;
	for (int i = 0; i < (signed) fruitsVec.size(); i++) {
		fruitsVec[i]->myTranslate(tempTR, 1);
		fruitsVec[i]->myRotate(angleFmotion, zAx, zAxis1);
	}
}

void Game::Motion()
{
	int savePicked = pickedShape;
	if (isActive)
	{	
		vec3 temp;
		if (rotRecently) {
			rotRecently = false;
			temp = vec3(tailDirection.x * slowSpeed, tailDirection.y * slowSpeed, tailDirection.z * slowSpeed);
		}
		else
			temp = vec3(tailDirection.x * speed, tailDirection.y * speed, tailDirection.z * speed);
		shapeTransformation(snakeNodesShapesStart, GlobalTranslate, temp);
		
		setSnakeNodesAngles();
		updateSnakePosition();

		fruitMotion();

		updateCam();

		IT->isIntersectSnakeHead(headTransMAT, headCurLocation.x, headCurLocation.y, snakeLevel);
	}
	pickedShape = savePicked;
}

float anglePL = 5.f;
void Game::changeCameraMode() {
	Deactivate();
	switchCamMode();
	Activate();
}

mat4 rotPl = glm::rotate(anglePL, zAx);
mat4 rotNPl = glm::rotate(-anglePL, zAx);
void Game::changeDirPInput(bool dir){
	Deactivate();

	rotRecently = true;
	int sign = (dir ? -1 : 1);
	arrowKeyPL += sign;
	sMT->add(zAx, sign*anglePL);
	shapes[snakeNodesShapesEnd]->myRotate(sign*anglePL, zAx, zAxis1);

	Activate();
}

void Game::playerInput(bool dir) {
	changeDirPInput(dir);
}