/*
 * IndexBuffer.cpp
 *
 *  Created on: 13 de dez de 2017
 *      Author: Casa
 */
#define GLEW_STATIC
#include <GL\glew.h>
#include "IndexBuffer.hpp"
#include "Log.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count){

	ASSERT(sizeof(unsigned int)== sizeof(GLuint));
	GLCall(glGenBuffers(1,&m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(const IndexBuffer &ib) {
	
	int size;
	GLCall(glGenBuffers(1, &m_RendererID));
	
	GLCall(glBindBuffer(GL_COPY_READ_BUFFER, ib.m_RendererID));
	  GLCall(glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size));
	

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW));

	GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, size));

}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1,&m_RendererID));
}

void IndexBuffer::Bind() const{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
