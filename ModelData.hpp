#ifndef MODEL_DATA
#define MODEL_DATA

//#define GLM_ENABLE_EXPERIMENTAL

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
//#include <glm/gtx/string_cast.hpp>
#include "Shader.hpp"
class ModelData
{
    public:
        ModelData(const char* file_name="eyeball.dae", GLuint shaderProgramID=0,GLuint Vao=0,bool root=0);

        GLuint VAO;
        GLuint ShaderID;

        long long mPointCount;

        GLfloat rotate_x = 0.0f;
        GLfloat rotate_y = 0.0f;
        GLfloat rotate_z = 0.0f;
 
        GLfloat scale_x = 1.0f;
        GLfloat scale_y = 1.0f;
        GLfloat scale_z = 1.0f;

        glm::vec3 Position=glm::vec3(0.0f, 0.0f, -200.0f);
        glm::mat4 draw(glm::mat4 view,glm::mat4 persp_proj,glm::mat4 Color_view,glm::mat4 Parent=glm::mat4(1.0f));
        glm::mat4 getModel();
        void scaleDown(GLfloat factor);
        void scaleUP(GLfloat factor);
        void UpDateLoc(int Index,float Increment);
        void IncrementLoc(int Index,float Increment=1.5f);
        void Correction(int Index,float Increment);
        bool isRoot;
};
#endif