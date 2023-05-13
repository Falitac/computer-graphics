#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>

class Shader {
public:
  Shader(const std::string& vertexShaderLocation, const std::string& fragmentShaderLocation);

  Shader(const Shader&) = delete;
  Shader& operator=(Shader&) = delete;
  Shader(Shader&&);
  Shader& operator=(Shader&&);

  void use();

  GLuint operator()() const;
private:
  GLuint compileShader(GLenum type, const std::string& source);
private:
  GLuint program;
};