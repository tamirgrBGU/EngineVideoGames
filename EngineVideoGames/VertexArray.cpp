/*
 * VertexArray.cpp

 *
 *  Created on: 22 de dez de 2017
 *      Author: Casa
 */
#define GLEW_STATIC
#include <GL\glew.h>
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Log.hpp"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1,&m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1,&m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb,int attribNum,int count, int type)
{
		vb.Bind();
		GLCall(glEnableVertexAttribArray(attribNum));
		GLCall(glVertexAttribPointer(attribNum,count,type,GL_FALSE, 0,0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb,
		const VertexBufferLayout& layout) {

	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i=0;i<elements.size();i++){
		const auto& element=elements[i];
		GLCall(glEnableVertexAttribArray(i));
	
		GLCall(glVertexAttribPointer(i,element.count,element.type,element.normalized, layout.GetStride()-12, (const void *)offset));

		offset+=element.count*VertexBufferElement::GetSizeOfType(element.type);
	}

}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}

