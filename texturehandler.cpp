#include <GL/glew.h>

#include "texturehandler.h"
#include "common/texture.hpp"

TextureHandler* TextureHandler::textureHandler = 0;

TextureHandler::TextureHandler(){

}

TextureHandler::~TextureHandler(){
	vTextures.clear();
}

TextureHandler* TextureHandler::getInstance(){
	if (textureHandler == 0)
    {
        textureHandler = new TextureHandler();
    }

    return textureHandler;

}


 GLuint TextureHandler::getTextureID(std::string filename){
 	for (Texture t: vTextures){
 		if(t.filename == filename){
 			if(glIsTexture(t.textureID))
 				return t.textureID;
 			else
 				t.filename="";
 		}
 	}

	GLuint textureID = loadBMP_custom(filename.c_str());
	Texture t;
	t.filename = filename;
	t.textureID = textureID;
	vTextures.push_back(t);

	return textureID;


 }

