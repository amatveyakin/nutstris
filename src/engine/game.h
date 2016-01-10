#pragma once

#include <array>
#include <memory>
#include <vector>

#include "engine/player.h"


namespace engine {

class Game;

//================================= GameBasics =================================

struct GameBasics {
  std::vector<PieceTemplate> pieceTemplates;
  std::vector<int> randomPieceTable;
  std::vector<Bonus> randomBonusTable;
};


//================================= GameRound ==================================

class GameRound {
public:
  GameRound(const Game* game__, Time currentTime__);

  const GameBasics& basics() const;

  std::vector<Player*> players() const;  // players participating in this round; fixed size vector
  std::vector<Player*> activePlayers() const;  // players that are still alive; this vector can shrink
  Player* playerById(int id) const;

  void deactivatePlayer(int id);

  bool over() const;
  void setOver(bool v);

  Time currentTime() const;

  const GlobalVisualEffects& globalEffects() const;
  GlobalVisualEffects& globalEffects();

  void onTimer(Time currentTime);

private:
  void onGlobalKeyPress(GlobalControl key);

private:
  const Game* game_ = nullptr;
  std::vector<std::unique_ptr<Player>> players_;
  std::vector<Player*> activePlayers_;

  bool over_ = false;
  Time currentTime_ = Time::zero();  // TODO(Andrei): Try to pass it through the call stack instead of storing here
  GlobalVisualEffects globalEffects_;
  std::array<Time, kNumGlobalControls> nextGlobalKeyActivationTable_;
};


//==================================== Game ====================================

class Game {
public:
  Game();

  std::vector<PlayerInfo*> playerInfos() const;
  const GameBasics& basics() const;
  const GlobalControls& globalControls() const;

  std::unique_ptr<GameRound> newRound(Time currentTime) const;
  void onRoundOver(const GameRound* round);

  void loadSettings();
  void saveSettings();
  void loadDefaultSettings();

private:
  void checkInvariants() const;
  void initBasics();

private:
  std::vector<std::unique_ptr<PlayerInfo>> playerInfos_;
  GameBasics basics_;
  GlobalControls globalControls_;
};

}  // namespace engine
