#define GLEW_STATIC
#include "shader.h"
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include "Log.hpp"

static void printMat(glm::mat4 phi)
{
	printf(" matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", phi[j][i]);
			printf("\n");
		}
}

Shader::Shader(const std::string& fileName)
{
	
	GLCall(m_program = glCreateProgram());
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for(unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);
	
	GLCall(glBindAttribLocation(m_program, POSITION_VB, "position"));
	GLCall(glBindAttribLocation(m_program, TEXCOORD_VB, "texCoords"));
	GLCall(glBindAttribLocation(m_program, NORMAL_VB, "normal"));
	GLCall(glBindAttribLocation(m_program, COLOR_VB, "color"));
	GLCall(glBindAttribLocation(m_program, WEIGHT_VB, "weights"));
	GLCall(glBindAttribLocation(m_program, JOINT_INDEX_VB, "jointIndices"));

	GLCall(glLinkProgram(m_program));
	
	GLCall(glValidateProgram(m_program));	
}

Shader::~Shader()
{
	for(unsigned int i = 0; i < NUM_SHADERS; i++)
    {
        GLCall(glDetachShader(m_program, m_shaders[i]));
		GLCall(glDeleteShader(m_shaders[i]));
    }

	GLCall(glDeleteProgram(m_program));
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_program));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}


unsigned int Shader::CreateShader(const std::string& text, unsigned int type)
{
    GLuint shader ;
	 GLCall(shader = glCreateShader(type));

    if(shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = text.length();

    GLCall(glShaderSource(shader, 1, p, lengths));
    GLCall(glCompileShader(shader));

    return shader;
}


void Shader::SetUniform1i(const std::string& name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name),value));
}

void Shader::SetUniform4i(const std::string& name,  int vi0,int vi1,int vi2,int vi3) {
	GLCall(glUniform4i(GetUniformLocation(name),vi0,vi1,vi2,vi3));
}

void Shader::SetUniform1f(const std::string& name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name),value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name),v0,v1,v2,v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4&  matrix) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4fv(const std::string& name,const glm::mat4 *matrices,const int length)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), length, GL_FALSE, &matrices[0][0][0]));
}
void Shader::SetUniform4fv(const std::string& name, float value[20])
{
	GLCall(glUniform4fv(GetUniformLocation(name), 1, &value[0]));
}

int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name)!= m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_program, name.c_str()));
	if (location == -1 )
		std::cout<< "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	m_UniformLocationCache[name]= location;
	return location;
}
