#version 330 core

// datos recibidos del vertex shader despues de interpolacion
in vec2 UV;

// Output <- color
out vec4 color;

// datos unifromes a todo el objeto
uniform sampler2D emissiveTextureSampler;
uniform mat4 MV;


void main(){

	

	

	color = vec4(texture( emissiveTextureSampler, UV ).rgb,1.0);



	

}