#include "DrawBuffer.h"
#include "GL/glew.h"

DrawBuffer::DrawBuffer(void)
{
	glGenFramebuffers(1,&buffer);
	
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
}

void DrawBuffer::SetDrawDistination( int num)
{
	if(num>=0)
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + num);
	else 
		glDrawBuffer(GL_BACK);
}



DrawBuffer::~DrawBuffer(void)
{
}
