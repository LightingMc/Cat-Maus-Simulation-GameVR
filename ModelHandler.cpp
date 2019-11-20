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
#include "ModelData.hpp"
#include "ModelHandler.hpp"
#include "Shader.hpp"

using namespace std;

ModelHandler::ModelHandler(const char* root, const char* eye, const char* leg)
{
    glGenVertexArrays(3,VAOs);

	shaderRoot = new Shader();
	shaderProgramID[0]=shaderRoot->ShaderProgramID;
	Root=new ModelData(root,shaderProgramID[0],VAOs[0]);
	Root->Position=glm::vec3(0.0f, 0.0f, -100.0f);

	shaderEyeR = new Shader();
	shaderProgramID[1]=shaderEyeR->ShaderProgramID;
	EyeR=new ModelData(eye,shaderProgramID[1],VAOs[1],1);
	EyeR->scaleDown(EyeScale);
	EyeR->Position=glm::vec3(Eye_x,Eye_y,Eye_z);

	shaderLeg1 = new Shader();
	shaderProgramID[2]=shaderLeg1->ShaderProgramID;
	Leg1=new ModelData(leg,shaderProgramID[2],VAOs[2],1);
}

void ModelHandler::draw(glm::mat4 view, glm::mat4 persp_proj, glm::mat4 Color_view)
{
	glm::mat4 ROOT=Root->draw(view,persp_proj,Color_view);
	EyeR->Position=glm::vec3(Eye_x,Eye_y,Eye_z);
	EyeR->draw(view,persp_proj,Color_view,ROOT);
	EyeR->Position=glm::vec3(-Eye_x,Eye_y,Eye_z);
	EyeR->draw(view,persp_proj,Color_view,ROOT);

	Leg1->rotate_z+=Angle;
	Leg1->Position=glm::vec3(UpperLeg_x,-UpperLeg_y,UpperLeg_z);
	Leg1->scaleDown(LegScale);
	glm::mat4 LEG1=Leg1->draw(view,persp_proj,Color_view,ROOT);
	Leg1->Position=glm::vec3(-UpperLeg_x,-UpperLeg_y,UpperLeg_z);
	glm::mat4 LEG2=Leg1->draw(view,persp_proj,Color_view,ROOT);
	Leg1->Position=glm::vec3(-UpperLeg_x, -UpperLeg_y, -UpperLeg_z);
	glm::mat4 LEG3=Leg1->draw(view,persp_proj,Color_view,ROOT);
	Leg1->Position=glm::vec3(UpperLeg_x, -UpperLeg_y, -UpperLeg_z);
	glm::mat4 LEG4=Leg1->draw(view,persp_proj,Color_view,ROOT);
	Leg1->rotate_z-=Angle;
	Leg1->scaleDown(1/LegScale);

	if(Change)
	{
		temp=Leg1->rotate_x;
		Leg1->rotate_x=0;
	}

	Leg1->rotate_y+=Angle;
	Leg1->Position=glm::vec3(LowerLeg_x,LowerLeg_y,LowerLeg_z);	
	Leg1->draw(view,persp_proj,Color_view,LEG1);
	Leg1->draw(view,persp_proj,Color_view,LEG2);
	Leg1->rotate_y-=2*Angle;
	Leg1->draw(view,persp_proj,Color_view,LEG3);
	Leg1->draw(view,persp_proj,Color_view,LEG4);
	Leg1->rotate_y+=Angle;

	if(Change)
	{
		Leg1->rotate_x=temp;
		Change=0;
	}
}