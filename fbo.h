
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

 FBO(int w, int h,std::string vfile,std::string ffile);
 ~FBO();
 void setFBO();
void setShaders(std::string vfile,std::string ffile);
void setEmissiveTexture(GLuint texture);
 void draw();
 GLuint FramebufferID;
 GLuint renderedTexture;
 GLuint renderedGlowTexture;
 GLuint depthrenderbuffer;
 GLuint programID;
 GLuint texID;
 GLuint texGlowID;
 GLuint quad_vertexbuffer;
 GLuint TexelOffsetID;
 bool useEmissive;

 int width;
 int height;
 glm::vec2 texelOffset;

};


#endif


