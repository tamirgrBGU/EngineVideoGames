#define GLEW_STATIC
#include <GL\glew.h>
#include "scene.h"
#include <iostream>
#include <glm/gtc/quaternion.hpp>

int Mesh::shpsNum = 0;
using namespace std;
using namespace glm;

	LineVertex axisVertices[] = 
	{
		LineVertex(glm::vec3(1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(-1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(0,1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,-1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,0,1),glm::vec3(0,0,1)),
		LineVertex(glm::vec3(0,0,-1),glm::vec3(0,0,1)),
	};


	 unsigned int axisIndices[] = 
	{
		0,1,
		2,3,
		4,5
	};

	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};



	unsigned int indices[] =	{0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	                          };


	static void printMat(const mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}
	Scene::Scene()
	{
		//verticesSize = sizeof(vertices)/sizeof(vertices[0]);
		//
		//indicesSize = sizeof(indices)/sizeof(indices[0]) ; 
		glLineWidth(3);
		
		cameras.push_back(new Camera(vec3(0,0,-20.0f),60.0f,1.0f,0.1f,100.0f));
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	Scene::Scene(vec3 position,float angle,float hwRelation,float near, float far)
	{
		//verticesSize = sizeof(vertices)/sizeof(vertices[0]);
		//
		//indicesSize = sizeof(indices)/sizeof(indices[0]) ; 
		glLineWidth(3);
		cameras.push_back(new Camera(position,angle,hwRelation,near,far));
	//	axisMesh = new Shape(axisVertices,sizeof(axisVertices)/sizeof(axisVertices[0]),axisIndices, sizeof(axisIndices)/sizeof(axisIndices[0]));
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	void Scene::addBox(const std::string& fileName, int parent)
	{
		unsigned int numVertices = sizeof(vertices)/sizeof(vertices[0]);
		
		unsigned int numIndices = sizeof(indices)/sizeof(indices[0]) ; 
		chainParents.push_back(-1);
		vaos.push_back(new VertexArray());
		//vaos.back()->Bind();
		if(fileName=="")
			shapes.push_back(new Shape((Vertex*)vertices,numVertices,indices,numIndices,vaos.back()));
		else
			shapes.push_back(new Shape((Vertex*)vertices,numVertices,indices,numIndices,fileName,vaos.back()));
	}
	void Scene::addAxis(){}

	void Scene::addShape(int Cylparts,int linkPosition,int parent)
	{
		if(parent<0)
		{
			vaos.push_back(new VertexArray());
			
		}
		vaos.back()->Bind();
		chainParents.push_back(-1);
		shapes.push_back(new Shape(Cylparts,linkPosition,vaos.back()));
	}
	void Scene::addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent)
	{
		if(parent<0)
		{
			vaos.push_back(new VertexArray());
			
		}
		//vaos.back()->Bind();
		chainParents.push_back(-1);
		shapes.push_back(new Shape(CylParts,linkPosition,textureFileName,vaos.back()));
	}
	void Scene::addShape(int type,int parent)
	{
		if(parent<0)
		{
			vaos.push_back(new VertexArray());
			
		}
		vaos.back()->Bind();
			chainParents.push_back(-1);
		shapes.push_back(new Shape(*shapes[type]));
	}

	void Scene::addShape(const std::string& fileName,int parent)
	{
		if(parent<0)
		{
			vaos.push_back(new VertexArray());
			
		}
		vaos.back()->Bind();
		chainParents.push_back(-1);
		shapes.push_back(new Shape(fileName,vaos.back()));
	}

	void Scene::addShape(const std::string& fileName,const std::string& textureFileName,int parent)
	{
		if(parent<0)
		{
			vaos.push_back(new VertexArray());
			
		}
		vaos.back()->Bind();
		chainParents.push_back(-1);
		shapes.push_back(new Shape(fileName,textureFileName,vaos.back()));
	}

	void Scene::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,int parent)
	{
		if(parent<0)
		{
			vaos.push_back(new VertexArray());
			
		}
		vaos.back()->Bind();
		chainParents.push_back(-1);
		shapes.push_back(new Shape(vertices,numVertices,indices,numIndices,vaos.back()));
	}
	
	void Scene::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName,int parent)
	{

		if(parent<0)
		{
			vaos.push_back(new VertexArray());
			vaos.back()->Bind();
		}
		chainParents.push_back(-1);
		shapes.push_back(new Shape(vertices,numVertices,indices,numIndices,textureFlieName,vaos.back()));
	}

	void Scene::addShader(const std::string& fileName)
	{
		shaders.push_back(new Shader(fileName));
		if(!axisMesh)
		{
			vaos.back()->Bind();
			axisMesh = new Shape(axisVertices,sizeof(axisVertices)/sizeof(axisVertices[0]),axisIndices, sizeof(axisIndices)/sizeof(axisIndices[0]),vaos.back());
			axisMesh->myScale(vec3(2.0*scaleFactor,2.0*scaleFactor,2.0*scaleFactor));
			axisMesh->myTranslate(vec3(0,0,0.5),1);
		}
	}

	mat4 Scene::GetViewProjection(int indx) const
	{
		return cameras[indx]->GetViewProjection();
	}

	mat4 Scene::GetShapeTransformation() const
	{
		return shapes[pickedShape]->makeTrans();
	}

	void Scene::Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx)
	{

		//shaders[shaderIndx]->SetUniformMat4f("MVP", MVP);
		//shaders[shaderIndx]->SetUniformMat4fv("jointTransforms", jointTransforms);
		//shaders[shaderIndx]->SetUniform4f("lightDirection", 0.0f, 0.0f, 1.0f,0.0f);
		////glUniform3f(m_uniforms[3], r/255.0f, g/255.0f, b/255.0f);
		//shaders[shaderIndx]->SetUniform4f("lightColor", 0.9f, 0.8f, 0.7f,1.0f);
		//shaders[shaderIndx]->SetUniform1i("linksNum",Mesh::shpsNum); 
	}

	void Scene::draw(int shaderIndx,int cameraIndx,bool drawAxis)
	{
		const int length = 40;
		glm::mat4 Normals[length];
		glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection();
		glm::mat4 global =  makeTrans();
		int counter = 1,firstCell = 0 ;
		//Normals[0] = makeTrans();
		//for (int i = 1; i < length; i++)
		//{
		//	Normals[i] = Normals[0]; 
		//}
		int vaoIndx = -1;
		shaders[shaderIndx]->Bind();
		Normals[0] = global*shapes[0]->makeTransScale();
		for (int i=1; i<shapes.size();i++)
		{
			//int j = i;
//			int counter = 0;
			
			glm::mat4 Normal1 = shapes[i]->makeTransScale();
			/*if(chainParents[i] == -1)
			{
				vaoIndx++;
				vaos[vaoIndx]->Bind();
			}*/
			//vaos[i]->Bind();
			for (int j = i; chainParents[j] > -1; j = chainParents[j])
			{
				
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			
			//mat4 MVP1 = MVP * Normal1; 
			Normals[i] = global * Normal1;
			if(chainParents[i] < 0)
			{
				Update(MVP,&Normals[firstCell],counter,shaderIndx);
				for (int k = 0; k < counter; k++)
				{				
					if(shaderIndx==0 && drawAxis && chainParents[firstCell + k]>=0)
					{
						axisMesh->draw(GL_LINES);
					}
					if(shaderIndx == 1)
						shapes[firstCell + k]->draw(GL_TRIANGLES);
					else 
						shapes[firstCell + k]->draw(GL_TRIANGLES);
				}
				counter = 1;
				firstCell = i;
				
			}
			else
				counter++;
	//		glm::quat q1 = glm::quat_cast(Normals[i]);
	
			
			//glm::vec4 qd = Normals[i][3];
			//
			//for (int j = 0; j < 4; j++)
			//{
			//	printf("%f ", qd[j]);
			//	Normals[i][j][0] = q1[j];
			//	Normals[i][j][1] = qd[j];
			//	//Normals[i][0][j] = q1[j];
			//	//	Normals[i][1][1] = qd[j];
			//}
			//	printf("\n");
		
			//if(shaderIndx==0 && drawAxis && chainParents[i]>=0)
			//{
			//	shaders[shaderIndx]->Update(axisMesh->makeTransScale(MVP),Normals,0);
			//	axisMesh->draw(GL_LINES);
			//}

			//MVP1 = MVP1 * shapes[i]->makeTransScale(mat4(1));
			//Normals[i] = Normal1 * shapes[i]->makeTrans();
			//
			//
			//if(shaderIndx == 1)
			//	shapes[i]->draw(GL_TRIANGLES);
			//else 
			//	shapes[i]->draw(GL_TRIANGLES);
		}
//		Update(MVP,&Normals[firstCell],counter,shaderIndx);
		//vaos[0]->Bind();
			
		Update(MVP,&Normals[firstCell],counter,shaderIndx);
				if(shaderIndx==0 && drawAxis && chainParents[firstCell]>=0)
				{
					axisMesh->draw(GL_LINES);
				}
				if(shaderIndx == 1)
					shapes[firstCell]->draw(GL_TRIANGLES);
				else 
					shapes[firstCell]->draw(GL_TRIANGLES);
		//for (int i = 0; i < shapes.size(); i++)
		//{

		//vaos[0]->Unbind();

		//	
		//	if(shaderIndx == 1)
		//		shapes[i]->draw(GL_TRIANGLES);
		//	else 
		//		shapes[i]->draw(GL_TRIANGLES);
		//}
		//if(shaderIndx==0 )
		//{
		//	shaders[shaderIndx]->Bind();
		//	//shaders[shaderIndx]->Update(cameras[0]->GetViewProjection()*scale(vec3(10,10,10)),Normals);
		//	axisMesh->draw(GL_LINES);
		//}
	}

	 void Scene::shapeRotation(vec3 v, float ang,int indx)
		{
			if(v.x >0.999)
				shapes[indx]->myRotate(ang,v,xAxis1);
			else 
				if(v.z >0.999)
					shapes[indx]->myRotate(ang,v,zAxis12);
			else
				shapes[indx]->myRotate(ang,v,-1);
		}

	void Scene::shapeTransformation(int type,float amt)
	{
		    vec3 newAxis;
			switch (type)
			{
			case xLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(amt,0,0),1);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(amt,0,0),1);
				}
				break;
			case yLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt,0),1);
				else
								{
						int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
						shapes[i]->myTranslate(vec3(0,amt,0),1);
					}
				break;
			case zLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt),1);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,0,amt),1);
				}
				break;
			case xGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(amt/5.0,0,0),0);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(amt,0,0),0);
				}
				break;
			case yGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt/5.0,0),0);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,amt,0),0);
				}
				break;
			case zGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt/5.0),0);
								else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,0,amt),0);
				}
				break;
			case xLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(1,0,0),xAxis1);
				else
					shapes[pickedShape]->myRotate(amt,vec3(1,0,0),xAxis1);
				break;
			case yLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(0,1,0),-1);
				else
					shapes[pickedShape]->myRotate(amt,vec3(0,1,0),-1);
				break;
			case zLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(0,0,1),zAxis12);
				else
					shapes[pickedShape]->myRotate(amt,vec3(0,0,1),zAxis12);
			break;
			case xGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(1,0,0),xAxis1);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(1,0,0),xAxis1);
				break;
			case yGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(0,1,0),-1);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(0,1,0),-1);
				break;
			case zGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(0,0,1),zAxis12);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(0,0,1),zAxis12);
			break;
			case xScale:
				if(pickedShape ==-1)
					myScale(vec3(amt,1,1));
				else
					shapes[pickedShape]->myScale(vec3(amt,1,1));
			break;
			case yScale:
				if(pickedShape ==-1)
					myScale(vec3(1,amt,1));
				else
					shapes[pickedShape]->myScale(vec3(1,amt,1));
			break;
			case zScale:
				if(pickedShape ==-1)
					myScale(vec3(1,1,amt));
				else
					shapes[pickedShape]->myScale(vec3(1,1,amt));
			break;
			case xCameraTranslate: //camera plane translate
				if(pickedShape == -1)
					myTranslate(vec3(amt/5.0,0,0),0);
				else
				{
					//newAxis = findAxis(vec3(1,0,0));					
						int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
				
					shapes[i]->translateInSystem(*this,vec3(amt,0,0),0,false);
				}
				break;
			case yCameraTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt/5.0,0),0);
				else
					{
						//newAxis = findAxis(vec3(0,1,0));
							int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
						
						shapes[i]->translateInSystem(*this,vec3(0,amt,0),0,false);
					}
			break;
			case zCameraTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt/5.0),0);
				else
					{
					//	newAxis = findAxis(vec3(0,0,1));
							int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
			
						shapes[i]->translateInSystem(*this,vec3(0,0,amt),0,false);
					}
			break;
			default:
				break;
			}

		
	}
	
	void Scene::resize(int width,int height)
	{
		glViewport(0,0,width,height);
		cameras[0]->setProjection((float)width/(float)height,cameras[cameraIndx]->GetNear(),cameras[cameraIndx]->GetFar());
	}

	float Scene::picking(double x,double y)
	{
		//float depth;
		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw(1,0,false); 
						
		GLint viewport[4];  
		unsigned char data[4];
		glGetIntegerv(GL_VIEWPORT, viewport); //reading viewport parameters
		glReadPixels(x,viewport[3] - y,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glReadPixels(x,viewport[3] - y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
		cout<<"depth "<<depth<<endl;
		int pickedID = data[0] + data[1]*256 + data[2]*256*256 - 1;
		if(data[0] == 0 && data[1] == 0 && data[2] == 0)
		{
			pickedShape = -1;
			//std::cout<<"not picked"<<std::endl;
		}
		else
		{
			pickedShape = pickedID;
			std::cout<<"picked "<<pickedID<<std::endl;
			xold = x;
			yold =  y;
		}
		return depth;
	}

	vec3 Scene::getTipPosition(int indx)
	{
		mat4 Normal1 = mat4(1);
		if(indx>-1)
		{
			for (int j = indx;  chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1,vec3(0,0,1));
			//return shapes[indx]->getTipPos(Normal1);
		}
		else
		{
			return shapes[0]->getPointInSystem(mat4(1),vec3(0,0,-1)); 
				//shapes[0]->getRootPos(mat4(1));
		}
	}

	vec3 Scene::getDistination(int indx)
	{
		mat4 Normal1 = mat4(1);
		if( indx>-1)
		{
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1,vec3(0,0,0));
			//return shapes[indx]->getCenterOfRotation(Normal1);
		}
		else
		{
			return vec3(0,0,0);
		}
	}

	vec3 Scene::getAxisDirection(int indx,int axis)
	{
		if(axis == xAxis)
		{
			mat4 Normal1 = mat4(1);
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1,vec3(1,0,0)); 
				//shapes[indx]->getXdirection(Normal1);
		}
		else
		{
			mat4 Normal1 = mat4(1);
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getVectorInSystem(Normal1,vec3(0,0,1)); 
				//shapes[indx]->getZdirection(Normal1);
		}

	}
	
	void Scene::mouseProccessing(int button)
	{

			if(button == 1)
			{				
				GLint viewport[4];
				//float zTmp = 2.0*depth -1.0;
				glGetIntegerv(GL_VIEWPORT, viewport);
				float z=cameras[cameraIndx]->GetFar()+depth*(cameras[cameraIndx]->GetNear()-cameras[cameraIndx]->GetFar());
				float transX = cameras[cameraIndx]->GetWHRelation()*(xrel)/(float) (viewport[2])*cameras[cameraIndx]->GetNear()*2.0*tan(cameras[cameraIndx]->GetAngle()*M_PI/360.0)*(cameras[cameraIndx]->GetFar()/z);
				float transY =(yrel)/(float) (viewport[3])*cameras[cameraIndx]->GetNear()*2.0*tan(cameras[cameraIndx]->GetAngle()*M_PI/360.0)*(cameras[cameraIndx]->GetFar()/z);

				shapeTransformation(xCameraTranslate,transX);
				shapeTransformation(yCameraTranslate,transY);
				//changeDistPos();
			}
			else
			{
				shapeTransformation(zGlobalRotate,xrel*.5);
				shapeTransformation(xGlobalRotate,yrel*.5);
			}		
	}

	void Scene::updatePosition(double xpos, double ypos)
	{
		xrel = xold - xpos;
		yrel = yold - ypos;
		xold = xpos;
		yold = ypos;
	}

	Scene::~Scene(void)
{
	for (Shape* shp : shapes)
		{
			delete shp;
		}
	for (Camera* cam : cameras)
		{
			delete cam;
		}
	for(Shader* sdr: shaders)
		{
			delete sdr;
		}
	for(VertexArray* vao: vaos)
		{
			delete vao;
		}
	delete axisMesh;
}
