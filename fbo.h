
#ifndef FBO_H
#define FBO_H

#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class FBO 
{ 
    // Access specifier 
public:

 FBO(int w, int h);
 FBO(std::string filename, float scale=1.0);
 ~FBO();
 void setFBO();
 void draw();
 GLuint FramebufferID;
 GLuint renderedTexture;
 GLuint depthrenderbuffer;
 GLuint programID;
 GLuint texID;
 GLuint quad_vertexbuffer;

 int width;
 int height;

};


#endif


