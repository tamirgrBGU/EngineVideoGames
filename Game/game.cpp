#include "game.h"
#include "../KDtree/intersect.cpp"
#include "../EngineVideoGames/MeshConstructor.h"
#include "../EngineVideoGames/Bezier1D.h"
#include "../EngineVideoGames/Bezier2D.h"

#include <iostream>

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

Game::Game(glm::vec3 position,float angle,float hwRelation,float near, float far) : Scene(position,angle,hwRelation,near,far)
{ 
	curve = new Bezier1D();	
}

Bezier1D *Game::getBezier1D() {
	return curve;
};

void Game::addShape(IndexedModel model, int parent, unsigned int mode)
{
	chainParents.push_back(parent);
	shapes.push_back(new Shape(model, mode));	
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

static std::vector<glm::mat4> getHeadSegs(float *lastX, float *lastY, float jumpX, float jumpY, int segs) {
	std::vector<glm::mat4> segments;
	mat4 seg0 = mat4(0);
	float mult = 1 - 1 / float(segs);
	for (int i = 0; i < segs; i++) {
		seg0[0] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY*mult;
		seg0[1] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY*mult;
		seg0[2] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY*mult;
		seg0[3] = vec4(*lastX, *lastY, 0, 1);
		mult -= 1 / float(segs);
		segments.push_back(seg0);
	}
	return segments;
}

static std::vector<glm::mat4> getTailSegs(float *lastX, float *lastY, float jumpX, float jumpY, int segs) {
	std::vector<glm::mat4> segments;
	mat4 seg0 = mat4(0);
	float mult = 0;
	for (int i = 0; i < segs; i++) {
		seg0[0] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY*mult;
		seg0[1] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY*mult;
		seg0[2] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY*mult;
		seg0[3] = vec4(*lastX, *lastY, 0, 1);
		mult += 1 / float(segs);
		segments.push_back(seg0);
	}
	return segments;
}

static std::vector<glm::mat4> getBodySegs(float *lastX, float *lastY, float jumpX, float jumpY, int segs) {
	std::vector<glm::mat4> segments;
	mat4 seg0 = mat4(0);
	for (int i = 0; i < segs; i++) {
		seg0[0] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
		seg0[1] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
		seg0[2] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
		seg0[3] = vec4(*lastX, *lastY, 0, 1);
		segments.push_back(seg0);
	}
	return segments;
}

intersect *a = nullptr;
intersect *b = nullptr;
std::vector<Bezier1D> b1vec;
glm::vec3 yAx(0, 1, 0);
float jumpy = 0.8f, jumpx = 0.32f;
int snakeLength = 5, bezierRes = 20, cirSubdiv = 4, segs = 5, ends = 10;
void Game::Init()
{
	addShape(Axis, -1, LINES);

	std::cout << "start snake" << std::endl;
	float x = 0; float y = 0; float rounding = float(segs) / ends;
	Bezier1D tail(getTailSegs(&x, &y, jumpx * rounding, jumpy, ends));
	b1vec.push_back(tail);
	for (int i = snakeLength - 2; i > 0; i--) {
		//y += jumpy;
		Bezier1D body(getBodySegs(&x, &y, 0, jumpy, 4));
		b1vec.push_back(body);
	}
	//y += jumpy/4;
	Bezier1D head(getHeadSegs(&x, &y, -jumpx * rounding, jumpy, ends));
	b1vec.push_back(head);

	vec3 axisFrom = *(b1vec[0].GetControlPoint(0, 0).GetPos());
	Bezier2D b(b1vec[0], cirSubdiv, yAx, vec3(0, axisFrom.y, 0));
	addShape(b.GetSurface(bezierRes, bezierRes), -1, TRIANGLES);
	int lastPickedShape = 1;
	for (int i = 1; i < snakeLength; i++) {
		axisFrom = *(b1vec[i].GetControlPoint(0, 0).GetPos());
		Bezier2D b(b1vec[i], cirSubdiv, yAx, vec3(0, axisFrom.y, 0));
		addShape(b.GetSurface(bezierRes, bezierRes), lastPickedShape++, TRIANGLES);
		b.~Bezier2D();
	}
	std::cout << "done snake" << std::endl;

	// translate all scene away from camera
	myTranslate(glm::vec3(0, 0, -20), 0);

	pickedShape = 0;

	shapeTransformation(yScale, 10);
	shapeTransformation(xScale, 10);
	shapeTransformation(zScale, 10);


	ReadPixel();
	pickedShape = -1;
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
		shapeTransformation(zLocalRotate,0.75);
	}
}

Game::~Game(void)
{
	delete curve;
}

int Game::getControlPointByShapeId(int shapeId) {
	bool found = false;
	int index = 0;
	int ans = -1;
	int arr_size = controlPointsShapesIds.size();
	while (!found && (index < arr_size)) {
		if (controlPointsShapesIds[index] == shapeId) {
			ans = index;
			found = true;
		}
		index++;
	}
	return ans;
}


void Game::addControlPointShapeId(int shapeId) {
	controlPointsShapesIds.push_back(shapeId);
}
