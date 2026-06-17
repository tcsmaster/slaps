#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <glm/glm.hpp>

int main() {
	if (!glfwInit()){
		exit(EXIT_FAILURE);
	}
	const int WIDTH = 800;
	const int HEIGHT = 600;
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Slappe", NULL, NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	return 0;
}
