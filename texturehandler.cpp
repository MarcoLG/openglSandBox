#include <GL/glew.h>
#include <iostream>
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

 	int w,h;
 	GLuint format;


	GLuint textureID;


	int beginIdx = filename.find_last_of('.');
	std::string extension = filename.substr(beginIdx+1);
	std::cout << "LOAD :::  " << extension << std::endl;
	if(extension == "jpg" || extension == "jpeg" || extension == "JPG" || extension == "JPEG" )
		textureID = loadJPEG(filename.c_str(),w,h,format);
	else if(extension == "png" )
		textureID = loadPNG(filename.c_str(),w,h,format);
	else if(extension == "bmp" )
		textureID = loadBMP_custom(filename.c_str(),w,h,format);
	else if(extension == "dds" || extension == "DDS")
		textureID = loadDDS(filename.c_str(),w,h,format);
	else if(extension == "tga" || extension == "TGA")
		textureID = loadTGA(filename.c_str(), w, h,format);


	std::cout << "LOAD :::  " << filename<< " w " << w << " h "<< h << " ID " << textureID;
	if(format == GL_RGB){
		std::cout << " FORMAT GL_RGB " << std::endl;
	}else{
		std::cout << " FORMAT GL_RGBA " << std::endl;
	}
	Texture t;
	t.filename = filename;
	t.textureID = textureID;
	t.w=w;
	t.h=w;
	t.format=format;
	vTextures.push_back(t);
	

	return textureID;


 }


 void TextureHandler::getTextureSize(std::string filename,int &w, int &h){
 	for (Texture t: vTextures){
 		if(t.filename == filename){
 			if(glIsTexture(t.textureID))
 			{
 				w=t.w;
 				h=t.h;
 			}
 		}
 	}

 }

  void TextureHandler::getTextureFormat(std::string filename,GLuint &format){
 	for (Texture t: vTextures){
 		if(t.filename == filename){
 			if(glIsTexture(t.textureID))
 			{
 				format=t.format;
 			}
 		}
 	}

 }

