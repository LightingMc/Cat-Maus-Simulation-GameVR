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
#include <opencv2/opencv.hpp>
#include "Simulation.hpp"
#include "ModelData.hpp"
#include "ModelHandler.hpp"
#include "Shader.hpp"

#define floor "GrassPatch.obj"

using namespace cv;
using namespace std;

VideoCapture cap;
Mat frame;
Scalar m;

bool firstMouse=1;

Simulation *Hunt;
Shader *shaderFloor;
ModelData *Floor;
GLuint Vao_floor;

int width = 800;
int height = 600;

GLfloat lastX = 400, lastY = 300, xoffset, yoffset , yaw=-90, pitch=0;

int Thumb_x=0;
int Thumb_y=0;
bool Thumb=0;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  100.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

void display()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace( GL_BACK );		 // cull back face
	glFrontFace( GL_CCW );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 persp_proj = glm::perspective(glm::radians(40.0f), (float) width / (float) height, 10.0f, 1000.0f);	
	
	//down below calculate the view_mat for frag?????? 
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos[0],-cameraPos[1],-cameraPos[2]));
	glm::mat4 R(1.0f);
	glm::rotate(R, glm::degrees(-yaw ) , glm::vec3(0,1,0));
	glm::mat4 Color_view = R * T;
	Floor->draw(view,persp_proj,Color_view);
	Hunt->draw(view,persp_proj,Color_view);
	glutSwapBuffers();
}

void init()
{
	glGenVertexArrays(1,&Vao_floor);
	shaderFloor = new Shader();
	Floor=new ModelData(floor,shaderFloor->ShaderProgramID,Vao_floor,0);
	Floor->Position=glm::vec3(0.0f, -60.0f, -250.0f);
	//Floor->scaleDown(4);
	Floor->rotate_x+=0.082;

	Hunt=new Simulation();
}

void processSpecialKeys(int key, int x, int y) 
{
	float cameraSpeed = 0.5f; // adjust accordingly
	switch(key) {
		case GLUT_KEY_UP:
	        cameraPos += cameraSpeed * cameraFront;break;
		case GLUT_KEY_DOWN:
	        cameraPos -= cameraSpeed * cameraFront;break;
		case GLUT_KEY_LEFT:
	        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;break;
		case GLUT_KEY_RIGHT:
		    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;break;
	}
	glutPostRedisplay();
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) 
{
	if(Hunt->Game)
	{
		if 		(key == 'd') {Hunt->Player->Root->IncrementLoc(0);}
		else if (key == 'a') {Hunt->Player->Root->IncrementLoc(0,-1.5f);}
		else if (key == 'w') {Hunt->Player->Root->IncrementLoc(1);}
		else if (key == 's') {Hunt->Player->Root->IncrementLoc(1,-1.5f);}
		else if (key == 'r') {Hunt->Player->Root->IncrementLoc(2,+2.0f);}
		else if (key == 'f') {Hunt->Player->Root->IncrementLoc(2,-2.0f);}
		else if (key == 'q') {Hunt->Player->Root->scale_x*=2;Hunt->Player->Root->scale_y*=2;Hunt->Player->Root->scale_z*=2;}
		else if (key == 'e') {Hunt->Player->Root->scale_x/=2;Hunt->Player->Root->scale_y/=2;Hunt->Player->Root->scale_z/=2;}
		else if (key == '8') {Hunt->Player->Leg1->rotate_x+=0.005;Hunt->Player->Change=1;}
		else if (key == '5') {Hunt->Player->Leg1->rotate_x-=0.005;Hunt->Player->Change=1;}
		else if (key == '4') {Hunt->Player->Leg1->rotate_y+=0.005;Hunt->Player->Change=1;}
		else if (key == '6') {Hunt->Player->Leg1->rotate_y-=0.005;Hunt->Player->Change=1;}
		else if (key == '7') {Hunt->Player->Leg1->rotate_z+=0.005;Hunt->Player->Change=1;}
		else if (key == '9') {Hunt->Player->Leg1->rotate_z-=0.005;Hunt->Player->Change=1;}
		else if (key == 'g') {Hunt->Player->Root->scale_x*=2;}
		else if (key == 'j') {Hunt->Player->Root->scale_x/=2;}
		else if (key == 'y') {Hunt->Player->Root->scale_y*=2;}
		else if (key == 'h') {Hunt->Player->Root->scale_y/=2;}
	}

	if (key == 'm') {Hunt->SwitchMode();}
	if (key == 'n') {Hunt->Intelligence=!Hunt->Intelligence;}
	glutPostRedisplay();
}


void onMousarClick(int xpos, int ypos)
{
	if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	glutPostRedisplay();
}

void MouseClick_noMove(int xpos, int ypos)//int button, int state, int xpos, int ypos)
{
	lastX = xpos;
	lastY = ypos;
}

void drawer(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	glutDisplayFunc(display);
	glutKeyboardFunc(keypress);
	glutMotionFunc(onMousarClick);
	glutPassiveMotionFunc(MouseClick_noMove);
	glutSpecialFunc(processSpecialKeys);

	glewInit();
	init();

	while(1)
	{
		if(Hunt->Game)	Hunt->GameStep();
		else Hunt->SimulationStep();
		// Do some image processing on "frame"
		glutMainLoopEvent();        // One iteration only
		display();      // Call the func used with glutDisplayFunc()
		if(waitKey(30) == 27)
			break;
	}
	glutMainLoop();
}

int main(int argc, char** argv) 
{
	drawer(argc, argv);
	return 0;
}