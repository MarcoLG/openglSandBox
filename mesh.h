
#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Skeleton;



class Mesh 
{ 
    // Access specifier 
public:

 Mesh();
 ~Mesh();

 void loadMesh();
 void setShaders(std::string vfile,std::string ffile);
 void loadLights();
 void loadTextureDiff(std::string filename,bool propagate=false);
 void loadTextureSpec(std::string filename,bool propagate=false);
 void draw(glm::mat4 modelmat);
 bool isRoot;
 std::vector<Mesh*> vmesh;

 std::string vertShaderFilename;
 std::string fragShaderFilename;
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

 GLuint TextureDiff;
 GLuint TextureSpec;
 GLuint vertexbuffer;
 GLuint normalbuffer;
 GLuint uvbuffer;
 GLuint lightbuffer;

 int lightCount ;

 glm::mat4 ModelMatrix;


 float load_scale;

 Skeleton* skel;

 std::string name;

}; 

#endif