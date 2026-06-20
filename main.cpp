#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>
#ifndef M_PI
#define M_PI 3.1415926535897932384626
#endif

const int WIDTH {800};
const int HEIGHT {600};
const int CIRCLE_SEGMENTS{50};
void  draw_circle(float x, float y, const float radius){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	constexpr float rotation_angle {2.0f * M_PI / static_cast<float>(CIRCLE_SEGMENTS)};
	for (int i{0};i <= CIRCLE_SEGMENTS; i++) {
		const float x_c {x + radius * std::cos(static_cast<float>(i) * rotation_angle)};
		const float y_c{y + radius * std::sin(static_cast<float>(i) * rotation_angle)};
		glVertex2f(x_c, y_c);
	}
	glEnd();
}
void draw_outline(float x, float y, const float radius){
	glBegin(GL_LINE_LOOP);
	for (int i{0};i < CIRCLE_SEGMENTS; ++i) {
		const float rotation_angle {static_cast<float>(i)*2.0f * static_cast<float>(M_PI) / static_cast<float>(CIRCLE_SEGMENTS)};
		const float x_c {x + radius * std::cos(rotation_angle)};
		const float y_c{y + radius * std::sin(rotation_angle)};
		glVertex2f(x_c, y_c);
	}
	glEnd();
}
int main() {
	if (!glfwInit()){
		exit(EXIT_FAILURE);
	}
	GLFWwindow* window {glfwCreateWindow(WIDTH, HEIGHT, "Slappe", NULL, NULL)};
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
	int fbwight, fbheight;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwGetFramebufferSize(window, &fbwight,&fbheight);
	glViewport(0, 0,fbwight, fbheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0, 10.0,-10.0, 10.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// INFO: drawing the cente of ther black hole
		glColor3f(0.0f, 0.0f, 0.0f);
		draw_circle(0.0f, 0.0f, 1.0f);
		// INFO: drawing the event horizon on top
		glColor3f(0.0f, 0.8f, 0.8f);
		draw_outline(0.0f, 0.0f,1.5f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
