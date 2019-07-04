#include "game.h"
#include "bezier1D.h"
#include "Bezier2D.h"
#include "MeshConstructor.h"
#include "IntersectTracker.h"
#include <windows.h>
#include <iostream>  
#include <thread>

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}


Game::Game():Scene(){}

Game::Game(glm::vec3 position,float angle,float hwRelation,float near1, float far1): 
	Scene(position,angle,hwRelation,near1,far1){}

Game::~Game(void) {}

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

glm::vec3 xAx(1, 0, 0);
glm::vec3 yAx(0, 1, 0);
glm::vec3 zAx(0, 0, 1);
vec3 snakeDirection; //TODO - maintain on key pressed
vec3 snakeCurLocation; //TODO
int snakeNodesShapesStart = -1;
int snakeNodesShapesEnd = -1;
float snakeFullLength = 0;
float jumpy = 0.8f, jumpx = 0.32f;
float lastYext = 0;
static const int bezierRes = 10, cirSubdiv = 4;
void Game::getSegs(float *lastX, float mult, float sign, float jumpX, float jumpY, int segs) {
	std::vector<glm::mat4> segments;
	float lastY = 0;
	mat4 seg0 = mat4(0);
	for (int i = 0; i < segs; i++) {
		seg0[0] = vec4(*lastX, lastY, 0, 1);
		*lastX = *lastX + jumpX;	lastY = lastY + jumpY*mult;
		seg0[1] = vec4(*lastX, lastY, 0, 1);
		*lastX = *lastX + jumpX;	lastY = lastY + jumpY*mult;
		seg0[2] = vec4(*lastX, lastY, 0, 1);
		*lastX = *lastX + jumpX;	lastY = lastY + jumpY*mult;
		seg0[3] = vec4(*lastX, lastY, 0, 1);
		mult += sign*1 / float(segs);
		segments.push_back(seg0);
	}
	Bezier1D body(segments);
	vec3 axisFrom = *(body.GetControlPoint(0, 0).GetPos());
	Bezier2D b(body, cirSubdiv, yAx, vec3(0, axisFrom.y, 0)); 
	addShape(b.GetSurface(bezierRes, bezierRes), -1, TRIANGLES, 3, 1);
	shapeTransformation(yGlobalTranslate, lastYext);
	snakeFullLength += lastY;
	lastYext = lastY;
	pickedShape++;
}

void Game::getTailSegs(float *lastX, float jumpX, float jumpY, int segs) {
	getSegs(lastX, 0, 1, jumpX, jumpY, segs);
}

