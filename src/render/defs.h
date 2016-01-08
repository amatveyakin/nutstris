#pragma once

#include "engine/defs.h"

namespace render {

const float MAX_WORLD_FIELD_WIDTH  = 2.0f;
const float MAX_WORLD_FIELD_HEIGHT = 4.0f;
const float CUBE_SCALE = std::min (MAX_WORLD_FIELD_WIDTH / engine::FIELD_WIDTH, MAX_WORLD_FIELD_HEIGHT / engine::FIELD_HEIGHT);
const float FIELD_INDENT_LEFT = 0.0f;
const float FIELD_INDENT_RIGHT = 0.00f;
const float FIELD_INDENT_BOTTOM = 2 * CUBE_SCALE;
const float FIELD_INDENT_TOP = 4 * CUBE_SCALE;
const float HUD_HEIGHT = 2 * CUBE_SCALE;
const float VP_WORLD_WIDTH  = FIELD_INDENT_LEFT + MAX_WORLD_FIELD_WIDTH + FIELD_INDENT_RIGHT;
const float VP_WORLD_HEIGHT = HUD_HEIGHT + FIELD_INDENT_BOTTOM + MAX_WORLD_FIELD_HEIGHT + FIELD_INDENT_TOP;
const float VP_ASPECT = VP_WORLD_WIDTH / VP_WORLD_HEIGHT;
const float ANGLE_FOV_Y = 0.25f * math::kPi; //Now you CAN change it!
const float SEARCHLIGHT_RADIUS = 5 * CUBE_SCALE;
const float SEARCH_BETA = math::kPi / 50.0f;
const float DIST_TO_SEARCHLIGHT = SEARCHLIGHT_RADIUS / tan(SEARCH_BETA);
const float EYE_TO_FIELD = 0.5f * VP_WORLD_HEIGHT / tan(ANGLE_FOV_Y / 2.0f);

}
