#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"
#include "bezier1D.h"
#include "kdtree.h"
#include "Mesh.h"

class B_Node 
{
private:
	BoundingBox bb;
	int low;
	int high;
	B_Node* left;
	B_Node* right;

	BoundingBox getBoundingbox(int low, int high,std::vector<glm::vec3>* points[]);
	BoundingBox getDefultBoundingbox(glm::vec3 point);
public:
	B_Node(int low, int high, std::vector<glm::vec3>* points[]);
	B_Node(const B_Node &other);
	bool isColiding(glm::mat4 myMat, B_Node &other,glm::mat4 otherMat);

	~B_Node(void);
};

class MeshConstructor
{
	VertexArray vao;
	IndexBuffer *ib;
	//TO DO: add bounding box data base and build it in the constructor 
	B_Node boundingTree;
	std::vector<VertexBuffer*> vbs;
	bool is2D;
	int unsigned indicesNum;
	
	void InitLine(IndexedModel &model);
	void InitMesh(IndexedModel &model);
	void CopyMesh(const MeshConstructor &mesh);
	void CopyLine(const MeshConstructor &mesh);
	static const unsigned int VEC3_ATTRIB_NUM = 4;
	static const unsigned int VEC2_ATTRIB_NUM = 1;
	
public:
	//TO DO: add collision detection function which get other MeshConstructor and Mat4 of related transformasions. The function may return a pointer to the relevant Bounding Box when collide
	enum SimpleShapes
	{
		Axis,
		Cube,
		Octahedron,
		Tethrahedron,
		BezierLine,
		BezierSurface,
	};
	MeshConstructor(const int type);
	MeshConstructor(Bezier1D *curve,bool isSurface,unsigned int resT,unsigned int resS);
	MeshConstructor(const MeshConstructor &mesh);
	MeshConstructor(const std::string& fileName);

	void Bind() {vao.Bind();}
	void Unbind() {vao.Unbind();}
	inline unsigned int GetIndicesNum(){return indicesNum;}
	inline bool Is2D() {return is2D;}
	~MeshConstructor(void);

};

