#version 330 core

// datos recibidos del vertex shader despues de interpolacion
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace[2];

// Output <- color
out vec3 color;

// datos unifromes a todo el objeto
uniform mat4 MV;
uniform int LightCount;

struct Light {
	vec4 position ;
	vec4 color ;
};

layout(std140) uniform LightBlock{
	Light lights[2];
};

vec3 coolColor = vec3(0.25, 0.25, 0.75);
vec3 warmColor = vec3(0.80, 0.75, 0.25);

vec3 lit(in vec3 l,in vec3 n,in vec3 v){
	
	vec3 r = reflect(-l,n);
	float s = clamp( 100.0 * dot ( r , v ) - 97.0 , 0.0 , 1.0);
	
	vec3 highlightColor = vec3 (2.0 ,2.0 ,2.0) ;

	vec3 c = mix ( warmColor , highlightColor , s ) ;
	return c;

}

void main(){

	


	// Normal despues de la interpolacion
	vec3 n = normalize( Normal_cameraspace );
	vec3 v = normalize(EyeDirection_cameraspace);


	color= 0.5*coolColor;
	for(int i=0;i<LightCount;i++){// Distancia hacia la luz
		float distance = length( vec3(lights[i].position) - Position_worldspace );
		//frosbite / cryengine
		vec3 LightColor = vec3(lights[i].color)*pow(4.0/(max(distance,0.1f)),2);

		vec3 l = normalize( LightDirection_cameraspace[i] );
		float NdL = clamp ( dot (n , l ) , 0.0 , 1.0) ;

		color += NdL * lights[i].color.rgb * lit (l ,n , v ) ;

	}

	

}