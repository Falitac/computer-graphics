#version 440 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;

out vec3 fPos;
out vec3 fNormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
  mat4 MVP = Projection * View * Model;
  gl_Position = MVP * vec4(iPos, 1.0);

  fPos = vec3(Model * vec4(iPos, 1.0));
  fNormal = mat3(transpose(inverse(Model))) * iNormal;
  fNormal = normalize(fNormal);
}