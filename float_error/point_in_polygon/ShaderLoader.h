#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>



class ShaderLoader
{
	using shader_type = GLint;

private:
	std::vector<GLint> shaders;

public:
	void compileShader(shader_type, const char* source);
	GLuint linkShader();
	void useProgram(GLuint shaderProgram);
};

