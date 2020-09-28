#pragma once
#include <string>
#include "GL/glew.h"

class Shader
{
public:
	// state
	unsigned int ID;

	Shader();
	GLuint Use();
	void Compile(const char * vertexShader, const char *fragmentShader, const char *geometryShader);
	GLuint CreateShader(GLenum eShaderType, const std::string & strShaderFile);
	~Shader();

private:
	// checks if compilation or linking failed and if so, print the error logs
	void    checkCompileErrors(unsigned int object, std::string type);
};