void Game::getBodySegs(float *lastX, float jumpX, float jumpY, int segs) {
	getSegs(lastX, 1, 0, jumpX, jumpY, segs);
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
static const int snakeLength = 15, segs = 5, ends = 10;
std::vector<float> nodesAngles;
void Game::genSnake(float xLoc, float yLoc, float zLoc, int direction) {
	std::vector<Bezier2D> b1vec;
	float x = 0; float rounding = float(segs) / ends;

	snakeNodesShapesStart = shapes.size();
	pickedShape = snakeNodesShapesStart;//chaining!

	getTailSegs(&x, jumpx * rounding, jumpy, ends);
	for (int i = snakeLength - 2; i > 0; i--)
		getBodySegs(&x, 0, jumpy, 4);		
	getHeadSegs(&x, -jumpx * rounding, jumpy, ends);

	snakeNodesShapesEnd = shapes.size() - 1;

	for (unsigned int i = snakeNodesShapesStart + 1; i < shapes.size(); i++) {
		setParent(i, i-1);
	}

	for (unsigned int i = 0; i < snakeLength; i++) {
		nodesAngles.push_back(0.f);
	}
		
	pickedShape = snakeNodesShapesStart;
	shapeTransformation(xLocalTranslate, xLoc);
	shapeTransformation(yLocalTranslate, yLoc - snakeFullLength);
	shapeTransformation(zLocalTranslate, zLoc);
	shapeTransformation(zLocalRotate, 180.f);
	shapeTransformation(zLocalRotate, 90.f * direction);
	snakeDirection = myDir(direction);
	snakeCurLocation = vec3(xLoc, yLoc, zLoc);
}

const char *caveStr = "../res/objs/cave.obj";
const char *nokiaStr = "../res/objs/Nokia_3310.obj";
const char *tntStr = "../res/objs/TNT_box.obj";
const char *appleStr = "../res/objs/apple.obj";
const char *snake_headStr = "../res/objs/snake_head.obj";
void Game::genObj(const char * ptr, int tex, vec3 startLoc, float scale, int direction) {
	addShapeFromFile(ptr, -1, TRIANGLES, tex, 3);
	shapes[shapes.size() - 1]->myTranslate(startLoc, 1);
	if (scale != -1)
		shapes[shapes.size() - 1]->myScale(vec3(scale, scale, scale));
	shapeTransformation(zLocalRotate, 90.f * direction);
}

void onIntersectPrint(void) {
	printf("hey man\n");
}

void onIntersectWalls(void) {
	printf("wall\n");
}

void onIntersectStairs(void) {
	printf("stairs\n");
}


const MeshConstructor *meshelper = nullptr;
void Game::specialObjHandle(objLocation &obj) {
	float x = obj.x, y = obj.y, z = obj.z;
	int dir = obj.direction;
	//printf("%f %f %f %d %d\n", x, y, z, dir, obj.type);
	switch (obj.type) {
	case 1:
		genSnake(x, y, z, dir);
		addSnakeHead(meshelper->getlastInitMeshPositions());
		break;
	case 2:
		genObj(caveStr, 2, vec3(x, y, z), 0.05f * allscale, dir);
		addObj(x, y, obj.level, onIntersectPrint, meshelper->getlastInitMeshPositions());
		break;
	case 3:
		genObj(appleStr, 3, vec3(x, y, z), 0.003f * allscale, dir);
		addObj(x, y, obj.level, onIntersectPrint, meshelper->getlastInitMeshPositions());
		break;
	default:
		printf("unknown special obj <%d>\n", obj.type);
		break;
	}
}


leveGenerator lGen(0);
void Game::Init()
{
	meshelper = new MeshConstructor(100);
	//addShape(Axis, -1, LINES);
	
	struct objMap map = lGen.getLevel(0);//todo - level -1 is random
	if (map.levelGround != nullptr) {
		for (modelWrapper &obj : *map.levelGround)
			addShape(obj.model, -1, TRIANGLES, 2, 3);
		for (modelWrapper &obj : *map.walls) {
			addShape(obj.model, -1, TRIANGLES, 2, 3);
			addObj(obj.x, obj.y, obj.level, onIntersectWalls, meshelper->getlastInitMeshPositions());
		}
		for (modelWrapper &obj : *map.stairs) {
			addShape(obj.model, -1, TRIANGLES, 2, 3);
			addObj(obj.x, obj.y, obj.level, onIntersectStairs, meshelper->getlastInitMeshPositions());
		}
		for (objLocation &obj : *map.specialObj)
			specialObjHandle(obj);
	}
	else
		printf("level did not been loaded!");	

	//translate all scene away from camera
	//myTranslate(glm::vec3(0, 0, -20), 0);
		
	vec3 midSnake = snakeDirection;
	int halfS = snakeFullLength / 2;
	midSnake = vec3(midSnake.x*halfS, midSnake.y*halfS, -snakeFullLength);// midSnake.z*halfS);
	midSnake = snakeCurLocation - midSnake;
	myTranslate(-midSnake, 0);
	
	addShape(Cube, -1, TRIANGLES);
	addShape(Cube, -1, TRIANGLES);
	addShape(Cube, -1, TRIANGLES);
	pickedShape = shapes.size() - 1;
	this->shapeTransformation(this->xGlobalTranslate, 10.f);
	pickedShape--;
	this->shapeTransformation(this->yGlobalTranslate, 20.f);
	pickedShape--;
	this->shapeTransformation(this->zGlobalTranslate, 30.f);

	//PLAYING THEME MUSIC
	//std::thread t1(&Game::PlayTheme, this);
	//t1.detach();
	//new thread PlaySound("../res/sounds/theme.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	//PlaySound("../res/sounds/eat_apple.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	//PlaySound("../res/sounds/explosion.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	//std::thread t2(&Game::PlayPoint, this);
	//t2.detach();
	ReadPixel();
	//Activate();
	pickedShape = -1;
}

void Game::PlayTheme()
{
	//PlaySound("../res/sounds/theme.wav", NULL,  SND_FILENAME | SND_LOOP);
}

void Game::PlayPoint()
{
	//PlaySound("../res/sounds/eat_apple.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void Game::PlayExplosion()
{
	//PlaySound("../res/sounds/explosion.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void finUpdate(Shader *s, const int  shaderIndx, const int pickedShape) {
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


void Game::UpdateLinear(const glm::mat4 &lastMVP, const glm::mat4 &MVP, const glm::mat4 &nextMVP, const glm::mat4 &Normal, const int  shaderIndx) {
	Shader *s = shaders[shaderIndx];
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("lastMVP", lastMVP);
	s->SetUniformMat4f("nextMVP", nextMVP);
	s->SetUniformMat4f("Normal", Normal);
	finUpdate(s, shaderIndx, pickedShape);
}

void Game::UpdateQuaternion(const glm::mat2x4 &lastQuaternion, const glm::mat2x4 &Quaternion, const glm::mat2x4 &nextQuaternion, const glm::mat4 &Normal, const int  shaderIndx) {
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

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	finUpdate(s, shaderIndx, pickedShape);
}

void Game::WhenRotate() {}
void Game::WhenTranslate() {}

void Game::setSnakeNodesAngles() {
	for (int i = 0; i < snakeLength - 1; i++) {
		float diff = (nodesAngles[i + 1] - nodesAngles[i])/10;
		nodesAngles[i] += diff;
		shapes[snakeNodesShapesStart + i]->myRotate(diff, vec3(0, 0, 1), zAxis1);
		//nodesAngles[i + 1] -= diff;
		shapes[snakeNodesShapesStart + i + 1]->myRotate(-diff, vec3(0, 0, 1), zAxis1);
	}	
}

bool snakeviewmode = false;
void Game::Motion()
{
	if (isActive)
	{
		vec3 t(snakeDirection.x / 10, snakeDirection.y / 10, snakeDirection.z / 10);
		shapes[snakeNodesShapesStart]->myTranslate(t, 0);

		if (snakeviewmode)
			myTranslate(vec3(0, 0, 1), 0);
		else
			myTranslate(-t, 0);

		snakeCurLocation += t;
		setSnakeNodesAngles();
		//isIntersectSnakeHead(shapes[snakeNodesShapesEnd]->makeTrans(), snakeCurLocation.x, snakeCurLocation.y);
	}
}

Bezier1D b1d; Bezier2D b2d;
float anglePL = 5.f;
int arrowKeyPL = 0;
void Game::changeCameraMode() {
	snakeviewmode = !snakeviewmode;
	//float angle = b2d.angle_mine_deg(-yAx, snakeDirection);
	//printf("%f\n",angle);
	int sign = snakeviewmode ? 1 : -1;
	Deactivate();
	myRotate(sign * 90.f, glm::cross(zAx,snakeDirection), 8);
	//TODO
	//myRotate(sign * -50.f, xAx, 1);
	//myRotate(sign * angle, zAx, 1);
	myTranslate(glm::vec3(sign * -snakeFullLength*1.5, 0, sign * 1.5 * snakeFullLength), 0);
	/*if(snakeviewmode)
		myRotate(sign*-arrowKeyPL*anglePL, zAx, 0);
	else*/
		myRotate(sign*arrowKeyPL*anglePL, zAx, 0);

	Activate();
}

mat4 rotPl = glm::rotate(anglePL, zAx);
mat4 rotNPl = glm::rotate(-anglePL, zAx);
void Game::playerInput(bool dir) {
	int sign = (dir ? -1 : 1);

	Deactivate();
	if(dir)
		snakeDirection = glm::normalize(b1d.v4to3(b1d.v3to4(snakeDirection)*rotPl));
	else
		snakeDirection = glm::normalize(b1d.v4to3(b1d.v3to4(snakeDirection)*rotNPl));
	
	arrowKeyPL += sign;
	if (snakeviewmode)
		myRotate(sign*-anglePL, zAx, 0);	

	nodesAngles[snakeNodesShapesEnd-snakeNodesShapesStart] += sign*anglePL;//head angle
	shapes[snakeNodesShapesEnd]->myRotate(sign*anglePL, vec3(0, 0, 1), zAxis1);

	Activate();
}