#include "App.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <fmt/core.h>

#include <exception>
#include <glm/trigonometric.hpp>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

#include "Vertices.h"

void App::run() {
  init();
  mainLoop();
  cleanup();
}

void App::init() {
  if(!glfwInit()) {
    throw std::runtime_error("GLFW doesn't work");
  }
  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_SAMPLES, 4);
  window = glfwCreateWindow(1600, 900, "OpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  glewExperimental = true;
  auto glewStatus = glewInit();
  if(glewStatus != GLEW_OK) {
    std::string errorString((const char*)glewGetErrorString(glewStatus));
    throw std::runtime_error("GLEW doesn't work: " + errorString);
  }

  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  
  glfwSetWindowUserPointer(window, this);
  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto app = static_cast<App*>(glfwGetWindowUserPointer(window));

    if(action == GLFW_PRESS) {
      app->pressedKeys[key] = true;
    }
    if(action == GLFW_RELEASE) {
      app->pressedKeys[key] = false;
    }
  });

  glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) { 
    glViewport(0, 0, width, height);
  });

  std::vector<float> vertices = {
    0.0f, 0.0f, 0.0f,
     0.01f,  0.0f, 0.0f,
    0.01f,  0.90f, 0.0f,
  };

  glGenVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);  

  // Cube
  glGenVertexArrays(1, &cubeVao);
  glCreateBuffers(1, &cubeVbo);

  glBindVertexArray(cubeVao);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeWithNormals), cubeWithNormals, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  shader = std::make_unique<Shader>("assets/basic.vert", "assets/basic.frag");

  fmt::print("Found: {}", modelLocation);

  running = true;
}

void App::cleanup() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void App::mainLoop() {
  double accum = 0.0f;
  double lastTime = glfwGetTime();
  while(running) {
    glfwPollEvents();
    double currentTime = glfwGetTime();
    accum += currentTime - lastTime;
    lastTime = currentTime;

    while(accum > 0.0f) {
      accum -= fixedDt;
      update();
      tickCounter++;
    }
    shader->use();
    render();
    fpsCounter++;

    if(glfwWindowShouldClose(window)) {
      running = false;
    }
    if(currentTime - timer > 1.0f) {
      timer = currentTime;
      fmt::print("FPS: {}, Ticks: {}\n", fpsCounter, tickCounter);
      fpsCounter = 0;
      tickCounter = 0;
    }
  }
}

void App::update() {
  shader = std::make_unique<Shader>("assets/basic.vert", "assets/basic.frag");
}


void App::render() {
  double time = glfwGetTime();

  glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();


  float centerDistance = (glm::cos(time * 0.65) + 1.0f) * 0.8f + 0.05f;
  float camAngle = glm::radians(90.0f);
  glm::vec3 eye = glm::vec3(glm::cos(camAngle), 0.0f, glm::sin(camAngle)) * 7.0f + glm::vec3(0.0f, 6.0f, 0.0f);

  auto view = glm::lookAt(
    eye,
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));
  auto projection = glm::perspective(glm::radians(88.f), 16.f / 9.f, 0.3f, 300.f);

  glUniform3fv(glGetUniformLocation((*shader)(), "CameraEye"), 1, glm::value_ptr(eye));
  glUniform1f(glGetUniformLocation((*shader)(), "Time"), time);
  modelLocation = glGetUniformLocation((*shader)(), "Model");
  glUniformMatrix4fv(glGetUniformLocation((*shader)(), "View"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation((*shader)(), "Projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glBindVertexArray(cubeVao);

  glm::mat4 model{1.0f};
  model = glm::rotate(model, (float)time, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::translate(model, glm::vec3(0.0f, height, 0.0f));
  glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
  ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
  ImGui::SliderFloat("Slider", &height, -10.0f, 10.0f);
  ImGui::End();
  ImGui::Render();
  
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}
