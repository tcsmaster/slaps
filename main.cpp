#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <iostream>
#ifndef M_PI
#define M_PI 3.1415926535897932384626
#endif
const int WIDTH{800};
const int HEIGHT{600};
const int NUM_PARTICLES{3};
const float rect_height{0.2f};
const float rect_width{0.4f};
constexpr const char *VERTEX_SHADER_SRC = R"(#version 450 core
layout(location = 0) in vec3 pos_a;

void main(){
	gl_Position = vec4(pos_a,  1.0);
}

)";

constexpr char const *FRAGMENT_SHADER_SRC = R"(#version 450 core
layout(location = 0) out vec4 frag_color;

void main(){
	frag_color = vec4(0.4, 0.5, 0.2, 1.0);
}

)";
GLuint compile_shader(GLenum type, const char *src);
int main() {
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window{
      glfwCreateWindow(WIDTH, HEIGHT, "Slappe", nullptr, nullptr)};
  if (!window) {
    std::cerr << "Failed to create GLHF window!";
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD!" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow *, int w, int h) { glViewport(0, 0, w, h); });
  glClearColor(0.0f, 0.8f, 0.4f, 1.0f);
  float vertices[] = {-.5f, -.5f, .0f, -.5f, .5f,  .0f,
                      .5f,  .5f,  .0f, .5f,  -.5f, .0f};
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};
  /*
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<std::mt19937::result_type>
  dist_angle(0.f, 1.f);
  */
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glEnableVertexArrayAttrib(VAO, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  GLuint vertex_shader{compile_shader(GL_VERTEX_SHADER, VERTEX_SHADER_SRC)};
  GLuint fragment_shader{
      compile_shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC)};
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glUseProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    std::string log(length, '\0');
    glGetProgramInfoLog(program, length, nullptr, log.data());
    std::cerr << "Program link error: " << log << std::endl;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
  }
  glDeleteProgram(program);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
GLuint compile_shader(GLenum type, const char *src) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::string log(length, '\0');
    glGetShaderInfoLog(shader, length, nullptr, log.data());
    std::cerr << "Shader compile error: " << log << std::endl;
  }
  return shader;
}
