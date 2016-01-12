#pragma once

#include <string>

#include "engine/units.h"


namespace engine {

static constexpr int kMaxSimultaneousSounds = 256;
static constexpr int kNumSounds = 1;

enum class Sound {
  LineCompleted,
};

extern const Time kLineCompletedSoundRepeteationInterval;

void initSounds();
std::string getSoundName(Sound sound);
void playSound(Sound sound);
void playSoundRepeatedly(Sound sound, int nTimes, Time interval);

}  // namespace engine
