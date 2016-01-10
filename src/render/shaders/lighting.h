#pragma once
#include <string>

namespace render {
namespace shaders {

std::string kLightingHelper = R"(
std140 uniform LightsSettings {
  vec4 position;
  vec4 direction;
  vec4 ambientColor;
  vec4 diffuseColor;
  vec4 specularColor;
  vec3 att;
  float spotPower;
  float range;
  int   lightType;  //0 - off, 1 - parallel, 2 - point, 3 - spot 
  float brightness;
} lightSettings;

struct SurfaceInfo {
  vec3 position;
  vec3 normal;
  vec4 diffuseColor;
  vec4 specularColor;
};

float discreetSpecFactor(float specFactor) {
  if (specFactor >= 0.0f && specFactor < 0.1f) return 0.0f;
  if (specFactor >= 0.1f && specFactor < 0.8f) return 0.5f;
  if (specFactor >= 0.8f && specFactor < 1.0f) return 0.8f;
  return specFactor;
}

float discreetDiffuseFactor(float diffuseFactor) {
  if (diffuseFactor < 0.0f)                         return 0.1f;
  if (diffuseFactor >= 0.0f && diffuseFactor < 0.5f) return 0.3f;
  if (diffuseFactor >= 0.5f && diffuseFactor < 1.0f) return 0.66f;
  return diffuseFactor;
}

float distribution(int dx, int dy) {
  return 1.0 / (1 + dx * dx + dy * dy);
}

vec3 parallelLight(SurfaceInfo v, vec3 eyePos) {
  vec4 litColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

  // The light vector aims opposite the direction the light rays travel.
  vec3 lightVec = -normalize(lightSettings.direction.xyz);

  // Add the ambient term.
  litColor += v.diffuseColor * lightSettings.brightness * lightSettings.ambientColor;

  // Add diffuse and specular term, provided the surface is in
  // the line of sight of the light.

  float diffuseFactor = dot(lightVec, v.normal);
  if (diffuseFactor > 0.0f) {
    float specPower = max(v.specularColor.a, 1.0f);
    vec3 toEye = normalize(eyePos - v.position);
    vec3 R = reflect(-lightVec, v.normal);
    float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

    // diffuse and specular terms
    litColor += diffuseFactor * v.diffuseColor  * lightSettings.brightness * lightSettings.diffuseColor;
    litColor += specFactor    * v.specularColor * lightSettings.brightness * lightSettings.specularColor;
  }

  return litColor.rgb;
}

vec3 pointLight(SurfaceInfo v, vec3 eyePos) {
  vec4 litColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

  // The vector from the surface to the light.
  vec3 lightVec = lightSettings.position.xyz - v.position;

  // The distance from surface to light.
  float d = length(lightVec);

  if (d > lightSettings.range) 
    return vec3(0.0f, 0.0f, 0.0f);

  // Normalize the light vector.
  lightVec /= d;

  // Add the ambient light term.
  litColor += v.diffuseColor * lightSettings.brightness * lightSettings.ambientColor;

  // Add diffuse and specular term, provided the surface is in
  // the line of site of the light.

  float diffuseFactor = dot(lightVec, v.normal);
  if (diffuseFactor > 0.0f) {
    float specPower = max(v.specularColor.a, 1.0f);
    vec3 toEye = normalize(eyePos - v.position);
    vec3 R = reflect(-lightVec, v.normal);
    float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

    // diffuse and specular terms
    litColor += diffuseFactor * v.diffuseColor * lightSettings.brightness * lightSettings.diffuseColor;
    litColor += specFactor * v.specularColor * lightSettings.brightness * lightSettings.specularColor;
  }

  // attenuate
  return litColor.rgb / dot(lightSettings.att, vec3(1.0f, d, d*d));
}

vec3 spotlight(SurfaceInfo v, vec3 eyePos) {
  vec3 litColor = pointLight(v, eyePos);

  // The vector from the surface to the light.
  vec3 lightVec = normalize(lightSettings.position.xyz - v.position);

  float s = pow(max(dot(-lightVec, normalize(lightSettings.direction.xyz)), 0.0f), lightSettings.spotPower);

  // Scale color by spotlight factor.
  return litColor * s;
}

vec3 getLitColor(SurfaceInfo v, vec3 eyePos) {
  if (lightSettings.brightness > 0) {
    if (lightSettings.lightType == 1) // Parallel
      return parallelLight(v, eyePos);
    else if (lightSettings.lightType == 2) // Point
      return pointLight(v, eyePos);
    else if (lightSettings.lightType == 3)// Spot
      return spotlight(v, eyePos);
  }
  return vec3(0.0f, 0.0f, 0.0f);
}
)";

}
}