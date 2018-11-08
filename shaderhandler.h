
#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H


#include <vector>
#include <string>
#include <GLFW/glfw3.h>

struct Shader{
	std::string vshader;
	std::string fshader;
	GLuint programID;
};

class ShaderHandler 
{ 
    // Access specifier 
private:
 static ShaderHandler* shaderHandler;
 explicit ShaderHandler();
 ~ShaderHandler();

public:
 static ShaderHandler* getInstance();
 GLuint getShader(std::string vshaderFilename, std::string fshaderFilename);
 std::vector<Shader> vShaders;

};

#endif