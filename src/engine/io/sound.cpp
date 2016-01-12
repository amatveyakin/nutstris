#include "sound.h"

#include <cassert>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "engine/strings.h"
#include "util/file.h"


namespace engine {

const Time kLineCompletedSoundRepeteationInterval = 0.07s;

class SoundEngine {
public:
  SoundEngine() {
    for (int soundId = 0; soundId < kNumSounds; ++soundId) {
      buffers_[soundId].loadFromFile(util::joinPath(kSoundPath, getSoundName(Sound(soundId)) + ".wav"));
    }
    delayed_sounds_thread_ = std::thread([this](){
      while (true) {
        Time now = std::chrono::steady_clock::now().time_since_epoch();
        {
          std::lock_guard<std::mutex> lock(delayed_sounds_mutex_);
          while (!delayed_sounds_.empty() && now > delayed_sounds_.begin()->first) {
            playSound(delayed_sounds_.begin()->second);
            delayed_sounds_.erase(delayed_sounds_.begin());
          }
          if (!delayed_sounds_thread_running_)
            return;
        }
        std::this_thread::sleep_for(1ms);
      }
    });
  }

  ~SoundEngine() {
    std::lock_guard<std::mutex> lock(delayed_sounds_mutex_);
    delayed_sounds_thread_running_ = false;
    delayed_sounds_thread_.join();
  }

  void playSound(Sound sound) {
    while (players_.size() >= kMaxSimultaneousSounds - 1)
      players_.pop();
    auto newSound = std::make_unique<sf::Sound>(buffers_[static_cast<int>(sound)]);
    newSound->setVolume(50);
    newSound->play();
    players_.push(std::move(newSound));
  }

  void playSoundDelayed(Sound sound, Time delay) {
    Time now = std::chrono::steady_clock::now().time_since_epoch();
    std::lock_guard<std::mutex> lock(delayed_sounds_mutex_);
    delayed_sounds_.emplace(now + delay, sound);
  }

private:
  std::array<sf::SoundBuffer, kNumSounds> buffers_;
  std::queue<std::unique_ptr<sf::Sound>> players_;

  bool delayed_sounds_thread_running_ = true;
  std::multimap<Time, Sound> delayed_sounds_;
  std::mutex delayed_sounds_mutex_;
  std::thread delayed_sounds_thread_;
};

std::unique_ptr<SoundEngine> soundEngine;


void initSounds() {
  assert(!soundEngine);
  soundEngine = std::make_unique<SoundEngine>();
}

std::string getSoundName(Sound sound) {
  switch (sound) {
    case Sound::LineCompleted:  return "LineCompleted";
    case Sound::BonusTaken:     return "BonusTaken";
  }
  abort();
}

void playSound(Sound sound) {
  assert(soundEngine);
  soundEngine->playSound(sound);
}

void playSoundRepeatedly(Sound sound, int nTimes, Time interval) {
  assert(soundEngine);
  playSound(sound);
  for (int i = 1; i < nTimes; ++i)
    soundEngine->playSoundDelayed(sound, interval * i);
}

}  // namespace engine
