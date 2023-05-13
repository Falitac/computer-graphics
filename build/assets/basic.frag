#version 440 core

in vec3 fPos;
in vec3 fNormal;

out vec4 colorResult;

uniform float Time;
uniform vec3 CameraEye;

uniform mat4 Model;

void main() {
	float radius = 5.0;
  
  vec3 lightPos = vec3(0.0, 3.0, 0.0);
	lightPos.x = cos(Time) * radius;
	lightPos.z = sin(Time) * radius;
  vec3 lightDir = normalize(lightPos - fPos);

	vec3 viewDir = normalize(CameraEye - fPos);
	vec3 reflectDir = reflect(-lightDir, fNormal);

  vec3 ambient = vec3(0.11);
  float diffuse = max(dot(lightDir, fNormal), 0.0);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);

  vec3 color3 =  ambient + vec3(diffuse) + vec3(specular);

  colorResult = vec4(color3, 1.0); 
}