
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
#include "common/tangentspace.hpp"



Mesh::Mesh(std::string modelPath){
	texturePath = modelPath;
	glGenVertexArrays(1, &VertexArrayID);
	TextureDiff=0;
	TextureSpec=0;
	TextureBump=0;
	TextureNorm=0;
	TextureEmissive=0;
	ModelMatrix = glm::mat4(1.0);
	isRoot=false;
	lightCount=0;
	format = GL_RGB;
}

Mesh::~Mesh(){

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureDiff);
	glDeleteTextures(1, &TextureSpec);
	glDeleteTextures(1, &TextureBump);
	glDeleteTextures(1, &TextureNorm);
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
	std::cout << "PROGRAMID :: " << programID << std::endl;

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	TextureID  = glGetUniformLocation(programID, "rgbTextureSampler");
	TextureSpecID  = glGetUniformLocation(programID, "specTextureSampler");
	TextureBumpID  = glGetUniformLocation(programID, "bumpTextureSampler");
	TextureNormID  = glGetUniformLocation(programID, "normTextureSampler");
	TextureEmissiveID  = glGetUniformLocation(programID, "emissiveTextureSampler");
	TexelOffsetID  = glGetUniformLocation(programID, "TexelOffset");
	loadLights();

}

void Mesh::autoShaders(){

	if(isRoot){

		for(Mesh* m : vmesh){
			m->autoShaders();
		}
		return;
	}
	std::cout << "AUTO CHOOSE SHADERS, DIFF"<< TextureDiff << " BUMP" << TextureBump 
	<< " NORM" << TextureNorm << " EMISSIVE " << TextureEmissive << " SPEC " << TextureSpec  << std::endl;
	if(TextureDiff==0){
		setShaders("shaders/StandardShading.vertexshader", "shaders/phongNoText.fragmentshader");
	}else if(TextureBump){
		setShaders("shaders/tangentShading.vertexshader", "shaders/phongBump.fragmentshader");

	}else if(TextureNorm){
		setShaders("shaders/tangentShading.vertexshader", "shaders/normal.fragmentshader");

	}else{
		setShaders("shaders/StandardShading.vertexshader", "shaders/phong.fragmentshader");

	}

	for(Mesh* m : vmesh){
		m->autoShaders();
	}
}

void Mesh::loadTextureDiff(std::string filename, bool propagate){

	if(isRoot || propagate){
		for(Mesh* m : vmesh){
			m->loadTextureDiff(filename,true);
		}
		if(isRoot)
			return;
	}

	std::cout << "LOAD TEX ::::::" <<texturePath + filename<< std::endl;

	TextureDiff = TextureHandler::getInstance()->getTextureID(texturePath + filename);
	TextureHandler::getInstance()->getTextureFormat(texturePath + filename,format);

	
}


void Mesh::loadTextureSpec(std::string filename, bool propagate){

	if(isRoot || propagate){
		for(Mesh* m : vmesh){
			m->loadTextureSpec(filename,true);
		}
		if(isRoot)
			return;
	}

	TextureSpec = TextureHandler::getInstance()->getTextureID(texturePath + filename);

}

void Mesh::loadTextureBump(std::string filename, bool propagate){

	if(isRoot || propagate){
		for(Mesh* m : vmesh){
			m->loadTextureBump(filename,true);
		}
		if(isRoot)
			return;
	}

	TextureBump = TextureHandler::getInstance()->getTextureID(texturePath + filename);
	int w,h;
	TextureHandler::getInstance()->getTextureSize(texturePath + filename,w,h);
	texelOffset.x=1.0/float(w);
	texelOffset.y=1.0/float(h);

}

void Mesh::loadTextureNormal(std::string filename, bool propagate){

	if(isRoot || propagate){
		for(Mesh* m : vmesh){
			m->loadTextureNormal(texturePath + filename,true);
		}
		if(isRoot)
			return;
	}

	TextureNorm = TextureHandler::getInstance()->getTextureID(texturePath + filename);
}

void Mesh::loadTextureEmissive(std::string filename, bool propagate){

	if(isRoot || propagate){
		for(Mesh* m : vmesh){
			m->loadTextureEmissive(texturePath + filename,true);
		}
		if(isRoot)
			return;
	}

	TextureEmissive = TextureHandler::getInstance()->getTextureID(texturePath + filename);
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

	computeTangentBasis(vertices,uvs,normals,tangents,bitangents);

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

	glGenBuffers(1, &tangenbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangenbuffer);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &bitangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);



}


void Mesh::draw(glm::mat4 modelmat){

	//skel->ModelMatrix = glm::translate(ModelMatrix,glm::vec3(0,0,1));
	//skel->draw();
	//std::cout <<"\n\n" << name << "\n" << glm::to_string(ModelMatrix) << std::endl;
	if(format == GL_RGBA){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

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


	glUniform2fv(TexelOffsetID,1, &texelOffset[0]);
	

		// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureDiff);
	glUniform1i(TextureID, 0);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureSpec);
	glUniform1i(TextureSpecID, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureBump);
	glUniform1i(TextureBumpID, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureNorm);
	glUniform1i(TextureNormID, 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, TextureEmissive);
	glUniform1i(TextureEmissiveID, 4);

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

		// 4rd attribute buffer : normals
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, tangenbuffer);
	glVertexAttribPointer(
			3,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// 5rd attribute buffer : normals
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glVertexAttribPointer(
			4,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);



	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


	for(Mesh* m : vmesh){
		m->draw(ModelMatrix);
	}




}