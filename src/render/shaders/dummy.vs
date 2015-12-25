#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 4) in mat4 ModelToWorld;

uniform mat4 VP;

out vec3 vertexNormal;

void main() {
  gl_Position = VP * ModelToWorld * vec4(vertexPosition_modelspace, 1);
  vertexNormal = (ModelToWorld * vec4(vertexNormal_modelspace, 0)).xyz;
}