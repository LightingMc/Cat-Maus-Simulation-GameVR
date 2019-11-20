#ifndef Sha_der
#define Sha_der

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

class Shader
{
    private:
        void AddShader(const char* pShaderText, GLenum ShaderType);
    public:
        GLuint ShaderProgramID;
        Shader(const char* vertex_shader="simpleVertexShader.txt",const char* Fragment_shader="simpleFragmentShader.txt");
};
#endif