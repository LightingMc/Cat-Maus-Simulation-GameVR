#include "ModelData.hpp"
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
#include "Shader.hpp"

using namespace std;

ModelData::ModelData(const char* file_name,GLuint shaderProgramID, GLuint Vao, bool root)
{
	GLuint x=0;
	if(shaderProgramID!=x&&Vao!=x)
	{
		this->ShaderID=shaderProgramID;
		this->VAO=Vao;
	}
	else
	{
		//Maybe not the best stratergy for the shaders???
		glGenVertexArrays(1,&VAO);
		Shader* temp=new Shader();
		this->ShaderID = temp->ShaderProgramID;
	}

	this->isRoot=root;
	glUseProgram(this->ShaderID);

	const aiScene *scene = aiImportFile( file_name, aiProcess_Triangulate | aiProcess_PreTransformVertices);
	const aiMesh *mesh = scene->mMeshes[0];
	this->mPointCount = mesh->mNumVertices;
	glBindVertexArray(this->VAO);
	GLfloat *points = NULL;		 // array of vertex points
	GLfloat *normals = NULL;	 // array of vertex normals
	GLfloat *texcoords = NULL; // array of texture coordinates

	//A lot of help was taken from here https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/13_mesh_import/main.cpp
	if ( mesh->HasPositions() ) {
		points = (GLfloat *)malloc( this->mPointCount * 3 * sizeof( GLfloat ) );
		for ( int i = 0; i < this->mPointCount; i++ ) {
			const aiVector3D *vp = &( mesh->mVertices[i] );
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;
		}
	}
	if ( mesh->HasNormals() ) {
		normals = (GLfloat *)malloc( this->mPointCount * 3 * sizeof( GLfloat ) );
		for ( int i = 0; i < this->mPointCount; i++ ) {
			const aiVector3D *vn = &( mesh->mNormals[i] );
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if ( mesh->HasTextureCoords( 0 ) ) {
		texcoords = (GLfloat *)malloc( this->mPointCount * 2 * sizeof( GLfloat ) );
		for ( int i = 0; i < this->mPointCount; i++ ) {
			const aiVector3D *vt = &( mesh->mTextureCoords[0][i] );
			texcoords[i * 2] = (GLfloat)vt->x;
			texcoords[i * 2 + 1] = (GLfloat)vt->y;
		}
	}

	/* copy mesh data into VBOs */
	if ( mesh->HasPositions() ) {
		GLuint vbo;
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, 3 * this->mPointCount * sizeof( GLfloat ), points,
									GL_STATIC_DRAW );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 0 );
		free( points );
	}
	if ( mesh->HasNormals() ) {
		GLuint vbo;
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, 3 * this->mPointCount * sizeof( GLfloat ), normals,
									GL_STATIC_DRAW );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 1 );
		free( normals );
	}
	if ( mesh->HasTextureCoords( 0 ) ) {
		GLuint vbo;
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, 2 * this->mPointCount * sizeof( GLfloat ), texcoords,
									GL_STATIC_DRAW );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 2 );
		free( texcoords );
	}

	aiReleaseImport( scene );
}

glm::mat4 ModelData::draw(glm::mat4 view, glm::mat4 persp_proj,glm::mat4 Color_view, glm::mat4 Parent)
{
	glm::mat4 model=this->getModel();
	// Apply the root matrix to the child matrix
	if (this->isRoot) model = Parent * model;

	glUseProgram(this->ShaderID);
	glBindVertexArray(this->VAO);

	int proj_mat_location = glGetUniformLocation(this->ShaderID, "proj");
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, glm::value_ptr(persp_proj));
	int matrix_location = glGetUniformLocation(this->ShaderID, "model");
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(model));
 	int view_mat_location = glGetUniformLocation(this->ShaderID, "view");
 	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view));
	int Col_location=glGetUniformLocation(this->ShaderID, "Col_mat");
 	glUniformMatrix4fv(Col_location, 1, GL_FALSE, glm::value_ptr(Color_view));
	
	glDrawArrays(GL_TRIANGLES, 0, this->mPointCount);

	return model;
}

glm::mat4 ModelData::getModel()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, this->Position);
	model = glm::rotate(model, glm::degrees(rotate_x) , glm::vec3(1,0,0));
	model = glm::rotate(model, glm::degrees(rotate_y) , glm::vec3(0,1,0));
	model = glm::rotate(model, glm::degrees(rotate_z) , glm::vec3(0,0,1));
	model = glm::scale(model, glm::vec3(scale_x,scale_y, scale_z));

	return model;
}


void ModelData::scaleDown(GLfloat factor)
{
	this->scale_x/=factor;
	this->scale_y/=factor;
	this->scale_z/=factor;
}


void ModelData::scaleUP(GLfloat factor)
{
	this->scale_x*=factor;
	this->scale_y*=factor;
	this->scale_z*=factor;
}

void ModelData::IncrementLoc(int Index,float Increment)
{
	this->Position[Index]+=Increment;
	Correction(Index,Increment);
}


void ModelData::UpDateLoc(int Index,float Increment)
{
	this->Position[Index]=Increment;
	Correction(Index,Increment);
}


void ModelData::Correction(int Index,float Increment)
{
	if(Index==0)
	{
		if(this->Position[Index]>70)
			this->Position[Index]=70;


		if(this->Position[Index]<-70)
			this->Position[Index]=-70;
	}

	else if(Index==1)
	{
		if(this->Position[Index]>48)
			this->Position[Index]=48;


		if(this->Position[Index]<-10)
			this->Position[Index]=-10;
	}

	else if(Index==2)
	{
		if(this->Position[Index]>-100)
			this->Position[Index]=-100;

		if(this->Position[Index]<-380)
			this->Position[Index]=-380;
	}
}