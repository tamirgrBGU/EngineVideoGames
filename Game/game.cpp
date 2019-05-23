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

static std::vector<glm::mat4> getBodySegs(float *lastX, float *lastY, float jumpX, float jumpY, float segs) {
	std::vector<glm::mat4> segments;
	mat4 seg0 = mat4(0);

	seg0[0] = vec4(0, *lastY, 0, 1);
	seg0[1] = vec4(*lastX, *lastY, 0, 1);
	*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
	seg0[2] = vec4(*lastX, *lastY, 0, 1);
	*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
	seg0[3] = vec4(*lastX, *lastY, 0, 1);

	segments.push_back(seg0);
	for (int i = 0; i < segs - 1; i++) {
		seg0[0] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
		seg0[1] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
		seg0[2] = vec4(*lastX, *lastY, 0, 1);
		*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
		seg0[3] = vec4(*lastX, *lastY, 0, 1);
		segments.push_back(seg0);
	}
	seg0[0] = vec4(*lastX, *lastY, 0, 1);
	*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
	seg0[1] = vec4(*lastX, *lastY, 0, 1);
	*lastX = *lastX + jumpX;	*lastY = *lastY + jumpY;
	seg0[2] = vec4(*lastX, *lastY, 0, 1);
	seg0[3] = vec4(0, *lastY, 0, 1);
	segments.push_back(seg0);
	return segments;
}

intersect *a = nullptr;
intersect *b = nullptr;
std::vector<Bezier1D> b1vec;
float jumpy = 0.016, jumpx = 0.005;
int snakeLength = 4;
void Game::Init()
{
	addShape(Axis, -1, LINES);
	MeshConstructor meshelper(100);

	std::cout << "start snake" << std::endl;
	float x = 0; float y = 0;
	Bezier1D head(getBodySegs(&x, &y, jumpx, jumpy, 4));
	b1vec.push_back(head);
	for (int i = snakeLength - 2; i > 0; i--) {
		y += jumpy;
		Bezier1D body(getBodySegs(&x, &y, 0, jumpy, 4));
		b1vec.push_back(body);
	}
	y += jumpy;
	Bezier1D tail(getBodySegs(&x, &y, -jumpx, jumpy, 4));
	b1vec.push_back(tail);

	Bezier2D b(b1vec[0], 5);
	addShape(b.GetSurface(12, 12), -1, LINES);//QUADS
	int lastPickedShape = 1;
	for (int i = 1; i < snakeLength; i++) {
		std::cout << i << std::endl;
		Bezier2D b(b1vec[i], 5);
		addShape(b.GetSurface(12, 12), lastPickedShape++, LINES);
		b.~Bezier2D();
		//TODO TRANSLATE and parent
	}
	std::cout << "done snake" << std::endl;

	//pickedShape = 1;
	//shapeTransformation(xScale,10);
	//shapeTransformation(xGlobalTranslate, -10);

	ReadPixel();
	pickedShape = -1;
}

	void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.1f,0.8f,0.7f,1.0f);
	s->Unbind();
}

void Game::WhenRotate() {}

void Game::WhenTranslate()
{	}

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
