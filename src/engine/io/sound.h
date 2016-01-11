#pragma once

#include <string>


namespace engine {

static constexpr int kNumSounds = 1;

enum class Sound {
  LineCompleted,
};

void initSounds();
std::string getSoundName(Sound sound);
void playSound(Sound sound);

}  // namespace engine
