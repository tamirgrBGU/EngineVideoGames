/*
 * VertexArray.cpp

 *
 *  Created on: 22 de dez de 2017
 *      Author: Casa
 */
#define GLEW_STATIC
#include <GL\glew.h>
#include "VertexArray.hpp"
#include "Log.hpp"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1,&m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1,&m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb,int attribNum,int count, int type)
{
		//vb.Bind();
		GLCall(glEnableVertexAttribArray(attribNum));
		GLCall(glVertexAttribPointer(attribNum,count,type,GL_FALSE, 0,0));
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}

