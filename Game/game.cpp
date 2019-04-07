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


bool proj2D = true;
int projMode = 0;
unsigned int xResolution = 50, yResolution = 50;

void Game::changeMode() {
	projMode = (projMode + 1) % 6;
	updateBezier(1, proj2D, projMode);
}

void Game::changeSurfaceLine() {
	proj2D = proj2D^1;
	updateBezier(1, proj2D, projMode);
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

void Game::updateBezier(int BezierShapeId, bool is2D, unsigned int mode)
{
	shapes[BezierShapeId] = new Shape(curve, xResolution, yResolution, is2D, mode);	
}

void Game::Init()
{
	addShape(Axis,-1,LINES);
	//addShape(Octahedron,-1,TRIANGLES);
	//addShapeFromFile("../res/objs/torus.obj",-1,TRIANGLES);
	//addShapeCopy(1,-1,TRIANGLES);
	//modes{POINTS,LINES,LINE_LOOP,LINE_STRIP,TRIANGLES,QUADS};
	if(proj2D)
		addShape(BezierSurface, -1, QUADS);
	else
		addShape(BezierLine, -1, LINES);
	
	//translate all scene away from camera
	myTranslate(glm::vec3(0,0,-20),0);
	int shape_par = 1;
	int init_shapes_num = shapes.size();
	int index = shapes.size();
	for (int j = 0; j < curve->segNo(); j++) {
		for (unsigned int i = 0; i < 4; i++) {
			if ((index == init_shapes_num) || (((index - init_shapes_num) % 4) != 0)) { //check for having only one shape between 2 segments
				addShape(Octahedron, -1, TRIANGLES);
				pickedShape = shapes.size()-1;
				addControlPointShapeId(pickedShape);
				//printf("here pos %d, %d! \n", j,i);
				glm::vec3 tmp = curve->GetControlPointPos(j, i);
				shapeTransformation(xGlobalTranslate, tmp.x);
				shapeTransformation(yGlobalTranslate, tmp.y);
				shapeTransformation(zGlobalTranslate, tmp.z);
				shapeTransformation(yScale, (float)0.3);
				shapeTransformation(xScale, (float)0.3);
				shapeTransformation(zScale, (float)0.3);
			}
			index++;
		};
	}

	pickedShape = 0;

	shapeTransformation(yScale,10);
	shapeTransformation(xScale,10);
	shapeTransformation(zScale,10);

	
	ReadPixel();
	
	pickedShape = -1;
	Activate();
	//printf("here first %d! \n", pickedShape);
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
	int controlpoint = getControlPointByShapeId(pickedShape);
	if ( pickedShape > 1 ) {
		//printf("im in, sending %d and %d to MoveControlPoint \n", (controlpoint / 3), (controlpoint % 3));
		savePastPositions(controlpoint);
		//printf("finished pastt \n");
		//glm::vec4 pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);
			//printf("here %d! \n", pickedShape);
			updateControlShapes(controlpoint, curve);
		//pickedShape = -1;
	}
}

void Game::WhenRotate()
{
	//if(pickedShape>=0)
	//	printMat(GetShapeTransformation());
}

void Game::WhenTranslate()
{

	if(pickedShape>1)
	{
		
		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0,0,0,1);
		int controlpoint = getControlPointByShapeId(pickedShape);
		curve->MoveControlPoint(controlpoint / 3, controlpoint % 3, true, pos);
		updateBezier(1, proj2D, projMode);
		//std::cout<<"( "<<pos.x<<", "<<pos.y<<", "<<pos.z<<")"<<std::endl;
		//if (pickedShape > 1) {
		//	
		//	int controlpoint = getControlPointByShapeId(pickedShape); //find the right control point seg and ind in out array.
		//	printf("%d is the controlpoint \n", controlpoint);
		//	
		//}
	}
}

void Game::savePastPositions(int controlPoint) {
	 int indx = 0;
	for ( int i = -1; i < 2; i=i+2) {
		if (controlPoint+i > -1 && controlPoint + i < (signed)controlPointsShapesIds.size()) {
			//printf("%d is the location \n", controlPoint + i);
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
			//printf("shape: %d \n", pickedShape);
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
	//	int p = pickedShape;
	//	pickedShape = 2;
	//	//shapeTransformation(zLocalRotate,0.45);
	//	pickedShape = p;
	//}
}

Game::~Game(void)
{
	delete curve;
}

//int Game::getControlPointShapeId(int segment, int indx) {
//	return controlPointsShapesIds[3 * segment + indx];
//}

int Game::getControlPointByShapeId(int shapeId) {
	bool found = false;
	int index = 0;
	int ans = -1;
	int arr_size = controlPointsShapesIds.size();
	//printf("%d is the length of controlPointsShapesIds now.. \n", controlPointsShapesIds.size());
	while (!found && (index < arr_size)) {
		//printf("%d is equal to %d ?\n", controlPointsShapesIds[index], shapeId);
		if (controlPointsShapesIds[index] == shapeId) {
			ans = index;
			found = true;
		}
		index++;
	}
	//printf("and is %d  \n", ans);
	return ans;
}

//void Game::setControlPointShapeId(int segment, int indx, int shapeId) {
//	controlPointsShapesIds[3 * segment + indx] = shapeId;
//}

void Game::addControlPointShapeId(int shapeId) {
	controlPointsShapesIds.push_back(shapeId);
	//printf("vector size: %d and got: %d \n", controlPointsShapesIds.size(), shapeId);
}
