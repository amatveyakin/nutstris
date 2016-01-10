#ifndef BASE_CUBES_H
#define BASE_CUBES_H

namespace render {
namespace shaders {

const char* cBaseCubesVertexShader = R"(
#version 330 core

layout(location = 0) in vec3 positionModelIn;
layout(location = 1) in vec3 normalModelIn;
layout(location = 2) in int  textureIndexIn;
layout(location = 4) in mat4 modelToWorldIn;
layout(location = 8) in vec3 diffuseColorIn;

uniform mat4 gVP;
uniform mat4 gGlobalRotation;
uniform float gWaveProgress;

out vec3 positionModel;
out vec3 positionWorld;
out vec3 normalWorld;
out vec3 diffuseColor;
flat out int textureIndex;
out vec2 textureCoord;

void main() {
  positionModel = positionModelIn;

  positionWorld = (modelToWorldIn * vec4(positionModelIn, 1)).xyz;
  positionWorld.x += sin(gWaveProgress) * sin(4 * positionWorld.y)  * 0.2 * cos(abs(positionWorld.x / 0.7));
  positionWorld = (gGlobalRotation * vec4(positionWorld, 1)).xyz;

  normalWorld = (gGlobalRotation * modelToWorldIn * vec4(normalModelIn, 0)).xyz;

  gl_Position = gVP * vec4(positionWorld, 1);

  diffuseColor = diffuseColorIn;
  textureIndex = textureIndexIn;
  textureCoord = positionModelIn.xy + vec2(0.5, 0.5);
}
)";
 
const char* cBaseCubesFragmentShader = R"(
#version 330 core

in vec3 positionModel;
in vec3 positionWorld;
in vec3 normalWorld;
in vec3 diffuseColor;
flat in int textureIndex;
in vec2 textureCoord;

uniform sampler2DArray gBonusesTextureArray;
uniform vec4 gClipPlane;

layout(std140) uniform LightsSettings {
  vec3 direction;
} lightSettings;

out vec3 color;

void main() {
  if (dot(gClipPlane, vec4(positionModel, 1)) > 0)
    discard;
  vec4 textureColor = texture(gBonusesTextureArray, vec3(textureCoord, textureIndex));
  color = (textureColor.rgb * textureColor.a + (1.0 - textureColor.a) * diffuseColor)
        * (0.6 * max(0.0, dot(normalize(normalWorld), lightSettings.direction)) + 0.2);
}
)";

}
}




#endif//BASE_CUBES_H
