#pragma once

#include "gfx/Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SFML/Audio.hpp>
#include <glm/glm.hpp>

#include <memory>
#include <map>
#include <random>

class App {
public:
  App() = default;
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  void run();

private:
  void init();
  void cleanup();

  void mainLoop();
  void update();
  void render();

  float getDt() const { return fixedDt; };

public:
  std::map<int, bool> pressedKeys;
private:
  bool running;
  const float fixedDt = 1. / 60.f;

  uint32_t fpsCounter = 0;
  uint32_t tickCounter = 0;
  double timer = 0.0;

  GLFWwindow* window;

  GLuint vao;
  GLuint vbo;

  GLuint cubeVao;
  GLuint cubeVbo;

  bool alreadyPressed = false;

  std::unique_ptr<Shader> shader;
  GLint modelLocation;
  float height = 0.0;

  std::vector<glm::vec3> transformations;
};