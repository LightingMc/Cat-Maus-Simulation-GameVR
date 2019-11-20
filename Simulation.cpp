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
#include "ModelData.hpp"
#include "ModelHandler.hpp"
#include "Shader.hpp"
#include "Simulation.hpp"
#include <random>

using namespace std;
using namespace cv;

Simulation::Simulation(int Cat,int Mauser)
{
    for(int i=0;i<Cat;i++)
        this->Cats.push_back(new ModelHandler());
    for(int i=0;i<Mauser;i++)
        this->Mice.push_back(new ModelData());

    for(uint i=0;i<Cats.size();i++)
        AssignPosition(this->Cats[i]->Root);
    for(uint i=0;i<Mice.size();i++)
        AssignPosition(this->Mice[i]);
    
    Player=new ModelHandler();
    Opponent=new ModelHandler();
    Game=0;
}

void Simulation::AssignPosition(ModelData* Model, bool start,bool type)
{
    if(!Cats.empty() && !Mice.empty())
    {
        while(true)
        {
            bool Ass=1;
            glm::vec3 Position1=glm::vec3(rand() % 140 - 70,rand() % 58 - 10,rand() % 280 - 380);
            if(start) Position1=glm::vec3(rand() % 5 - 2,rand() % 5 - 2,rand() % 5 - 2 );
            glm::vec3 Position2=glm::vec3(0.0f,0.0f,0.0f);

            uint Total_Iterations=Cats.size()+this->Mice.size();
            if(start)
            {
                if (type) Total_Iterations=Cats.size();
                else Total_Iterations=Mice.size();
            }
            for(uint i=0;i<Total_Iterations;i++)
            {
                if(start)
                {

                    if (type) Position2=Cats[i]->Root->Position;
                    else Position2=Mice[i]->Position;
                }
                else
                {
                    if(i<this->Cats.size()) Position2=Cats[i]->Root->Position;
                    else Position2=Mice[i-Cats.size()]->Position;
                }
                //Here the logic for start and mouse eating takes place
                if(DistanceCalculator(Position1,Position2)<25)
                {
                    Ass=0;
                }
            }
            if(Ass)//assinging position is different from making them move...
            {
                if(start)
                {
                    Model->IncrementLoc(0,Position1[0]);
                    Model->IncrementLoc(1,Position1[1]);
                    Model->IncrementLoc(2,Position1[2]);
                }

                else
                {
                    Model->UpDateLoc(0,Position1[0]);
                    Model->UpDateLoc(1,Position1[1]);
                    Model->UpDateLoc(2,Position1[2]);
                }
                break;
            }
        }
    }
}


void Simulation::SwitchMode()
{
    Game=!Game;
    if(Game)
    {
        cap.open(0);
        this->GetColors();
    }
    else
    {
        cap.release();
        destroyAllWindows();
    }
}


GLint Simulation::DistanceCalculator(glm::vec3 Position1,glm::vec3 Position2)
{
    GLint x=pow(Position1[0]-Position2[0],2);
    GLint y=pow(Position1[1]-Position2[1],2);
    GLint z=pow(Position1[2]-Position2[2],2);
    return sqrt(x+y+z);
}


void Simulation::draw(glm::mat4 view, glm::mat4 persp_proj, glm::mat4 Color_view)
{
    if(Game)
    {
        this->Player->draw(view,persp_proj,Color_view);
        this->Opponent->draw(view,persp_proj,Color_view);
    }
    else
    {    //Need to change this depending on mode
    for(uint i=0;i<Cats.size();i++)
        this->Cats[i]->draw(view,persp_proj,Color_view);
    }

    //Do I need to reassign the mice?? Probably not?
    for(uint i=0;i<Mice.size();i++)
        this->Mice[i]->draw(view,persp_proj,Color_view);
}

int Simulation::eat(ModelData* Cat, int pos)
{
    int score=0;
    for(uint i=0;i<Mice.size();i++)
    {
        if(!Mice.empty() && DistanceCalculator(Mice[i]->Position,Cat->Position)<20)
        {
            if(!Game)cout<<"Maus Eaten"<<endl;;
            death(0,i);
            score++;
            if(!Game)
            {
                Cat->scaleUP(2);
                if(Cat->scale_x>6)
                {
                    death(1,pos);
                    cout<<"Cat died"<<endl;
                    return -1;
                }
                i--;
            }
        }
    }
    return score;
}

