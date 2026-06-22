#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
#ifndef M_PI
#define M_PI 3.1415926535897932384626
#endif

const int WIDTH{800};
const int HEIGHT{600};
const int CIRCLE_SEGMENTS{50};
void draw_circle(float x, float y, const float radius) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y);
  constexpr float rotation_angle{2.0f * M_PI /
                                 static_cast<float>(CIRCLE_SEGMENTS)};
  for (int i{0}; i <= CIRCLE_SEGMENTS; i++) {
    const float x_c{x +
                    radius * std::cos(static_cast<float>(i) * rotation_angle)};
    const float y_c{y +
                    radius * std::sin(static_cast<float>(i) * rotation_angle)};
    glVertex2f(x_c, y_c);
  }
  glEnd();
}
void draw_outline(float x, float y, const float radius) {
  glBegin(GL_LINE_LOOP);
  for (int i{0}; i < CIRCLE_SEGMENTS; ++i) {
    const float rotation_angle{static_cast<float>(i) * 2.0f *
                               static_cast<float>(M_PI) /
                               static_cast<float>(CIRCLE_SEGMENTS)};
    const float x_c{x + radius * std::cos(rotation_angle)};
    const float y_c{y + radius * std::sin(rotation_angle)};
    glVertex2f(x_c, y_c);
  }
  glEnd();
}
constexpr const char *OPENGL_SHADER_SRC = R"(#version 450 core
layout(location = 0) in vec2 pos_a;

void main(){
	gl_Position = vec4(pos_a, 0.0, 1.0);
}

)";

constexpr char const *PENGL_SHADER_SRC = R"(#version 450 core
layout(location = 0) out vec4 frag_color;

void main(){
	frag_color = vec4(1.0);
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
  auto glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW!";
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  int fbwight, fbheight;
  glClearColor(0.0f, 0.8f, 0.4f, 1.0f);
  constexpr float data[] = {
      0.f, .5f, -.5f, -.5f, .5f, -.5f,
  };

  GLuint vbo;
  glCreateBuffers(1, &vbo);
  glNamedBufferStorage(vbo, sizeof(data), data, 0);
  GLuint vao;
  glCreateVertexArrays(1, &vao);
  glVertexArrayVertexBuffer(vao, 0, vbo, 0, 2 * sizeof(float));
  glVertexArrayBindingDivisor(vao, 0, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
  glEnableVertexArrayAttrib(vao, 0);
  GLuint vertex_shader{compile_shader(GL_VERTEX_SHADER, OPENGL_SHADER_SRC)};
  GLuint fragment_shader{compile_shader(GL_FRAGMENT_SHADER, PENGL_SHADER_SRC)};
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glUseProgram(program);
  glBindVertexArray(vao);

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
    // INFO: drawing the center of ther black hole
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
  }
  glDeleteProgram(program);
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
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
