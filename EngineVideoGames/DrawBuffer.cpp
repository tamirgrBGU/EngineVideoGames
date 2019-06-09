#include "DrawBuffer.h"
#include "GL/glew.h"

DrawBuffer::DrawBuffer(void)
{
	glGenFramebuffers(1,&buffer);
	//glGenFramebuffers(1,&depthBuffer);
	glGenRenderbuffers(1,&depthBuffer);
	
	
	//Bind();
	//glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA8, 1200, 800 );
	//UnBind();
}


//void DrawBuffer::GetViewport(int *viewport)
//{
//	viewport[0] = left;
//	viewport[1] = bottom;
//	viewport[2] = width;
//	viewport[3] = height;
//}
//	
//void DrawBuffer::SetViewport(int _left, int _bottom, int _width,int _height)
//{
//	left = _left;
//	bottom = _bottom;
//	width = _width;
//	height = _height;
//		
//}
void DrawBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	CreateDepthBufferAttachment(1200,800);
	
}



void DrawBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_BUFFER);
}


void DrawBuffer::SetDrawDistination( int num,int mode)
{
	switch(mode)
	{
	case COLOR:
		    Bind();
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + num);
		break;
	case DEPTH:
		 Bind();
		glDrawBuffer(GL_DEPTH_ATTACHMENT);
		break;
	case STENCIL:
		 Bind();
			glDrawBuffer( GL_STENCIL_ATTACHMENT );
		break;
	case BACK:
		UnBind();
		glDrawBuffer(GL_BACK);
		break;
	case FRONT:
		UnBind();
		glDrawBuffer(GL_FRONT);
		break;
	case NONE:
		UnBind();
		glDrawBuffer(GL_NONE);
		break;
	default:
		UnBind();
		glDrawBuffer(GL_BACK);
	}
}

void DrawBuffer::CreateDepthBufferAttachment(int width,int height)
{
	
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthBuffer);
	
}

DrawBuffer::~DrawBuffer(void)
{
	glDeleteFramebuffers(1, &buffer);
	glDeleteRenderbuffers(1,&depthBuffer);
}