void Simulation::death(bool type,int pos)//0 for maus, 1 for Cats
{
    if(type)//Dont need to worry about this being called on the actual thing
    {
        Cats.erase(Cats.begin()+pos);
        for(int i=0;i<3;i++)
            this->Mice.push_back(new ModelData());
        for(uint i=Mice.size()-3;i<Mice.size();i++)
            AssignPosition(this->Mice[i]);

        if(Cats.empty())
        {
            cout<<"New Cats being added"<<endl;
            for(int i=0;i<5;i++)
                this->Cats.push_back(new ModelHandler());
            for(uint i=Cats.size()-5;i<Cats.size();i++)
                AssignPosition(this->Cats[i]->Root);
        }
    }
    else Mice.erase(Mice.begin()+pos);
    if(Game)
    {
        cout<<"Player Score "<<player_score<<" opp_score "<<opp_score<<endl;
    }
    if(Mice.empty())
    {
        if(player_score>opp_score) cout<<"Congrats...You Won :)"<<endl;
        else cout<<"A random number generator machine beat you"<<endl;
        for(int i=0;i<15;i++)
            this->Mice.push_back(new ModelData());
        for(uint i=0;i<Mice.size();i++)
            AssignPosition(this->Mice[i]);
        this->SwitchMode();
    }
}


void Simulation::GetColors()
{
	while(1)
	{
		cap.read(frame);
		cvtColor(frame, frame,COLOR_BGR2HSV);

		Point pt1=Point(frame.size().width/2-25,frame.size().height/5-25); 
		Point pt2=Point(frame.size().width/2+25,frame.size().height/5+25);

		Mat fmare=frame.clone();
		Rect R(pt1,pt2); //Create a rect 
		Mat ROI = fmare(R); //Crop the region of interest using above rect
		
		rectangle(frame, pt1, pt2, Scalar(150,100,100), 2);

		m = cv::mean(ROI);
	
		imshow("frame",frame);
		imshow("ROI",ROI);

		if(waitKey(30) == 27)
			break;
	}
	destroyAllWindows();
}

void Simulation::FilterForColors()
{
	Scalar lower_Limit(m[0]-50,m[1]-50,m[2]-50);
	Scalar upper_limit(m[0]+50,m[1]+50,m[2]+50);

	cap.read(frame);
	Mat org=frame.clone();
	cvtColor(frame, frame,COLOR_BGR2HSV);
	inRange(frame, lower_Limit, upper_limit, frame);

	GaussianBlur(frame,frame, Size(9,9),0,0);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(frame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//find the highest area contour and draw that
	if(contours.size()>0)
	{
		int Highest_area=contourArea(contours[0]);
		int highest_index=0;
		for(unsigned int i=0;i<contours.size();i++)
		{	
			if (contourArea(contours[i])>Highest_area)
			{
				Highest_area=contourArea(contours[i]);
				highest_index=i;
			}
		}
		if(Highest_area>50)
		{
            drawContours(org, contours, highest_index,  Scalar(255, 255,255));
            Moments mu=moments(contours[highest_index],1);
            int cx = mu.m10 / mu.m00;
            int cy = mu.m01 / mu.m00;
            circle(org, Point(cx,cy),5,Scalar(255,255,255));
            imshow("MyVideo",org);
            int Thumb_x=org.size().height/2-cy;
            int Thumb_y=org.size().width/2-cx;;
            Player->Root->Position[0]=Thumb_y/5;
            Player->Root->Position[1]=Thumb_x/5;
		}
	}
}

void Simulation::SimulationStep(int Steps)
{
    for(int j=0;j<Steps;j++)
    {
        for(uint i=0;i<Mice.size();i++)
            AssignPosition(this->Mice[i],1);
        for(uint i=0;i<Cats.size();i++)
            {   if(Intelligence) Greedy_action(this->Cats[i]->Root);
                else AssignPosition(this->Cats[i]->Root,1,1);
                int x=eat(this->Cats[i]->Root,i);
                if(x==-1)
                {
                    i--;
                }
            }
    }
}

void Simulation::GameStep()
{
    for(uint i=0;i<Mice.size();i++)
            AssignPosition(this->Mice[i],1);
    FilterForColors();
    if (Intelligence) Greedy_action(this->Opponent->Root);
    else
    {
        glm::vec3 Position=glm::vec3(rand() % 5 - 2,rand() % 5 - 2,rand() % 5 - 2 );   
        this->Opponent->Root->IncrementLoc(0,Position[0]);
        this->Opponent->Root->IncrementLoc(1,Position[1]);
        this->Opponent->Root->IncrementLoc(2,Position[2]);
    }

    player_score+=eat(this->Player->Root);
    opp_score+=eat(this->Opponent->Root);
}


void Simulation::Greedy_action(ModelData* Cat)
{
    glm::vec3 x=glm::vec3(0.0f,0.0f,0.0f);
    GLint Min_Distance=0;
    int Index=0;
    for(uint i=0;i<Mice.size();i++)
    {
        if(DistanceCalculator(Mice[i]->Position,Cat->Position)<Min_Distance)
        {
            Min_Distance=DistanceCalculator(Mice[i]->Position,Cat->Position)>Min_Distance;
            Index=i;
        }
    }

    x=Mice[Index]->Position-Cat->Position;
    GLint max=x[0];
    if(abs(x[1])>abs(max)) max=x[1];
    if(abs(x[2])>abs(max)) max=x[2];
    Cat->IncrementLoc(0,x[0]/abs(max));
    Cat->IncrementLoc(1,x[1]/abs(max));
    Cat->IncrementLoc(2,x[2]/abs(max));  
}