#ifndef SIMULATION
#define SIMULATION
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
#include "ModelHandler.hpp"
#include "Shader.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Simulation
{
    public:
        bool Game;//1 for game, 0 for simulation
        bool Intelligence=0;//0 for random, 1 for greedy
        VideoCapture cap;
        Mat frame;
        Scalar m;
        
        ModelHandler *Player;
        ModelHandler *Opponent;
        int player_score=0;
        int opp_score=0;

        std::vector<ModelHandler*> Cats;
        std::vector<ModelData*> Mice;
        Simulation(int Cats=5,int Mice=15);
        //   |
        //   |  Contains
        //   V
        void AssignPosition(ModelData* Model, bool start=0,bool type=0);//0 for maus, 1 for Cats
        //   |                                                          //Not one for ModelHandler caz only the root needs to be assigned???
        //   |  Contains
        //   V
        GLint DistanceCalculator(glm::vec3 Position1,glm::vec3 Position2);
        void SimulationStep(int Steps=3);

        void SwitchMode();

        void GetColors();
        void FilterForColors();
        void Greedy_action(ModelData* Cat);
        void GameStep();
        
        void draw(glm::mat4 view, glm::mat4 persp_proj, glm::mat4 Color_view);
        int eat(ModelData* Cat,int pos=-1);
        void death(bool type=0,int pos=0);//0 for maus, 1 for Cats
};
#endif