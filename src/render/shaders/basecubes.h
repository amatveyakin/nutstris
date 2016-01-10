#pragma once
#include <string>

namespace render {
namespace shaders {

std::string kBaseCubesVertexShader = R"(
layout(location = 0) in vec3 positionModelIn;
layout(location = 1) in vec3 normalModelIn;
layout(location = 2) in int  textureIndexIn;
layout(location = 4) in mat4 modelToWorldIn;
layout(location = 8) in vec4 diffuseColorIn;
layout(location = 9) in vec4 specularColorIn;

uniform mat4 gVP;
uniform mat4 gGlobalRotation;
uniform float gWaveProgress;

out vec3 positionModel;
out vec3 positionWorld;
out vec3 normalWorld;
out vec4 diffuseColor;
out vec4 specularColor;
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
  specularColor = specularColorIn;
  textureIndex = textureIndexIn;
  textureCoord = positionModelIn.xy + vec2(0.5, 0.5);
}
)";
 
std::string kBaseCubesFragmentShader = R"(
in vec3 positionModel;
in vec3 positionWorld;
in vec3 normalWorld;
in vec4 diffuseColor;
in vec4 specularColor;
flat in int textureIndex;
in vec2 textureCoord;

uniform sampler2DArray gBonusesTextureArray;
uniform vec4 gClipPlane;

out vec3 color;

void main() {
  if (dot(gClipPlane, vec4(positionModel, 1)) > 0)
    discard;
  vec4 textureColor = texture(gBonusesTextureArray, vec3(textureCoord, textureIndex));
  SurfaceInfo v;
  v.position = positionWorld;
  v.normal   = normalize(normalWorld);
 
  v.diffuseColor  = (1 - textureColor.a) * diffuseColor + textureColor.a * textureColor;
  v.specularColor = (1 - textureColor.a) * specularColor + textureColor.a * (textureColor + vec4(1.0, 1.0, 1.0, 1.0)) / 2;

  color = getLitColor(v, vec3(0, 0, 6.86));
}
)";

}
}

