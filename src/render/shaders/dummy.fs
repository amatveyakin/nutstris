#version 330 core

in vec3 vertexNormal;
in vec3 vertexColor;

out vec3 color;

void main() {
  color = vertexColor * (0.2 * max(0.0, vertexNormal.z + vertexNormal.x + vertexNormal.y) + 0.2);
}