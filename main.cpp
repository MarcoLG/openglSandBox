// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include "object3d.h"
#include "gizmo.h"

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"



int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Phong Shader", NULL, NULL);
	glfwSetWindowPos(window, 1500, 0);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024/2, 768/2);


	// Dark blue background
	glClearColor(0.125f, 0.152f, 0.16f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);


	Object3D* trex = new Object3D("data/trex/source/trex.fbx");
	trex->loadTexture("data/trex/textures/diffuse.bmp");
	trex->loadTextureSpec("data/trex/textures/spec.bmp");
	trex->setShaders("shaders/StandardShading.vertexshader", "shaders/phong.fragmentshader");
	trex->ModelMatrix = glm::scale(trex->ModelMatrix,glm::vec3(0.1f,0.1f,0.1f));


	Gizmo* gizmo = new Gizmo();
	gizmo->setShaders("shaders/gizmo.vertexshader", "shaders/gizmo.fragmentshader");

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();	
		gizmo->draw();
		//hands->draw();
		//trex->draw();
		trex->draw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	// Close OpenGL window and terminate GLFW
		glfwTerminate();

	return 0;
}
