/*
 * Renderer.hpp
 *
 *  Created on: 13 de dez de 2017
 *      Author: Casa
 */

#ifndef SRC_RENDERER_HPP_
#define SRC_RENDERER_HPP_

#include <GL/glew.h>
#include <csignal>
#include <string>

using std::string;

#define ASSERT(x) if (!(x)) raise(SIGABRT);  // its the most similar thing to __debugbreak() I could find
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))



void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

//void Shader::CheckShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage)
//{
//    GLint success = 0;
//    GLchar error[1024] = { 0 };
//
//    if(isProgram)
//        glGetProgramiv(shader, flag, &success);
//    else
//        glGetShaderiv(shader, flag, &success);
//
//    if(success == GL_FALSE)
//    {
//        if(isProgram)
//            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
//        else
//            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
//
//        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
//    }
//}

#endif /* SRC_RENDERER_HPP_ */
