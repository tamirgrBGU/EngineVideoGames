#include "game.h"

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

void Game::updateBezier(int BezierShapeId, bool is2D, unsigned int mode)
{
	shapes[BezierShapeId] = new Shape(curve, xResolution, yResolution, is2D, mode);	
}

void Game::Init()
{
	//addShape(Axis,-1,LINES);
	//if(proj2D)
	//	addShape(BezierSurface, -1, QUADS);
	//else
	//	addShape(BezierLine, -1, LINES);
	//
	////translate all scene away from camera
	//myTranslate(glm::vec3(0,0,-20),0);
	//int shape_par = 1;
	//int init_shapes_num = shapes.size();
	//int index = shapes.size();
	//for (int j = 0; j < curve->segNo(); j++) {
	//	for (unsigned int i = 0; i < 4; i++) {
	//		if ((index == init_shapes_num) || (((index - init_shapes_num) % 4) != 0)) { //check for having only one shape between 2 segments
	//			addShape(Octahedron, -1, TRIANGLES);
	//			pickedShape = shapes.size()-1;
	//			addControlPointShapeId(pickedShape);
	//			glm::vec3 tmp = curve->GetControlPointPos(j, i);
	//			shapeTransformation(xGlobalTranslate, tmp.x);
	//			shapeTransformation(yGlobalTranslate, tmp.y);
	//			shapeTransformation(zGlobalTranslate, tmp.z);
	//			shapeTransformation(yScale, (float)0.3);
	//			shapeTransformation(xScale, (float)0.3);
	//			shapeTransformation(zScale, (float)0.3);
	//		}
	//		index++;
	//	};
	//}

	//pickedShape = 0;

	//shapeTransformation(yScale,10);
	//shapeTransformation(xScale,10);
	//shapeTransformation(zScale,10);

	addShape(Axis, -1, LINES);
	//addShapeFromFile("../res/objs/torus.obj", -1, TRIANGLES);
	addShape(Octahedron, -1, TRIANGLES);
	addShape(Octahedron, -1, TRIANGLES);
	myTranslate(glm::vec3(0, 0, -40), 0);
	pickedShape = 0;

	shapeTransformation(yScale,10);
	shapeTransformation(xScale,10);
	shapeTransformation(zScale,10);

	pickedShape = 1;

	shapeTransformation(xGlobalTranslate,-20);
	//shapeTransformation(yGlobalTranslate, tmp.y);
	//shapeTransformation(zGlobalTranslate, tmp.z);


	ReadPixel();
	pickedShape = -1;
}

void Game::Update(glm::mat4 MVP,glm::mat4 Normal,Shader *s)
{
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 1.0f);
	s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	
	/*if (pickedShape > 1) {
		int controlpoint = getControlPointByShapeId(pickedShape);
		savePastPositions(controlpoint);
		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
		curve->MoveControlPoint(controlpoint / 3, controlpoint % 3, true, pos);
		updateControlShapes(controlpoint, curve);
	}*/
}

void Game::WhenRotate(){}

void Game::WhenTranslate()
{
	
	/*if(pickedShape>1)
	{
		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
		int controlpoint = getControlPointByShapeId(pickedShape);
		curve->MoveControlPoint(controlpoint / 3, controlpoint % 3, true, pos);
		updateBezier(1, proj2D, projMode);
	}*/
}

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
	//if(isActive)
	//{
		int p = pickedShape;
		pickedShape = 1;
		shapeTransformation(xGlobalTranslate,movement_);
		pickedShape = p;
	//}
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
