#ifndef SHADERS_TEXTURED_H
#define SHADERS_TEXTURED_H

namespace render {
namespace shaders {

const char* cTexturedVertexShader = R"(
#version 330 core

layout(location = 0) in vec3 positionModelIn;
layout(location = 1) in vec3 normalModelIn;
layout(location = 2) in vec2 textureCoordIn;

uniform mat4 gWorld;
uniform mat4 gVP;

out vec3 positionModel;
out vec3 positionWorld;
out vec3 normalWorld;
out vec2 textureCoord;

void main() {
  positionModel = positionModelIn;

  positionWorld = (gWorld * vec4(positionModelIn, 1)).xyz;
  normalWorld = (gWorld * vec4(normalModelIn, 0)).xyz;

  gl_Position = gVP * vec4(positionWorld, 1);

  textureCoord = textureCoordIn;
}
)";
 
const char* cTexturedFragmentShader = R"(
#version 330 core

in vec3 positionModel;
in vec3 positionWorld;
in vec3 normalWorld;
in vec2 textureCoord;

uniform sampler2D gDiffuseMap;

out vec3 color;

void main() {
  vec4 textureColor = texture(gDiffuseMap, textureCoord);
  textureColor = vec4(0.8, 0.0, 0.0, 1.0);
  color = textureColor.rgb * (0.6 * max(0.0, normalize(normalWorld).z) + 0.2);
  color = textureColor.rgb;
}
)";

}
}

#endif//SHADERS_TEXTURED_H