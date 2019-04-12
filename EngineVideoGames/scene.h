#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include "VertexArray.hpp"
#include <vector>


class Scene : public MovableGLM
{

public:
	enum axis{xAxis,yAxis,zAxis};
	enum transformations{xLocalTranslate,yLocalTranslate,zLocalTranslate,xGlobalTranslate,yGlobalTranslate,zGlobalTranslate,
		xLocalRotate,yLocalRotate,zLocalRotate,xGlobalRotate,yGlobalRotate,zGlobalRotate,xScale,yScale,zScale,xCameraTranslate,yCameraTranslate,zCameraTranslate};
	enum modes{POINTS,LINES,LINE_LOOP,LINE_STRIP,TRIANGLES,TRIANGLE_STRIP,TRIANGLE_FAN,QUADS};
	enum Shapes
	{
		Axis,
		Cube,
		Octahedron,
		Tethrahedron,
		BezierLine,
		BezierSurface,
		LineCopy,
		MeshCopy,
	};
	Scene();
	Scene(glm::vec3 position,float angle,float hwRelation,float near, float far);
	
	void addShapeFromFile(const std::string& fileName,int parent,unsigned int mode);
	virtual void addShape(int type,int parent,unsigned int mode);
	void addShapeCopy(int indx,int parent,unsigned int mode);
	
	void addShader(const std::string& fileName);
	void ZeroShapesTrans();

	//virtual void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);//

	virtual void Update(const glm::mat4 &MVP,const glm::mat4 &Normal,Shader *s) = 0;
	virtual void WhenTranslate(){};
	virtual void WhenRotate(){};
	virtual void Motion(){};
	virtual void Draw(int shaderIndx,int cameraIndx,bool debugMode);

	glm::mat4 GetViewProjection(int indx) const; 
	glm::mat4 GetShapeTransformation() const;
	
	void shapeTransformation(int type,float amt);
	void shapeRotation(glm::vec3 v, float ang,int indx);
	//void inline setPicked(int pickID){pickedShape = pickID;}
	float picking(int x,int y);
	void resize(int width,int hight);

	inline void setParent(int indx,int newValue) {chainParents[indx]=newValue;}
	inline float GetNear(int cameraIndx){return cameras[cameraIndx]->GetNear();}
	inline float GetFar(int cameraIndx){return cameras[cameraIndx]->GetFar();}
	inline float GetWHRelation(int cameraIndx){return cameras[cameraIndx]->GetWHRelation();}

	void ReadPixel();
	
	inline float GetAngle(int cameraIndx) {return cameras[cameraIndx]->GetAngle();}
	inline void Activate() {isActive = true;}
	inline void Deactivate() {isActive = false;}
	void HideShape(int shpIndx);
	//inline void SetMousePosition(double xpos, double ypos){xold =xpos; yold=ypos;}
	void updatePosition(float xpos, float ypos);
	void mouseProccessing(int button);
	bool inline IsActive() const { return isActive;} 
	virtual ~Scene(void);
	inline void SetDirection() {direction = -direction;}
	inline int GetDirection() {return direction;}
	glm::vec3 GetPositionInSystem(int indx,glm::vec3 pos);
	glm::vec3 GetVectorInSystem(int indx,glm::vec3 vec);
	void Scene::OpositeDirectionRot(glm::vec3 vec,float angle);
private:	

	std::vector<Camera*> cameras; //light will have the properties of camera
	
	Shape *axisMesh;
	int verticesSize;
	int indicesSize;

	float depth;
	int xold, yold,xrel, yrel;
	int cameraIndx;

protected:
	std::vector<Shape*> shapes;
	std::vector<Shader*> shaders;
	std::vector<int> chainParents;
	int pickedShape;
	int direction;
	static const int scaleFactor =2;
	bool isActive;
	void ScaleAllDirections(int factor);
};

