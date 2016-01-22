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
layout(location = 10) in float cubeSmoothnessIn;

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
  positionModel = 0.5 * mix(sign(positionModelIn), positionModelIn, mix(0.55, 0.35, cubeSmoothnessIn));

  positionWorld = (modelToWorldIn * vec4(positionModel, 1)).xyz;
  positionWorld.x += sin(gWaveProgress) * sin(4 * positionWorld.y)  * 0.2 * cos(abs(positionWorld.x / 0.7));
  positionWorld = (gGlobalRotation * vec4(positionWorld, 1)).xyz;

  normalWorld = (gGlobalRotation * modelToWorldIn * vec4(normalModelIn, 0)).xyz;

  gl_Position = gVP * vec4(positionWorld, 1);

  diffuseColor = diffuseColorIn;
  specularColor = specularColorIn;
  textureIndex = textureIndexIn;
  textureCoord = positionModel.xy + vec2(0.5, 0.5);
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
uniform vec4 gHintAreaClipPlane;
uniform float gEdgeOpacity;
uniform float gFaceOpacity;
uniform float gTruncatedCubesProgress;

out vec4 color;

void main() {
  if (dot(gClipPlane, vec4(positionModel, 1)) > 0)
    discard;
 
  float t = mix(0.92, 0.15, sqrt(gTruncatedCubesProgress)); //some magic constants
  if   (+ pow(abs( positionModel.x - t), 4)
        + pow(abs( positionModel.y - t), 4)
        + pow(abs(-positionModel.z - t), 4) < 0.2) discard;

  float opacity = 1.0;

  if (dot(gHintAreaClipPlane, vec4(positionWorld, 1)) > 0) {
    vec3 distToGrid = (mod(positionModel + 0.5 - 0.06, 1.0f) - 0.9); //another VERY MAGIC constants
    if ((distToGrid.x > 0) || (distToGrid.y > 0) || (distToGrid.z > 0))
      opacity = gFaceOpacity;
    else
      opacity = gEdgeOpacity;
  }
    
  vec4 textureColor = texture(gBonusesTextureArray, vec3(textureCoord, textureIndex));
  SurfaceInfo v;
  v.position = positionWorld;
  v.normal   = normalize(normalWorld);
  if (v.normal.z < 0)
    v.normal *= -1;
 
  v.diffuseColor  = mix(diffuseColor,  textureColor, textureColor.a);
  v.specularColor = mix(specularColor, (textureColor + vec4(1.0, 1.0, 1.0, 1.0)) / 2, textureColor.a);

  color = vec4(getLitColor(v, vec3(0, 0, 6.86)), opacity);
}
)";

}
}

