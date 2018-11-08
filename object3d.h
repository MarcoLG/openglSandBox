
#ifndef OBJECT3D_H
#define OBJECT3D_H


#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Skeleton;
class Mesh;


class Object3D 
{ 
    // Access specifier 
public:

 Object3D();
 Object3D(std::string filename, float scale=1.0);
 ~Object3D();

 void setShaders(std::string vfile,std::string ffile);
 void loadMesh();
 void loadTextureDiffuse(std::string filename);
 void loadTextureSpec(std::string filename);
 void draw();

 glm::mat4 ModelMatrix;
 std::string meshFilename;
 Mesh* rootMesh;
 float load_scale;

 Skeleton* skel;

}; 

#endif