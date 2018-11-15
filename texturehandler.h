
#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H


#include <vector>
#include <string>
#include <GLFW/glfw3.h>

struct Texture{
	std::string filename;
	GLuint textureID;
	int w;
	int h;
	GLuint format;
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
 void getTextureSize(std::string filename,int &w, int &h);
 void getTextureFormat(std::string filename,GLuint &format);
 std::vector<Texture> vTextures;

};

#endif