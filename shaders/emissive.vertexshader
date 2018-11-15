#version 330 core

// Aqui van los vertex buffer que mandamos al GPU
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;




// datos de salida hacia el fragment shader (lo que tenemos que calcular)
out vec2 UV;

// Datos uniformes al objeto
uniform mat4 MVP;


void main(){

	// gl_position es la position del vertice despues de la proyeccion
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	
	
	UV = vertexUV;
}

