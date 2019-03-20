/*
 * VertexBuffer.cpp
 *
 *  Created on: 13 de dez de 2017
 *      Author: Casa
 */
#define GLEW_STATIC
#include <GL\glew.h>
#include "VertexBuffer.hpp"
#include "Log.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	GLCall(glGenBuffers(1,&m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER,size, data, GL_STATIC_DRAW));
}


VertexBuffer::VertexBuffer(const VertexBuffer &vb) {
	int size;
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_COPY_READ_BUFFER, vb.m_RendererID));
    GLCall(glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size));

    GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW));
	GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::copy()
{
	int size;
	GLCall(glBindBuffer(GL_COPY_READ_BUFFER, m_RendererID));
   GLCall( glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size));
   
	GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1,&m_RendererID));
}

void VertexBuffer::Bind()const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind()const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
