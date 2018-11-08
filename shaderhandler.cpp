#include <GL/glew.h>

#include "shaderhandler.h"
#include "common/shader.hpp"

ShaderHandler* ShaderHandler::shaderHandler = 0;

ShaderHandler::ShaderHandler(){

}

ShaderHandler::~ShaderHandler(){
	vShaders.clear();
}

ShaderHandler* ShaderHandler::getInstance(){
	if (shaderHandler == 0)
    {
        shaderHandler = new ShaderHandler();
    }

    return shaderHandler;

}


 GLuint ShaderHandler::getShader(std::string vshaderFilename, std::string fshaderFilename){
 	for (Shader s: vShaders){
 		if(s.vshader == vshaderFilename && s.fshader == fshaderFilename){
 			if(glIsProgram(s.programID))
 				return s.programID;
 			else
 				s.vshader = "";
 		}
 	}

	GLuint programID = LoadShaders( vshaderFilename.c_str(), fshaderFilename.c_str());
	Shader s;
	s.vshader = vshaderFilename;
	s.fshader = fshaderFilename;
	s.programID = programID;
	vShaders.push_back(s);

	return programID;


 }

