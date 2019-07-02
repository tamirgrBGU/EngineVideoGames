#include "game.h"
#include "../KDtree/intersect.cpp"
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


Game::Game():Scene(){curve = 0;}

Game::Game(glm::vec3 position,float angle,float hwRelation,float near1, float far1) : Scene(position,angle,hwRelation,near1,far1)
{ 
	curve = new Bezier1D();	
}

Bezier1D *Game::getBezier1D() {
	return curve;
};

void Game::addShape(IndexedModel model, int parent, unsigned int mode, int tex, int shader)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(model, mode, tex, shader));
}

void Game::addShape(int type,int parent,unsigned int mode)
{
	chainParents.push_back(parent);
	if(type!=BezierLine && type!=BezierSurface)
		shapes.push_back(new Shape(type,mode));
	else
	{
		projMode = mode;
		if(type == BezierLine)
			shapes.push_back(new Shape(curve, xResolution, yResolution, false, mode));
		else
			shapes.push_back(new Shape(curve, xResolution, yResolution, true, mode));
	}
}

void Game::updateIntersectors(unsigned int mode){
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
int  snakeFullLength = -1;
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
void Game::genSnake(float xLoc, float yLoc, float zLoc, int direction) {
	std::vector<Bezier2D> b1vec;
	float x = 0; float y = 0; float rounding = float(segs) / ends;

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

	/*addShapeFromFile("../res/objs/snake_head.obj", -1, TRIANGLES, 2, 4);// SNAKE HEAD
	//TODO extract head to know if touched
	pickedShape = snakeNodesShapesStartTemp + 1;
	shapeTransformation(yLocalTranslate, -snakeFullLength-50);
	shapeTransformation(xScale, 0.20f);
	shapeTransformation(yScale, 0.25f);
	shapeTransformation(zScale, 0.29f);
	shapeTransformation(xLocalRotate, 180);
	shapeTransformation(yLocalRotate, 180);
	setParent(snakeNodesShapesStartTemp, snakeNodesShapesStartTemp++);	
	y += 20;//assumption of shape size
	*/
	
	snakeFullLength = y;
	pickedShape = snakeNodesShapesStart;
	shapeTransformation(xLocalTranslate, xLoc);
	shapeTransformation(yLocalTranslate, yLoc);
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
void Game::genObj(const char * ptr, int tex, float x, float y, float z, int direction){
	addShapeFromFile(ptr, -1, TRIANGLES, tex, 3);
	pickedShape = shapes.size()-1;
	shapeTransformation(xLocalTranslate, x);
	shapeTransformation(yLocalTranslate, y);
	shapeTransformation(zLocalTranslate, z);
	shapeTransformation(zLocalRotate, 90.f * direction);
}

void Game::specialObjHandle(objLocation &obj) {
	float x=obj.x, y=obj.y, z=float(obj.level);
	int dir = obj.direction;
	printf("%f %f %f %d %d\n", x, y, z, dir, obj.type);
	switch (obj.type) {
		case 1:
			genSnake(x, y, z, dir);
			break;
		case 2:
			genObj(caveStr, 2, x, y, z, dir);
			break;
		case 3:
			genObj(appleStr, 3, x, y, z, dir);
			break;
		default:
			printf("unknown special obj <%d>\n", obj.type);
			break;
	}
}

leveGenerator lGen(0);
intersect *a = nullptr;
intersect *b = nullptr;
void Game::Init()
{
	//addShape(Axis, -1, LINES);
	
	//todo map need to know the level of everything so it can prevent snake from falling and
	// know the direction of stairs to make it climb right..
	
	struct objMap map = lGen.getLevel(0);//todo - level -1 is random
	if (map.levelGround != nullptr) {
		for (IndexedModel &obj : *map.levelGround)
			addShape(obj, -1, TRIANGLES, 2, 3);
		for (IndexedModel &obj : *map.walls) {
			addShape(obj, -1, TRIANGLES, 2, 3);
			//todo on snake colide!
		}
		for (IndexedModel &obj : *map.stairs) {
			addShape(obj, -1, TRIANGLES, 2, 3);
			//todo on snake colide
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
	midSnake = vec3(midSnake.x*halfS, midSnake.y*halfS, 0);// midSnake.z*halfS);
	midSnake = snakeCurLocation - midSnake;
	myTranslate(-midSnake, 0);
	//this->shapeTransformation(this->yGlobalTranslate, midSnake.y);
	//this->shapeTransformation(this->zGlobalTranslate, midSnake.z);
	myTranslate(glm::vec3(0, 0, -snakeFullLength), 0);
	
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
	Activate();
	pickedShape = -1;
}

void Game::PlayTheme()
{
	PlaySound("../res/sounds/theme.wav", NULL,  SND_FILENAME | SND_LOOP);
}

void Game::PlayPoint()
{
	PlaySound("../res/sounds/eat_apple.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void Game::PlayExplosion()
{
	PlaySound("../res/sounds/explosion.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
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

void Game::savePastPositions(int controlPoint) {
	int indx = 0;
	for ( int i = -1; i < 2; i=i+2) {
		if (controlPoint+i > -1 && controlPoint + i < (signed)controlPointsShapesIds.size()) {
			if (controlPoint + i == controlPointsShapesIds.size()-1) {
				pastLoc[indx] = curve->GetControlPointPos((controlPoint + i) / 3 - 1, 3);
			}
			else
				pastLoc[indx] = curve->GetControlPointPos((controlPoint + i) / 3, (controlPoint + i) % 3);
			indx++;
		}
	}
}

void Game::updateControlShapes(int controlPoint, Bezier1D *bez) {
	int index = 0;
	int pickedShape_tmp = pickedShape;
	vec3 move_delta;
	for (int i = -1; i < 2; i=i+2) {
		if (controlPoint + i > -1 && controlPoint + i < (signed) controlPointsShapesIds.size()) {
			if (controlPoint + i == controlPointsShapesIds.size() - 1) {
				move_delta = bez->GetControlPointPos((controlPoint + i) / 3 -1, 3) -= pastLoc[index];
			}
			else
				move_delta = bez->GetControlPointPos((controlPoint + i) / 3, (controlPoint + i) % 3) -= pastLoc[index];
			pickedShape = controlPointsShapesIds[controlPoint + i];
			shapeTransformation(xGlobalTranslate, move_delta.x);
			shapeTransformation(yGlobalTranslate, move_delta.y);
			shapeTransformation(zGlobalTranslate, move_delta.z);
			index++;
			pickedShape = pickedShape_tmp;
		}
	}
	pickedShape = pickedShape_tmp;
	
}

void Game::Motion()
{
	if(isActive)
	{
		int a = pickedShape;
		pickedShape = snakeNodesShapesStart;
		shapeTransformation(xGlobalTranslate, snakeDirection.x);
		shapeTransformation(yGlobalTranslate, snakeDirection.y);
		shapeTransformation(zGlobalTranslate, snakeDirection.z);


		myTranslate(-snakeDirection, 0);
		/*shapeTransformation(xCameraTranslate, snakeDirection.x);
		shapeTransformation(yCameraTranslate, snakeDirection.y);
		shapeTransformation(zCameraTranslate, snakeDirection.z);*/
		pickedShape = a;
	}
}

bool snakeviewmode = false;
Bezier1D b1d; Bezier2D b2d;
void Game::changeCameraMode() {
	snakeviewmode = !snakeviewmode;
	float angle = b2d.angle_mine_deg(-yAx, snakeDirection);
	printf("%f\n",angle);
	int sign = snakeviewmode ? 1 : -1;
	Deactivate();
	myRotate(sign * 80.f, glm::cross(zAx,snakeDirection), 8);
	//TODO
	//myRotate(sign * -50.f, xAx, 1);
	//myRotate(sign * angle, zAx, 1);
	myTranslate(glm::vec3(sign * -snakeFullLength*1.5, 0, sign * 0.8 * snakeFullLength), 0);
	Activate();
}

float anglePL = 5.f;
mat4 rotPl = glm::rotate(anglePL, zAx);
mat4 rotNPl = glm::rotate(-anglePL, zAx);
void Game::playerInput(bool dir) {
	int sign = (dir ? -1 : 1);
	if(dir)
		snakeDirection = glm::normalize(b1d.v4to3(b1d.v3to4(snakeDirection)*rotPl));
	else
		snakeDirection = glm::normalize(b1d.v4to3(b1d.v3to4(snakeDirection)*rotNPl));

	int a = pickedShape;
	pickedShape = snakeNodesShapesEnd;
	shapeTransformation(zLocalRotate, sign*anglePL);
	pickedShape = a;
}

Game::~Game(void)
{
	delete curve;
}