#include "sound.h"

#include <cassert>
#include <memory>
#include <queue>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "engine/strings.h"
#include "util/file.h"


namespace engine {

static constexpr int kMaxSimultaneousSounds = 256;

class SoundStorage {
public:
  SoundStorage() {
    for (int soundId = 0; soundId < kNumSounds; ++soundId) {
      buffers_[soundId].loadFromFile(util::joinPath(kSoundPath, getSoundName(Sound(soundId)) + ".wav"));
    }
  }

  void playSound(Sound sound) {
    while (sounds_.size() >= kMaxSimultaneousSounds - 1)
      sounds_.pop();
    auto newSound = std::make_unique<sf::Sound>(buffers_[static_cast<int>(sound)]);
    newSound->play();
    sounds_.push(std::move(newSound));
  }

private:
  std::array<sf::SoundBuffer, kNumSounds> buffers_;
  std::queue<std::unique_ptr<sf::Sound>> sounds_;
};

std::unique_ptr<SoundStorage> soundStorage;


void initSounds() {
  assert(!soundStorage);
  soundStorage = std::make_unique<SoundStorage>();
}

std::string getSoundName(Sound sound) {
  switch (sound) {
    case Sound::LineCompleted:  return "LineCompleted";
  }
  abort();
}

void playSound(Sound sound) {
  assert(soundStorage);
  soundStorage->playSound(sound);
}

}  // namespace engine
