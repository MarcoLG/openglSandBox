
#ifndef OBJECT3D_H
#define OBJECT3D_H


#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Skeleton;



class Object3D 
{ 
    // Access specifier 
public:

 Object3D();
 Object3D(std::string filename, float scale=1.0);
 ~Object3D();

 void setShaders(std::string vfile,std::string ffile);
 void loadLights();
 void loadMesh();
 void loadTexture(std::string filename);
 void loadTextureSpec(std::string filename);
 void draw();

 std::string vertShaderFilename;
 std::string fragShaderFilename;
 std::string meshFilename;
 std::string textureFilename;


 std::vector<glm::vec3> vertices;
 std::vector<glm::vec2> uvs;
 std::vector<glm::vec3> normals;

 GLuint programID;
 GLuint MatrixID;
 GLuint ViewMatrixID;
 GLuint ModelMatrixID;
 GLuint TextureID;
 GLuint TextureSpecID;
 GLuint TextureHandler;
 GLuint VertexArrayID;
 GLuint LightsBlockID;
 GLuint LightCountID;
 GLuint lightBindingPoint;

 GLuint Texture;
 GLuint TextureSpec;
 GLuint vertexbuffer;
 GLuint normalbuffer;
 GLuint uvbuffer;
 GLuint lightbuffer;

 glm::mat4 ModelMatrix;


 float load_scale;

 Skeleton* skel;

}; 

#endif