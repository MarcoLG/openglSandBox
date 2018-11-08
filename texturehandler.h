
#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H


#include <vector>
#include <string>
#include <GLFW/glfw3.h>

struct Texture{
	std::string filename;
	GLuint textureID;
};

class TextureHandler 
{ 
    // Access specifier 
private:
 static TextureHandler* textureHandler;
 explicit TextureHandler();
 ~TextureHandler();

public:
 static TextureHandler* getInstance();
 GLuint getTextureID(std::string filename);
 std::vector<Texture> vTextures;

};

#endif