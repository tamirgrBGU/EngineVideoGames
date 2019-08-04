#define GLEW_STATIC
#include <GL\glew.h>
#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "bezier2D.h"
#include "obj_loader.h"


MeshConstructor::MeshConstructor(const int type)
{
	switch (type)
	{
	case Axis:	
		InitLine(AxisGenerator());
		break;
	case Cube:
		 InitMesh(CubeTriangles());
		 break;
	case Octahedron:
		 InitMesh(OctahedronGenerator());
		 break;
	case Tethrahedron:
		 InitMesh(TethrahedronGenerator());
		 break;
	default:
		break;
	}
	
}

MeshConstructor::MeshConstructor(const std::string& fileName)
{
	IndexedModel im = OBJModel(fileName).ToIndexedModel();
	if (im.positions.size() > 0)
		InitMesh(im);
	else
		printf("failed to read file\n");
}

const int circularSubdivisions = 4;
MeshConstructor::MeshConstructor(Bezier1D *curve, bool isSurface, unsigned int resT, unsigned int resS)
{
	if(isSurface)
	{
		Bezier2D surface(*curve, circularSubdivisions);
		InitMesh(surface.GetSurface(resT,resS));		
	}
	else
	{		
		InitLine(curve->GetLine(resT));
	}
}

MeshConstructor::MeshConstructor(const IndexedModel &model){
	InitMesh(model);
}

MeshConstructor::MeshConstructor(const MeshConstructor &mesh)
{
	indicesNum = mesh.indicesNum;
	if(mesh.is2D)
		CopyMesh(mesh);
	else
		CopyLine(mesh);
}

MeshConstructor::~MeshConstructor(void)
{
	if(ib)
		delete ib;
	for (unsigned int i = 0; i < vbs.size(); i++)
	{
		if(vbs[i])
			delete vbs[i];
	}
}

void MeshConstructor::InitLine(IndexedModel &model){
	
	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();
	
	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i),verticesNum*sizeof(model.positions[0])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	
	ib = new IndexBuffer((unsigned int*)model.GetData(5),indicesNum);
	
	vao.Unbind();
	is2D = false;
	
}

std::vector<glm::vec3>& MeshConstructor::getlastInitMeshPositions() {
	//printf("%d meakss!\n", lastInitMeshPositions.size());
	return lastInitIndexedModel.positions;
}

IndexedModel& MeshConstructor::getlastIndexedModel() {
	//printf("%d meakss!\n", lastInitMeshPositions.size());
	return lastInitIndexedModel;
}

void MeshConstructor::InitMesh(const IndexedModel &model){
	lastInitIndexedModel = model;

	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();
	
	vao.Bind();
	int i = 0;
	for (; i < VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum*sizeof(model.positions[0])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	for(;i < VEC2_ATTRIB_NUM + VEC3_ATTRIB_NUM;i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum*sizeof(model.texCoords[0])));	
		vao.AddBuffer(*vbs.back(),i,2,GL_FLOAT);
	}

	ib = new IndexBuffer((unsigned int*)model.GetData(5), indicesNum);
	
	vao.Unbind();
	is2D = true;
	
}

void MeshConstructor::CopyLine(const MeshConstructor &mesh){
	
	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	
	ib = new IndexBuffer(*mesh.ib);
	
	vao.Unbind();

	is2D = false;
	
}

void MeshConstructor::CopyMesh(const MeshConstructor &mesh){

	vao.Bind();
	int i = 0;
	for (; i < VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));	
		vao.AddBuffer(*vbs.back(),i,3,GL_FLOAT);
	}
	for(;i < VEC2_ATTRIB_NUM + VEC3_ATTRIB_NUM;i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));	
		vao.AddBuffer(*vbs.back(),i,2,GL_FLOAT);
	}
	
	ib = new IndexBuffer(*mesh.ib);
	//ib = mesh.ib;
	vao.Unbind();

	is2D = true;
	
}
