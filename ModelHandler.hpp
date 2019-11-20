#ifndef MODEL_HANDLER
#define MODEL_HANDLER
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "ModelData.hpp"
#include "Shader.hpp"

class ModelHandler
{
    private:
        Shader *shaderRoot;
        Shader *shaderEyeR;
        Shader *shaderLeg1;

        GLuint shaderProgramID[3];
        GLuint VAOs[3];

        GLfloat EyeScale=3;
        GLfloat LegScale=3;

        GLfloat Eye_x=1.0f;
        GLfloat Eye_y=0.75f;
        GLfloat Eye_z=4.0f;

        GLfloat UpperLeg_x=1.5f;
        GLfloat UpperLeg_y=0.8f;
        GLfloat UpperLeg_z=3.75f;

        GLfloat LowerLeg_x=9.5f;
        GLfloat LowerLeg_y=0.0f;
        GLfloat LowerLeg_z=0.0f;

        GLfloat Angle=0.082;

    public:
        bool Change=0;
        GLfloat temp;

        ModelData *Root;
        ModelData *EyeR;
        ModelData *Leg1;
    
        ModelHandler(const char* root="box.3ds", const char* eye="eyeball.dae", const char* leg="bat.obj");
        void draw(glm::mat4 view, glm::mat4 persp_proj, glm::mat4 Color_view);
};
#endif