
#include <GL/glew.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>


#include <iostream>

#include "mesh.h"

#include "skeleton.h"
#include "shaderhandler.h"
#include "lighthandler.h"
#include "texturehandler.h"

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"



Mesh::Mesh(){
	glGenVertexArrays(1, &VertexArrayID);
	TextureDiff=0;
	TextureSpec=0;
	ModelMatrix = glm::mat4(1.0);
	isRoot=false;
	lightCount=0;
}

Mesh::~Mesh(){

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureDiff);
	glDeleteTextures(1, &TextureSpec);
	glDeleteVertexArrays(1, &VertexArrayID);
	delete skel;


}

void Mesh::setShaders(std::string vfile,std::string ffile){

	if(isRoot){

		for(Mesh* m : vmesh){
			m->setShaders(vfile, ffile);
		}
		return;
	}
	programID = ShaderHandler::getInstance()->getShader( vfile.c_str(), ffile.c_str());

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	TextureID  = glGetUniformLocation(programID, "rgbTextureSampler");
	TextureSpecID  = glGetUniformLocation(programID, "specTextureSampler");
	loadLights();

}

void Mesh::loadTextureDiff(std::string filename, bool propagate){

	if(isRoot || propagate){
		for(Mesh* m : vmesh){
			m->loadTextureDiff(filename,true);
		}
		if(isRoot)
			return;
	}

	TextureDiff = TextureHandler::getInstance()->getTextureID(filename);

	
}


void Mesh::loadTextureSpec(std::string filename, bool propagate){

	if(isRoot || propagate){
		for(Mesh* m : vmesh){
			m->loadTextureSpec(filename,true);
		}
		if(isRoot)
			return;
	}

	TextureSpec = TextureHandler::getInstance()->getTextureID(filename);

}


void Mesh::loadLights(){

	lightBindingPoint =0;

	LightsBlockID = glGetUniformBlockIndex(programID, "LightBlock");
	LightCountID = glGetUniformLocation(programID, "LightCount");
	glUniformBlockBinding(programID,LightsBlockID,lightBindingPoint);

	lightCount = LightHandler::getInstance()->vLights.size();
	lightbuffer = LightHandler::getInstance()->getLightBuffer();
	glBindBuffer(GL_UNIFORM_BUFFER, lightbuffer);


}

void Mesh::loadMesh(){

	
	skel = new Skeleton();

	glBindVertexArray(VertexArrayID);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

}


void Mesh::draw(glm::mat4 modelmat){

	//skel->ModelMatrix = glm::translate(ModelMatrix,glm::vec3(0,0,1));
	//skel->draw();
	//std::cout <<"\n\n" << name << "\n" << glm::to_string(ModelMatrix) << std::endl;
	glm::mat4 Model = ModelMatrix;
	Model = modelmat*Model;
	if(isRoot){

		for(Mesh* m : vmesh){
			m->draw(Model);
		}
		return;
	}

	glUseProgram(programID);
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();

	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * Model;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);


	glBindBufferBase(GL_UNIFORM_BUFFER,lightBindingPoint,lightbuffer);

	glUniform1i(LightCountID, lightCount);

		// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureDiff);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);


		// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureSpec);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureSpecID, 1);

		// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


	for(Mesh* m : vmesh){
		m->draw(ModelMatrix);
	}




}