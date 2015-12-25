#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

uniform mat4 MVP;
uniform mat4 ModelToWorld;

out vec3 vertexNormal;

void main() {
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
  vertexNormal = (ModelToWorld * vec4(vertexNormal_modelspace, 0)).xyz;
}