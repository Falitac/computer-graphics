#include "gfx/Shader.h"
#include <fstream>
#include <sstream>
#include <fmt/core.h>

Shader::Shader(const std::string& vertexShaderLocation, const std::string& fragmentShaderLocation) {
  std::ifstream vertexFile(vertexShaderLocation);
  std::ifstream fragmentFile(fragmentShaderLocation);

  if(!vertexFile.good()) {
    fmt::print("No such file {}\n", vertexShaderLocation);
    return;
  }
  if(!vertexFile.good()) {
    fmt::print("No such file {}\n", fragmentShaderLocation);
    return;
  }

  std::stringstream ss;
  ss << vertexFile.rdbuf();
  auto vertexShader = compileShader(GL_VERTEX_SHADER, ss.str());

  ss.str(std::string()); // clean stream
  ss << fragmentFile.rdbuf();
  auto fragmentShader = compileShader(GL_FRAGMENT_SHADER, ss.str());

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint linkStatus;
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  if(!linkStatus) {
    char errorBuffer[1024];
    glGetProgramInfoLog(program, 1024, NULL, errorBuffer);
    fmt::print("Linkage error:\n{}\n", errorBuffer);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::Shader(Shader&& otherShader) {
  program = otherShader.program;
  otherShader.program = 0;
}

Shader& Shader::operator=(Shader&& otherShader) {
  program = otherShader.program;
  otherShader.program = 0;
  return *this;
}

void Shader::use() {
  glUseProgram(program);
}

GLuint Shader::operator()() const {
  return program;
}

GLuint Shader::compileShader(GLenum type, const std::string& source) {
  GLuint shader = glCreateShader(type);

  const char* cstringSource = source.c_str();
  glShaderSource(shader, 1, &cstringSource, NULL);
  glCompileShader(shader);

  GLint compileStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
  if(!compileStatus) {
    char errorBuffer[1024];
    glGetShaderInfoLog(shader, 1024, NULL, errorBuffer);
    fmt::print("Error:\n{}\n", errorBuffer);
  }

  return shader;
}