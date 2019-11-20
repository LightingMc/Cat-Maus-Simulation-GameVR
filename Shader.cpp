#include "Shader.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include <fstream>
#include <sstream>
#include <sys/time.h>
// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 


using namespace std;

Shader::Shader(const char* vertex_shader, const char* Fragment_shader)
{
	this->ShaderProgramID = glCreateProgram();
	AddShader("simpleVertexShader.txt", GL_VERTEX_SHADER);
	AddShader("simpleFragmentShader.txt", GL_FRAGMENT_SHADER);
	glLinkProgram(this->ShaderProgramID);
}

void Shader::AddShader(const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);
	string vertexCode;
	ifstream vShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
	// open files
	vShaderFile.open(pShaderText);
	stringstream vShaderStream;
	// read file's buffer contents into streams
	vShaderStream << vShaderFile.rdbuf();
	// close file handlers
	vShaderFile.close();
	// convert stream into string
	vertexCode   = vShaderStream.str();
	const char* cstr=vertexCode.c_str();


//	Could this 1 in the shader source be the issues?
	glShaderSource(ShaderObj, 1, (const GLchar**)&cstr, NULL);
	glCompileShader(ShaderObj);
	glAttachShader(ShaderProgramID, ShaderObj);
}