#include <GL/glew.h>


#include "lighthandler.h"
#include "common/texture.hpp"

#include <iostream>

LightHandler* LightHandler::lightHandler = 0;

LightHandler::LightHandler(){
	lightbuffer=0;
}

LightHandler::~LightHandler(){
	vLights.clear();

	glDeleteBuffers(1, &lightbuffer);
}

LightHandler* LightHandler::getInstance(){
	if (lightHandler == 0)
    {
        lightHandler = new LightHandler();
    }

    return lightHandler;

}

void LightHandler::addLight(glm::vec4 pos, glm::vec4 c){
	Light l;
	l.pos = pos;
	l.color = c;
	vLights.push_back(l);
}


GLuint LightHandler::getLightBuffer(){
 	if(lightbuffer)
 		return lightbuffer;
 	std::cout << "size ::: " <<vLights.size()*8 << std::endl;
 	std::vector<float> lights;
 	
 	int cont=0;
 	for (Light l : vLights){
 		for(int i = 0;i<4;i++){
 			lights.push_back(l.pos[i]);
 		}
 		for(int i = 0;i<4;i++){
 			lights.push_back(l.color[i]);
 		}
 	}



 	glGenBuffers(1, &lightbuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightbuffer);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(float)*lights.size(),lights.data(),GL_DYNAMIC_DRAW);
	std::cout << "LIGHT HANDLER BUFFER" << lightbuffer <<std::endl;
	return lightbuffer;

}

