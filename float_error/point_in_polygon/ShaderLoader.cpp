#include "ShaderLoader.h"

#include <iostream>

void ShaderLoader::compileShader(shader_type shader, const char* source)
{
    GLuint _shader = glCreateShader(shader);
    glShaderSource(_shader, 1, &source, NULL);
    glCompileShader(_shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
    
    if (success)
        shaders.push_back(_shader);
    else 
        glGetShaderInfoLog(_shader, 512, NULL, infoLog);
}

GLuint ShaderLoader::linkShader()
{
    GLuint shaderProgram = glCreateProgram();
    for (const auto& shader : shaders)
        glAttachShader(shaderProgram, shader);
    glLinkProgram(shaderProgram);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        return GLuint(-1);
    }
    return shaderProgram;
}
void ShaderLoader::useProgram(GLuint shaderProgram)
{
    glUseProgram(shaderProgram);

    for (auto& shader : shaders)
        glDeleteShader(shader);
    shaders.clear();
}
