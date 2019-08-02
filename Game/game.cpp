#include "game.h"
#include "bezier1D.h"
#include "Bezier2D.h"
#include "MeshConstructor.h"
#include "IntersectTracker.h"
#include "cameraMotion.h"
#include <windows.h>
#include <iostream> 
#include <thread>


Game::Game():Scene(){
	sMT = new snakeMoveTracker(snakeLength, speed);
}

Game::Game(vec3 position,float angle,float hwRelation,float near1, float far1): 
	Scene(position,angle,hwRelation,near1,far1){
	sMT = new snakeMoveTracker(snakeLength, speed);
}

Game::~Game(void) {
	delete sMT;
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
	addShape(b.GetSurface(bezierRes, bezierRes), -1, TRIANGLES, 8, 1);
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
	Shape *bodySeg = shapes[shapes.size() - 1];
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
	//big obj head
	//genObj(4, 0, vec3(0,25,0), 0.001f * allscale, direction+1);

	snakeNodesShapesEnd = shapes.size() - 1;

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

static const char *filePath[] 
	{ "../res/objs/cave.obj", nullptr, nullptr, nullptr, "../res/objs/Nokia_3310.obj", "../res/objs/TNT_box.obj",
		"../res/objs/apple.obj", "../res/objs/snake_head.obj" };
Shape **uploadedFiles = (Shape **) calloc(sizeof(Shape *), sizeof(filePath)/sizeof(char*));
intersect **computedKDtrees = (intersect **)calloc(sizeof(intersect *), sizeof(filePath) / sizeof(char*));
void loadTheme() {
	//TODO
	//flush uploadedFiles, computedKDtrees
	//open dir and find names
	//load texture for walls and floor
	//future - combine with MTL
}

void Game::genObj(int ptrIndx, int tex, vec3 startLoc, float scale, int direction) {
	if (uploadedFiles[ptrIndx] == nullptr) {
		addShapeFromFile(filePath[ptrIndx], -1, TRIANGLES, tex, 4);//using the basic shader
		uploadedFiles[ptrIndx] = shapes[shapes.size() - 1];
	}
	else {
		chainParents.push_back(-1);
		shapes.push_back(new Shape(*uploadedFiles[ptrIndx], TRIANGLES));
	}
	shapes[shapes.size() - 1]->myTranslate(startLoc, 0);
	if (scale != -1)
		shapeTransformation(shapes.size() - 1, Scale, vec3(scale, scale, scale));
	shapes[shapes.size() - 1]->myRotate(90.f * direction, zAx, zAxis1);
}

void onIntersectPrint(std::vector<IndexedModel> sol) {
	printf("\r");
	printf("hey man");
}
void printIM(std::vector<IndexedModel> sol) {
	for (int i = 0; i < (signed)sol.size(); i++) {
		printf("box[%d]\n",i);
		for (int j = 0; j < (signed)sol[i].positions.size(); j++)
			printVec(sol[i].positions[j]);
	}
}

void onIntersectCave(std::vector<IndexedModel> sol) {
	printf("reach to seafty END GAME\n");
	printIM(sol);
	printf("||\n\n");
}

float climbAngle = glm::atan(zscale / allscale);
void onIntersectWalls(std::vector<IndexedModel> sol) {
	printf("wall  \n");
	printIM(sol);
	printf("||\n\n");
}

void onIntersectStairs(std::vector<IndexedModel> sol) {
	printf("stairs\n");
	printIM(sol);
	printf("||\n\n");
}

enum MapObjTypes { NOTUSED, Snake, Cave, Obstecle, Fruit };
const MeshConstructor *meshelper = nullptr;
inline void Game::addShapeAndKD(int myIndex, int tex, float x, float y, vec3 pos, int level, float scale, int dir) {
	genObj(myIndex, tex, pos, scale, dir);
	if (computedKDtrees[myIndex])
		addObj(x, y, level, shapes[shapes.size() - 1], onIntersectCave, computedKDtrees[0]);
	else
		computedKDtrees[myIndex] = addObj(x, y, level, shapes[shapes.size() - 1], onIntersectCave, meshelper->getlastInitMeshPositions());
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
		addSnakeHead(meshelper->getlastInitMeshPositions());
		//printf("added SnakeHead\n");
		break;
	case Cave:
		addShapeAndKD(0, 2, x, y, vec3(x + allscale / 2, y + allscale / 2, z - 22), obj.level, 0.05f * allscale, dir);
		break;
	case Obstecle:
		addShapeAndKD(1, 3, x, y, vec3(x + allscale / 2, y + allscale / 2, z), obj.level, 0.003f * allscale, dir);
		break;
	case Fruit:
		addShapeAndKD(2, 3, x, y, vec3(x + allscale / 2, y + allscale / 2, z), obj.level, 0.003f * allscale, dir);
		break;
	default:
		printf("unknown special obj <%d>\n", obj.type);
		break;
	}
}

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
	/*
	std::thread t1(&Game::PlayTheme, this);
	t1.detach();
	PlaySound("../res/sounds/theme.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	*/

	//PlaySound("../res/sounds/eat_apple.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	//PlaySound("../res/sounds/explosion.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	//std::thread t2(&Game::PlayPoint, this);
	//t2.detach();
}

