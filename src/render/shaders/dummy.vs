#version 330 core

layout(location = 0) in vec3 positionModelIn;
layout(location = 1) in vec3 normalModelIn;
layout(location = 2) in int  textureIndexIn;
layout(location = 4) in mat4 modelToWorldIn;
layout(location = 8) in vec3 diffuseColorIn;

uniform mat4 gVP;
uniform mat4 gGlobalRotation;

out vec3 positionModel;
out vec3 positionWorld;
out vec3 normalWorld;
out vec3 diffuseColor;
flat out int textureIndex;
out vec2 textureCoord;

void main() {
  positionModel = positionModelIn;

  positionWorld = (modelToWorldIn * vec4(positionModelIn, 1)).xyz;
  positionWorld = (gGlobalRotation * vec4(positionWorld, 1)).xyz;

  normalWorld = (gGlobalRotation * modelToWorldIn * vec4(normalModelIn, 0)).xyz;

  gl_Position = gVP * vec4(positionWorld, 1);

  diffuseColor = diffuseColorIn;
  textureIndex = textureIndexIn;
  textureCoord = positionModelIn.xy + vec2(0.5, 0.5);
}