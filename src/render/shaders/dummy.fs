#version 330 core

in vec3 positionModel;
in vec3 positionWorld;
in vec3 normalWorld;
in vec3 diffuseColor;
flat in int textureIndex;
in vec2 textureCoord;

uniform sampler2DArray gBonusesTextureArray;
uniform vec4 gClipPlane;

out vec3 color;

void main() {
  if (dot(gClipPlane, vec4(positionModel, 1)) > 0)
    discard;
  vec4 textureColor = texture(gBonusesTextureArray, vec3(textureCoord, textureIndex));
  color = textureColor.rgb + diffuseColor * (0.2 * max(0.0, normalWorld.z + normalWorld.x + normalWorld.y) + 0.2);
}