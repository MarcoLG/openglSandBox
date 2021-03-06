#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>

class Skeleton;
class Object3D;

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs, 
	std::vector<glm::vec3> & out_normals
);


bool loadFBX(
	const char * path, 
	Object3D* obj,
	Skeleton* skel,
	float scale=1.0
);



bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif