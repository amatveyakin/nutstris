#pragma once

#include <array>
#include <vector>

#include "engine/player.h"


namespace engine {

class Game
{
public:
  std::array<Player, MAX_PLAYERS> players;
  std::vector<Player*> participants;
  std::vector<Player*> activePlayers;

  std::vector<PieceTemplate> pieceTemplates;
  std::vector<int>   randomPieceTable;

  std::vector<Bonus> randomBonusTable;

  Time          currentTime;
  GlobalVisualEffects   globalEffects;

  std::array<Time, kNumGlobalControls> nextGlobalKeyActivationTable;
  GlobalControls globalControls;

  void          init();

  void          newMatch();
  void          newRound(Time currentTime__);
  void          endRound();

  void          onGlobalKeyPress(GlobalControl key);
  void          onTimer(Time currentTime);

private:
  void          checkInvariants() const;

  void          initPieces();
  void          initBonuses();
};

}  // namespace engine
