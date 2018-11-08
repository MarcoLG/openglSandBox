
#include <GL/glew.h>


#include <glm/gtc/matrix_transform.hpp>


#include <iostream>

#include "object3d.h"
#include "mesh.h"

#include "skeleton.h"

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"



Object3D::Object3D(){

	ModelMatrix = glm::mat4(1.0);
}

Object3D::Object3D(std::string filename, float scale){

	ModelMatrix = glm::mat4(1.0);
	meshFilename = filename;
	load_scale=scale;

	loadMesh();

}

Object3D::~Object3D(){

	if(skel != NULL)
		delete skel;
	if(rootMesh != NULL)
		delete rootMesh;


}

void Object3D::setShaders(std::string vfile,std::string ffile){

	rootMesh->setShaders(vfile,ffile);

}



void Object3D::loadMesh(){

	skel = new Skeleton();
	rootMesh = new Mesh();
	rootMesh->isRoot=true;

	loadFBX(meshFilename.c_str(), this,skel, load_scale);

}

void Object3D::loadTextureDiffuse(std::string filename){
	rootMesh->loadTextureDiff(filename);
}



void Object3D::loadTextureSpec(std::string filename){
	rootMesh->loadTextureSpec(filename);
}


void Object3D::draw(){

	rootMesh->draw(ModelMatrix);

}