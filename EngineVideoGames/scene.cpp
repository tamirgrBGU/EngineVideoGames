#define GLEW_STATIC
#include <GL\glew.h>
#include "scene.h"
#include <iostream>
#include <glm/gtc/quaternion.hpp>


using namespace std;
using namespace glm;



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
		cameras.push_back(new Camera(vec3(0,0,1.0f),vec3(0,0,-1.0f),800,800,60.0f,0.1f,100.0f));		
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	Scene::Scene(vec3 position,int width, int height,float angle,float near, float far)
	{
		//verticesSize = sizeof(vertices)/sizeof(vertices[0]);
		//
		//indicesSize = sizeof(indices)/sizeof(indices[0]) ; 
		glLineWidth(6);
		cameras.push_back(new Camera(position,-position,width,height,angle,near,far));
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
		
	}

	void Scene::addShapeFromFile(const std::string& fileName,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName,mode));
	}

	void Scene::addShape(int type, int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(type,mode));
	}

	void Scene::addShapeCopy(int indx,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(*shapes[indx],mode));
	}

	void Scene::addShader(const std::string& fileName)
	{
		shaders.push_back(new Shader(fileName));
		
	}

	void Scene::AddTexture(const std::string& textureFileName,bool for2D)
	{
		if(for2D)
		{
			texIndices.push_back(textures.size());
			plane2D->SetTexture(textures.size());
		}
		textures.push_back(new Texture(textureFileName));
	}

	void Scene::AddTexture(int width,int height,int mode)
	{
		texIndices.push_back(textures.size());
		plane2D->SetTexture(textures.size());
		textures.push_back(new Texture(width,height,mode));
	}

	void Scene::AddCamera(const glm::vec3& pos,int width,int height , float fov, float zNear, float zFar)
	{
		cameras.push_back(new Camera(pos,-pos,width,height,fov,zNear,zFar));
	}

	void Scene::AddBuffer(int texIndx,int mode)
	{
		buffers.push_back(new DrawBuffer());
		buffers.back()->Bind();
		textures[texIndx]->bindTex2Buffer(0,mode);
		buffers.back()->UnBind();
	}

	mat4 Scene::GetViewProjection(int indx) const
	{
		return cameras[indx]->GetViewProjection();
	}

	mat4 Scene::GetShapeTransformation() const
	{
		return shapes[pickedShape]->makeTrans();
	}

	void Scene::Draw(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode)
	{
		buffers.back()->SetDrawDistination(buffers.size()-1,buffer);
		
		glm::mat4 Normal = makeTrans();
		
		glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection() * Normal;
		glViewport(0,0,cameras[cameraIndx]->GetWidth(),cameras[cameraIndx]->GetHeight());
		int p = pickedShape;
		if(toClear)
		{
			if(shaderIndx>0)
				Clear(1,1,1,1);
			else
				Clear(0,0,0,0);
		}

		for (unsigned int i=0; i<shapes.size();i++)
		{
			if(shapes[i]->Is2Render())
			{
				mat4 Normal1 = mat4(1);
				pickedShape = i;
				for (int j = i; chainParents[j] > -1; j = chainParents[j])
				{
					Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
				}
			
				mat4 MVP1 = MVP * Normal1; 
				Normal1 = Normal * Normal1;

				MVP1 = MVP1 * shapes[i]->makeTransScale(mat4(1));
				Normal1 = Normal1 * shapes[i]->makeTrans();
				
				if(shaderIndx > 0)
				{
					Update(MVP1,Normal1,shapes[i]->GetShader());
					shapes[i]->Draw(shaders,textures,false);
					
				}
				else 
				{ //picking
					Update(MVP1,Normal1,0);
					shapes[i]->Draw(shaders,textures,true);
				}
			}
		}
		pickedShape = p;
	}

	void Scene::Draw2D(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode)
	{
		buffers.back()->SetDrawDistination(buffers.size()-1,buffer);
		glm::mat4 Normal = glm::mat4(1);
		
		glViewport(0,0,cameras[cameraIndx]->GetWidth(),cameras[cameraIndx]->GetHeight());
		
		if(toClear)
		{
			if(shaderIndx>0)
				Clear(1,1,1,1);
			else
				Clear(0,0,0,0);
		}
		Update2D(Normal,0,shaderIndx);
		plane2D->Draw(shaders,textures,false);
		
	}

	void Scene::Update2D(glm::mat4& mat, int time, const int shaderIndx)
	{
		Shader *s = shaders[shaderIndx];
		s->Bind();
		s->SetUniformMat4f("MVP",glm::mat4(1));
		s->SetUniformMat4f("Normal",mat);
		s->SetUniform1i("time", time);
		//glBindTexture(GL_TEXTURE_2D, m_texture);
		for(int i = 0; i<texIndices.size();i++)
		{
			char str[9];
			sprintf_s<9>(str,"sampler%d",i+1);
			s->SetUniform1i(str,textures[texIndices[i]]->GetSlot()); //to fix to slot
		}

	}

	void Scene::shapeRotation(vec3 v, float ang,int indx)
		{
			if(v.x >0.9999)
				shapes[indx]->globalSystemRot(ang,v,xAxis1);
			else 
				if(v.z >0.9999)
					shapes[indx]->globalSystemRot(ang,v,zAxis12);
			else
				shapes[indx]->globalSystemRot(ang,v,-1);
		}

	void Scene::shapeTransformation(int type,float amt)
	{
		   vec3 newAxis;
		if(glm::abs(amt)>1e-5)
		{
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
					myRotate(amt,vec3(0,0,1),zAxis1);
				else

					shapes[pickedShape]->myRotate(amt,vec3(0,0,1),zAxis1);
			break;
			case xGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(1,0,0),xAxis1);
				else
				{
					
					if(direction == -1 && pickedShape+2<shapes.size())
					{
						OpositeDirectionRot(glm::vec3(1,0,0),amt);
					}
					else
						shapes[pickedShape]->globalSystemRot(amt,vec3(1,0,0),xAxis1);
				}
				break;
			case yGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(0,1,0),-1);
				else
				{
					shapes[pickedShape]->globalSystemRot(amt,vec3(0,1,0),-1);
					if(direction == -1)
					{
						OpositeDirectionRot(glm::vec3(0,1,0),amt);
					}
					else
						shapes[pickedShape]->globalSystemRot(amt,vec3(1,0,0),xAxis1);
				}
				break;
			case zGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(0,0,1),zAxis12);
				else
				{
					
					
					if(direction == -1 && pickedShape+2<shapes.size())
					{
						OpositeDirectionRot(glm::vec3(0,0,1),amt);
					}
					else
						shapes[pickedShape]->globalSystemRot(amt,vec3(0,0,1),zAxis12);
				}
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
					for (; chainParents[i] > 0; i = chainParents[i]);
				
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
						for (; chainParents[i] > 0; i = chainParents[i]);
						
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
						for (; chainParents[i] > 0; i = chainParents[i]);
			
						shapes[i]->translateInSystem(*this,vec3(0,0,amt),0,false);
					}
			break;
			default:
				break;
			}

		}

	}

	void Scene::OpositeDirectionRot(glm::vec3 vec,float angle)
	{
		
		int i=pickedShape;
		glm::mat4 globalMat = glm::mat4(1)*shapes[pickedShape+2]->GetRot();
		glm::mat4 localMat = glm::mat4(1)*shapes[pickedShape+2]->GetRot();
		for (; chainParents[i] >= 0; i = chainParents[i])
			localMat =  shapes[i]->GetRot() * localMat;
		i=1;
		std::cout<<"vec1 ("<<vec.x <<", "<<vec.y<<", "<<vec.z<<")"<<std::endl;
		glm::mat4 rotMat = localMat*glm::rotate(glm::mat4(1),angle,vec)*glm::transpose(localMat); 
		
		glm::vec3 posStart = GetTipPositionInSystem(pickedShape);
		std::cout<<"start ("<<posStart.x <<", "<<posStart.y<<", "<<posStart.z<<")"<<std::endl;
		
		shapes[pickedShape]->zeroRot(true);

		shapes[i]->buildAngMatrices(rotMat*shapes[i]->GetRot());
		//rotMat = rotMat*shapes[i]->GetRot();
		//shapes[i]->SetRotMatrix(rotMat);
		
		rotMat = globalMat*glm::rotate(glm::mat4(1),-angle,vec)*glm::transpose(globalMat);
		rotMat = rotMat*shapes[pickedShape+2]->GetRot();
		shapes[pickedShape+2]->buildAngMatrices(rotMat);
		//if(vec.x>0.99)
		//	shapes[pickedShape+2]->myRotate(-angle,vec,xAxis1);
		//else
		//	shapes[pickedShape+2]->myRotate(-angle,vec,zAxis2);
		
		glm::vec3 posEnd = GetTipPositionInSystem(pickedShape);
		std::cout<<"end ("<<posEnd.x <<", "<<posEnd.y<<", "<<posEnd.z<<")"<<std::endl;
		shapes[i]->myTranslate((posStart-posEnd)/glm::vec3(1,1,(float)(scaleFactor)),0);
	}
	
	void Scene::resize(int width,int height)
	{
		glViewport(0,0,width,height);
		
		cameras[0]->setProjection(width,height,cameras[cameraIndx]->GetNear(),cameras[cameraIndx]->GetFar());
	}

	float Scene::picking(int x,int y)
	{
		//float depth;
		
		Draw(0,0,BACK,true,false); 
		
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
			yold = y;
		}
		return depth;
	}

	//return coordinates in global system for a tip of arm position is local system 
	glm::vec3 Scene::GetTipPositionInSystem(int indx)
	{
		mat4 Normal1 = mat4(1);
		if(indx>0)
		{
			int j = indx;
			glm::vec3 vec = glm::vec3(0);
			for (;  chainParents[j] > 0; j = chainParents[j])
			{
				vec = shapes[j]->getVectorInSystem(glm::mat4(1),vec + glm::vec3(0,0,2));
			}
			glm::vec3 origin = vec3(shapes[j]->getTraslate());
			return origin + shapes[j]->getVectorInSystem(glm::mat4(1),vec+glm::vec3(0,0,2))*(float)scaleFactor;
			//return shapes[indx]->getTipPos(Normal1);
		}
		else if(indx ==0)
			return vec3(shapes[0]->getTraslate());
		else
		{
			return vec3(0,0,0); 
		}
	}

	glm::vec3 Scene::GetVectorInSystem(int indx,glm::vec3 vec)
	{
		mat4 Normal1 = mat4(1);
		if(indx>-1)
		{
			for (int j = indx;  chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return vec3(Normal1*vec4(vec,0));//shapes[indx]->getVectorInSystem(Normal1,vec);
		}
		else
		{
			return vec; 
		}
	}
	
	void Scene::mouseProccessing(int button)
	{
		if(pickedShape == -1 || shapes[pickedShape]->Is2D())
		{
			if(button == 1 )
			{				
				GLint viewport[4];
				//float zTmp = 2.0*depth -1.0;
				glGetIntegerv(GL_VIEWPORT, viewport);
				float z=cameras[cameraIndx]->GetFar()+depth*(cameras[cameraIndx]->GetNear()-cameras[cameraIndx]->GetFar());
				float transX = cameras[cameraIndx]->GetWHRelation()*(xrel)/(float) (viewport[2])*cameras[cameraIndx]->GetNear()*2.0f*tan(cameras[cameraIndx]->GetAngle()*M_PI/360.0)*(cameras[cameraIndx]->GetFar()/z);
				float transY =(yrel)/(float) (viewport[3])*cameras[cameraIndx]->GetNear()*2.0f*tan(cameras[cameraIndx]->GetAngle()*M_PI/360.0)*(cameras[cameraIndx]->GetFar()/z);

				shapeTransformation(xCameraTranslate,-transX);
				shapeTransformation(yCameraTranslate,transY);
				WhenTranslate();
			}
			else
			{
				shapeTransformation(zGlobalRotate,xrel*.5f);
				shapeTransformation(xGlobalRotate,yrel*.5f);
				WhenRotate();
			}
		}
	}

	void Scene::ZeroShapesTrans()
	{
		for (unsigned int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->zeroTrans();
			shapes[i]->zeroRot(false);
			shapes[i]->zeroRot(true);
		}
	}

	void Scene::ReadPixel()
	{
		glReadPixels(1,1,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
	}

	void Scene::updatePosition(float xpos, float ypos)
	{
		xrel = xold - xpos;
		yrel = yold - ypos;
		xold = xpos;
		yold = ypos;
	}

	void Scene::HideShape(int shpIndx)
	{
		if(shapes[shpIndx]->Is2Render())
			shapes[shpIndx]->Hide();
		else 
			shapes[shpIndx]->Unhide();
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
	for(Texture* tex: textures)
		{
			delete tex;
		}

	for(DrawBuffer* buf: buffers)
		{
			delete buf;
		}
	if(plane2D)
		delete plane2D;

}

void Scene::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
	 
	void Scene::ScaleAllDirections(int factor)
	{
		shapeTransformation(xScale,factor);
		shapeTransformation(yScale,factor);
		shapeTransformation(zScale,factor);
	}