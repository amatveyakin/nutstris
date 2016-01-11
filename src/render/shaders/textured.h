#pragma once
#include <string>

namespace render {
namespace shaders {

std::string kTexturedVertexShader = R"(
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
 
const char* kTexturedFragmentShader = R"(
in vec3 positionModel;
in vec3 positionWorld;
in vec3 normalWorld;
in vec2 textureCoord;

uniform sampler2DArray gDiffuseMap;
uniform int gDiffuseMapLayer;

out vec3 color;

void main() {
  vec4 textureColor = texture(gDiffuseMap, vec3(textureCoord, gDiffuseMapLayer));
  SurfaceInfo v;
  v.position = positionWorld;
  v.normal   = normalize(normalWorld);
  v.diffuseColor  = textureColor;
  v.specularColor = 0.2 * textureColor;

  color = getLitColor(v, vec3(0, 0, 6.86));
}
)";

}
}
