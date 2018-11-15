
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

 Mesh(std::string modelPath);
 ~Mesh();

 void loadMesh();
 void setShaders(std::string vfile,std::string ffile);
 void autoShaders();
 void loadLights();
 void loadTextureDiff(std::string filename,bool propagate=false);
 void loadTextureSpec(std::string filename,bool propagate=false);
 void loadTextureBump(std::string filename,bool propagate=false);
 void loadTextureNormal(std::string filename,bool propagate=false);
 void loadTextureEmissive(std::string filename,bool propagate=false);
 void draw(glm::mat4 modelmat);
 bool isRoot;
 std::vector<Mesh*> vmesh;

 std::string vertShaderFilename;
 std::string fragShaderFilename;
 std::string texturePath;


 std::vector<glm::vec3> vertices;
 std::vector<glm::vec2> uvs;
 std::vector<glm::vec3> normals;
 std::vector<glm::vec3> tangents;
 std::vector<glm::vec3> bitangents;

 GLuint programID;
 GLuint MatrixID;
 GLuint ViewMatrixID;
 GLuint ModelMatrixID;
 GLuint TextureID;
 GLuint TextureSpecID;
 GLuint TextureBumpID;
 GLuint TextureNormID;
 GLuint TextureEmissiveID;
 GLuint TextureHandler;
 GLuint VertexArrayID;
 GLuint LightsBlockID;
 GLuint LightCountID;
 GLuint TexelOffsetID;
 GLuint lightBindingPoint;

 GLuint TextureDiff;
 GLuint TextureSpec;
 GLuint TextureBump;
 GLuint TextureNorm;
 GLuint TextureEmissive;
 GLuint vertexbuffer;
 GLuint normalbuffer;
 GLuint uvbuffer;
 GLuint tangenbuffer;
 GLuint bitangentbuffer;
 GLuint lightbuffer;
 glm::vec2 texelOffset;
 
 GLuint format;

 int lightCount ;

 glm::mat4 ModelMatrix;


 float load_scale;

 Skeleton* skel;

 std::string name;

}; 

#endif