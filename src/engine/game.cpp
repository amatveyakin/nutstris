#include "game.h"

#include <iostream>
#include <cstdio>

#include "engine/strings.h"


namespace engine {

void Game::init()
{
  checkInvariants();
  initPieces();
  initBonuses();
  for (int key = 0; key < kNumGlobalControls; ++key)
    nextGlobalKeyActivationTable[key] = Time::min();
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
    players[iPlayer].init(this, iPlayer);
}

void Game::newMatch()
{
  participants.clear();
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
    if (players[iPlayer].participates)
      participants.push_back(&players[iPlayer]);
}

void Game::newRound(Time currentTime__)
{
  currentTime = currentTime__;
  globalEffects = GlobalVisualEffects();
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
    players[iPlayer].active = players[iPlayer].participates;
  activePlayers = participants;
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
    if (players[iPlayer].participates)
      players[iPlayer].prepareForNewRound();
}

void Game::endRound()
{
  exit(0);
}

void Game::onGlobalKeyPress(GlobalControl key) {
  switch (key) {
  case GlobalControl::SayHi:
    std::cout << "Hi!" << std::endl;
    break;
  }
}

void Game::onTimer(Time currentTime__)
{
  currentTime = currentTime__;

  for (int key = 0; key < kNumGlobalControls; ++key)
  {
    if (Keyboard::isKeyPressed(globalControls[key]) &&
        (currentTime >= nextGlobalKeyActivationTable[key]))
    {
      onGlobalKeyPress(GlobalControl(key));
      nextGlobalKeyActivationTable[key] = currentTime + globalControlCooldown(GlobalControl(key));
    }
    else if (!Keyboard::isKeyPressed(globalControls[key]))
      nextGlobalKeyActivationTable[key]  = currentTime;
  }

  for (size_t iPlayer = 0; iPlayer < activePlayers.size(); ++iPlayer)
  {
    for (int key = 0; key < kNumPlayerControls; ++key)
    {
      if (Keyboard::isKeyPressed(activePlayers[iPlayer]->controls[key]) &&
          (currentTime >= activePlayers[iPlayer]->nextKeyActivationTable[key]))
      {
        activePlayers[iPlayer]->onKeyPress(PlayerControl(key));
        activePlayers[iPlayer]->nextKeyActivationTable[key] = currentTime + playerControlCooldown(PlayerControl(key));
      }
      else if (!Keyboard::isKeyPressed(activePlayers[iPlayer]->controls[key]))
        activePlayers[iPlayer]->nextKeyActivationTable[key] = currentTime;
    }
  }

  for (size_t iPlayer = 0; iPlayer < activePlayers.size(); ++iPlayer)
    activePlayers[iPlayer]->onTimer();
}


void Game::checkInvariants() const {
  assert(PIECE_FORCED_LOWERING_ANIMATION_TIME <= playerControlCooldown(PlayerControl::Down));
  assert(BONUS_LANTERN_MAX_SPEED >= 1.0 / DROPPING_PIECE_LOWERING_TIME);
}

void Game::initPieces()
{
  pieceTemplates = loadPieces();

  randomPieceTable.clear();
  for (size_t iPiece = 0; iPiece < pieceTemplates.size(); ++iPiece)
    for (int i = 0; i < pieceTemplates[iPiece].chance; ++i)
      randomPieceTable.push_back(iPiece);
}

void Game::initBonuses()
{
  for (Bonus bonus : ForEachBonus()) {
    for (int i = 0; i < bonusFrequency(bonus); ++i)
      randomBonusTable.push_back(bonus);
  }
}

}  // namespace engine
