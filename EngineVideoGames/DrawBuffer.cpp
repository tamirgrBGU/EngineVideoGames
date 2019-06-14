#include "DrawBuffer.h"
#include "GL/glew.h"

DrawBuffer::DrawBuffer(int width,int height)
{

	CreateColorBufferAttachment(width,height);
	CreateDepthBufferAttachment(width,height);
	
	
}

void DrawBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	
	
}


void DrawBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_BUFFER);
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

void DrawBuffer::CreateColorBufferAttachment(int width,int height)
{
	glGenFramebuffers(1,&buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glGenRenderbuffers(1,&buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA8,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DrawBuffer::CreateDepthBufferAttachment(int width,int height)
{

	glGenRenderbuffers(1,&depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthBuffer);
	
}

void DrawBuffer::resize(int width,int height)
{  	
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA8,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,buffer);

	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DrawBuffer::~DrawBuffer(void)
{
	glDeleteFramebuffers(1, &buffer);
	glDeleteRenderbuffers(1,&depthBuffer);
}
