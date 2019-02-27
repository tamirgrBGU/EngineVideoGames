#include "shape.h"

Shape::Shape(const Shape& shape)
{
	if(shape.mesh)
		mesh = shape.mesh;
	if(shape.lineMesh)
		lineMesh = shape.lineMesh;
	tex = shape.tex;
	isCopy = true;
	vao = new VertexArray();
}
Shape::Shape(const std::string& fileName,VertexArray *_vao){
	mesh = new Mesh(fileName,_vao);
	tex = 0;
	isCopy = false;
	vao = _vao;
}

Shape::Shape(const std::string& fileName,const std::string& textureFileName,VertexArray *_vao){
	mesh = new Mesh(fileName,_vao); 
	tex = new Texture(textureFileName);
	isCopy = false;
	vao = _vao;
}

Shape::Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,VertexArray *_vao)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices,_vao);
	tex = 0; 
	isCopy = false;
	vao = _vao;
}

Shape::Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,VertexArray *_vao)
{

	lineMesh = new LineMesh(vertices,numVertices,indices,numIndices);
	tex = 0; 
	isCopy = false;
	vao = _vao;
}


Shape::Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName,VertexArray *_vao)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices,_vao);
	tex = new Texture(textureFileName);
	isCopy = false;
	vao = _vao;
}

	Shape::Shape(int CylParts,int linkPosition,VertexArray *_vao)
	{
		mesh = new Mesh(CylParts,linkPosition,_vao);
		tex = 0; 
		isCopy = false;
		vao = _vao;
	}

	Shape::Shape(int CylParts,int linkPosition,const std::string& textureFileName,VertexArray *_vao)
	{
		mesh = new Mesh(CylParts,linkPosition,_vao);
		tex = new Texture(textureFileName); 
		isCopy = false;
		vao = _vao;
	}

void Shape::addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices,vao);
	tex = new Texture(textureFileName);
}

void Shape::addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices,vao);
}

void Shape::addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	lineMesh = new LineMesh(vertices,numVertices,indices,numIndices);
}

void Shape::draw(int mode)
{
	//transformations
	if(tex)
		tex->Bind();
	if(mesh)
		mesh->Draw(mode);
	if(lineMesh)
		lineMesh->Draw();
}

Shape::~Shape(void)
{
	if(!isCopy)
	{
		if(mesh)
 			delete mesh;
		if(lineMesh)
			delete lineMesh;
		if(tex)
			delete tex;
	}
	else
		if(vao)
			delete vao;
}
