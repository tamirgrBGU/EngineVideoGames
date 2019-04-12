#pragma once
#include "scene.h"
#include "bezier1D.h"

class Game : public Scene
{
	Bezier1D *curve;
	std::vector<int> controlPointsShapesIds;
	vec3 pastLoc[7];
	bool proj2D = false;
	int projMode = -1;
	unsigned int xResolution = 15, yResolution = 15;
	float movement_ = 0;
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
	
	void Game::changeMode() {
		projMode = (projMode + 1) % 6;
		updateBezier(1, proj2D, projMode);
	}

	//CHANGE FROM BEZIER Line TO SURFACE without changing mode
	void Game::changeSurfaceLine() {
		proj2D = proj2D ^ 1;
		updateBezier(1, proj2D, projMode);
	}

	//CHANGE FROM BEZIER Line TO SURFACE and changing plotting mode
	void Game::changeSurfaceLineSpace() {
		if (proj2D) {
			projMode = Scene::LINES;
		}
		else {
			projMode = Scene::QUADS;
		}
		proj2D = proj2D ^ 1;
		updateBezier(1, proj2D, projMode);
	}
	void Game::change_movement(float num) {
		movement_ += num;
	}

	~Game(void);


	//void setControlPointShapeId(int segment, int indx, int shapeId);
	int getControlPointByShapeId(int shapeId);
	void addControlPointShapeId(int shapeId);
	/*int getControlPointShapeId(int segment, int indx);*/

};