void Game::PlayTheme()
{
	PlaySound("../res/sounds/theme.wav", NULL, SND_FILENAME | SND_LOOP);
}

void Game::PlayPoint()
{
	PlaySound("../res/sounds/eat_apple.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void Game::PlayExplosion()
{
	PlaySound("../res/sounds/explosion.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
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

const int firstLvl = 0;
leveGenerator lGen(firstLvl);
void Game::Init()
{
	meshelper = new MeshConstructor(100);
	//addShape(Axis, -1, LINES);
	
	struct objMap map = lGen.getLevel(firstLvl);//todo - level -1 is random
	printf("level:%d walls:%d stairs:%d\n", firstLvl, map.walls->size(), map.stairs->size());

	if (map.levelGround != nullptr) {
		for (modelWrapper &obj : *map.levelGround) {
			addShape(obj.model, -1, TRIANGLES, 2, 4);
			shapes[shapes.size() - 1]->myTranslate(vec3(obj.x, obj.y, obj.z), 0);
		}
		for (modelWrapper &obj : *map.walls) {
			addShape(obj.model, -1, TRIANGLES, 2, 4);
			shapes[shapes.size() - 1]->myTranslate(vec3(obj.x, obj.y, obj.z), 0);
			addObj(obj.x, obj.y, obj.level, shapes[shapes.size() - 1],
				onIntersectWalls, meshelper->getlastInitMeshPositions());
		}
		for (modelWrapper &obj : *map.stairs) {
			addShape(obj.model, -1, TRIANGLES, 2, 4);
			shapes[shapes.size() - 1]->myTranslate(vec3(obj.x, obj.y, obj.z), 0);
			addObj(obj.x, obj.y, obj.level, shapes[shapes.size() - 1],
				onIntersectStairs, meshelper->getlastInitMeshPositions());
		}
		for (objLocation &obj : *map.specialObj)
			specialObjHandle(obj);
	}
	else
		printf("level did not been loaded!");

	orderCamera();
	addCubes();
	configSound();

	ReadPixel();
	//Activate();
	pickedShape = -1;
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
	printDSDebug();
	//sMT->printDS();
}

void Game::Motion()
{
	int savePicked = pickedShape;
	if (isActive)
	{
		vec3 temp(tailDirection.x * speed, tailDirection.y * speed, tailDirection.z * speed);
		shapeTransformation(snakeNodesShapesStart, GlobalTranslate, temp);
		
		setSnakeNodesAngles();
		updateSnakePosition();

		updateCam();

		isIntersectSnakeHead(headTransMAT, headCurLocation.x, headCurLocation.y, snakeLevel);
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

	int sign = (dir ? -1 : 1);
	arrowKeyPL += sign;
	sMT->add(zAx, sign*anglePL);
	shapes[snakeNodesShapesEnd]->myRotate(sign*anglePL, zAx, zAxis1);

	Activate();
}

void Game::playerInput(bool dir) {
	changeDirPInput(dir);
}