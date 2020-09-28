#pragma once

#include "Shader.h"
#include<map>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static Shader LoadShader(const char * vShaderFile, const char * fShaderFile, const char * gShaderFile, std::string name);
	static Shader loadShaderFromFile(const char * vShaderFile, const char * fShaderFile, const char * gShaderFile);

private:
	static std::map<std::string, Shader> Shaders;
};

