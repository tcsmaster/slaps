#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
int main() {
	if (!glfwInit()){
		exit(EXIT_FAILURE);
	}
	const int WIDTH = 800;
	const int HEIGHT = 600;
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Slappe", NULL, NULL);
	if (!window){
		std::cerr << "Failed to create GLHF window!";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	auto glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK){
		std::cerr << "Failed to initialize GLEW!";
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
