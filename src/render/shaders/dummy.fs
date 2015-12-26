#version 330 core

in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 textureCoord;
flat in int textureIndexFS;

uniform sampler2DArray textures;

out vec3 color;

void main() {
  vec4 textureColor = texture(textures, vec3(textureCoord, textureIndexFS));
  color = textureColor.rgb + vertexColor * (0.2 * max(0.0, vertexNormal.z + vertexNormal.x + vertexNormal.y) + 0.2);
}