#version 330 core

in vec3 vertexNormal;

out vec3 color;

void main() {
  color = vec3(1,0,0) * (0.2 * max(0.0, vertexNormal.z + vertexNormal.x + vertexNormal.y) + 0.2);
}