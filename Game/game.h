#pragma once
#include "scene.h"
#include "bezier1D.h"

class Game : public Scene
{
	Bezier1D *curve;
	std::vector<int> controlPointsShapesIds;
	vec3 pastLoc[7];
public:
	Game(void);
	Game(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void Init();
	void addShape(int type,int parent,unsigned int mode);
	void updateBezier(int BezierShapeId, bool is2D, unsigned int mode);
//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void Update( glm::mat4 MVP ,glm::mat4 Normals,Shader* s);
	void WhenRotate();
	void WhenTranslate();
	Bezier1D *getBezier1D();
	void savePastPositions(int controlPoint);
	void updateControlShapes(int controlPoint, Bezier1D *bez);
	void Motion();
	void changeMode();//CHANGE DRAWING MODE
	void Game::changeSurfaceLine();//CHANGE FROM BEZIER TO SURFACE
	~Game(void);


	//void setControlPointShapeId(int segment, int indx, int shapeId);
	int getControlPointByShapeId(int shapeId);
	void addControlPointShapeId(int shapeId);
	/*int getControlPointShapeId(int segment, int indx);*/

};

