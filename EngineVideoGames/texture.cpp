#define GLEW_STATIC
#include "texture.h"
#include <GL\glew.h>
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& fileName)
{
	int width, height, numComponents;
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);
	
    if(data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;
        
    glGenTextures(1, &m_texture);
    Bind(m_texture);
        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_LOD_BIAS,-0.4);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
	isReadFromBuffer = false;
}

Texture::Texture(int width, int height,int mode)
{
	glGenTextures(1, &m_texture);
    Bind(m_texture);

    switch(mode)
	{
	case COLOR:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); //note GL_RED internal format, to save memory.
		break;
	case DEPTH:
		glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		break;
	case STENCIL:
			glTexImage2D (GL_TEXTURE_2D, 0, GL_STENCIL_INDEX16, width, height, 0, GL_STENCIL_COMPONENTS, GL_UNSIGNED_BYTE, NULL);
		break;
	default:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); //note GL_RED internal format, to save memory.
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_LOD_BIAS,-0.4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);      //GL_REPEAT specified so that CGOL is simulated on a torus.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	isReadFromBuffer = true;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::bindTex2Buffer( int num,int mode)
{
	switch(mode)
	{
	case COLOR:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num , m_texture, 0);
		break;
	case DEPTH:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , m_texture, 0);
		break;
	case STENCIL:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT , m_texture, 0);
		break;
	default:
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + num , m_texture, 0);
	}
	
	
}

void Texture::Resize(int width,int height)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,width,height);
}
