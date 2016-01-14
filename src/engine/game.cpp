#include "game.h"

#include <fstream>
#include <iostream>
#include <cstdio>

#include "engine/io/sound.h"
#include "engine/strings.h"


namespace engine {

namespace {
template<typename ElementT, typename FuncT>
auto transformedVector(const std::vector<ElementT>& vector, FuncT func) {
  std::vector<std::result_of_t<FuncT(ElementT)>> result;
  std::transform(vector.begin(), vector.end(), std::back_inserter(result), func);
  return result;
}
}  // namespace

//================================= Game Round =================================

GameRound::GameRound(const Game* game__, Time currentTime__)
  : game_(game__)
  , currentTime_(currentTime__)
{
  for (auto& playerInfo : game_->playerInfos()) {
    if (playerInfo->participates())
      players_.push_back(std::make_unique<Player>(playerInfo, this));
  }
  activePlayers_ = players();
  for (const auto& player : players_)
    player->cycleVictim();
  for (auto& activationTime : nextGlobalKeyActivationTable_)
    activationTime = Time::min();
}

const GameBasics& GameRound::basics() const {
  return game_->basics();
}

std::vector<const Player*> GameRound::players() const {
  return transformedVector(players_, [](auto&& player) -> const Player* { return player.get(); });
}

std::vector<Player*> GameRound::players() {
  return transformedVector(players_, [](auto&& player) -> Player* { return player.get(); });
}

std::vector<const Player*> GameRound::activePlayers() const {
  return transformedVector(activePlayers_, [](auto&& player) -> const Player* { return player; });
}

std::vector<Player*> GameRound::activePlayers() {
  return activePlayers_;
}

const Player* GameRound::playerById(int id) const {
  return doGetPlayerById(id);
}

Player* GameRound::playerById(int id) {
  return doGetPlayerById(id);
}

void GameRound::deactivatePlayer(int id) {
  for (auto p : activePlayers_) {
    if (p->victimId == id) {
      p->cycleVictim();
      if ((activePlayers_.size() >= 2) && (p->victimId == p->id()))
        p->cycleVictim();
    }
  }

  for (auto it = activePlayers_.begin(); it != activePlayers_.end(); ++it) {
    if ((*it)->id() == id) {
      activePlayers_.erase(it);
      break;
    }
  }

  if (activePlayers_.empty())
    setOver(true);
}

bool GameRound::over() const {
  return over_;
}

void GameRound::setOver(bool v) {
  over_ = v;
}

Time GameRound::currentTime() const {
  return currentTime_;
}

const GlobalVisualEffects& GameRound::globalEffects() const {
  return globalEffects_;
}

GlobalVisualEffects& GameRound::globalEffects() {
  return globalEffects_;
}

void GameRound::onTimer(Time currentTime__) {
  currentTime_ = currentTime__;

  for (int key = 0; key < kNumGlobalControls; ++key) {
    if (Keyboard::isKeyPressed(game_->globalControls()[key]) &&
        (currentTime_ >= nextGlobalKeyActivationTable_[key])) {
      onGlobalKeyPress(GlobalControl(key));
      nextGlobalKeyActivationTable_[key] = currentTime_ + globalControlCooldown(GlobalControl(key));
    }
    else if (!Keyboard::isKeyPressed(game_->globalControls()[key]))
      nextGlobalKeyActivationTable_[key]  = currentTime_;
  }

  for (size_t iPlayer = 0; iPlayer < activePlayers_.size(); ++iPlayer)
    activePlayers_[iPlayer]->onTimer();
}

Player* GameRound::doGetPlayerById(int id) const {
  for (const auto& player : players_) {
    if (player->id() == id)
      return player.get();
  }
  return nullptr;
}

void GameRound::onGlobalKeyPress(GlobalControl key) {
  switch (key) {
  case GlobalControl::SayHi:
    std::cout << "Hi!" << std::endl;
    break;
  }
}


//==================================== Game ====================================

Game::Game()
{
  checkInvariants();
  initBasics();
  initSounds();
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer) {
    playerInfos_.push_back(std::make_unique<PlayerInfo>(iPlayer));
    playerInfos_.back()->setName("Player " + std::to_string(iPlayer));
  }
}

std::vector<PlayerInfo*> Game::playerInfos() const {
  std::vector<PlayerInfo*> result;
  for (const auto& playerInfo : playerInfos_)
    result.push_back(playerInfo.get());
  return result;
}

const GameBasics& Game::basics() const {
  return basics_;
}

const GlobalControls& Game::globalControls() const {
  return globalControls_;
}

std::unique_ptr<GameRound> Game::newRound(Time currentTime) const {
  std::cout << "Starting new round." << std::endl;
  return std::make_unique<GameRound>(this, currentTime);
}

void Game::onRoundOver(const GameRound* /*round*/) {
  // TODO(Andrei): update winner score
}


void Game::checkInvariants() const {
  assert(PIECE_FORCED_LOWERING_ANIMATION_TIME <= playerControlCooldown(PlayerControl::Down));
  assert(BONUS_LANTERN_MAX_SPEED >= 1.0 / DROPPING_PIECE_LOWERING_TIME);
}

void Game::initBasics() {
  basics_.pieceTemplates = loadPieces();

  for (size_t iPiece = 0; iPiece < basics_.pieceTemplates.size(); ++iPiece) {
    for (int i = 0; i < basics_.pieceTemplates[iPiece].chance; ++i)
      basics_.randomPieceTable.push_back(iPiece);
  }

  for (Bonus bonus : ForEachBonus()) {
    for (int i = 0; i < bonusFrequency(bonus); ++i)
      basics_.randomBonusTable.push_back(bonus);
  }
}

void Game::loadSettings() {
  // TODO(Andrei): save/load global keys
  globalControls_[SayHi] = Keyboard::F1;

  std::ifstream settingsFile(kSettingsFile);
  if (!settingsFile.good()) {
    loadDefaultSettings();
    return;
  }
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer) {
    int tmp;
    settingsFile >> tmp;
    switch (tmp) {
    case 0:
      playerInfos_[iPlayer]->setParticipates(false);
      break;
    case 1:
      playerInfos_[iPlayer]->setParticipates(true);
      break;
    default:
      throw std::runtime_error(ERR_FILE_CORRUPTED);
    }
    PlayerControls controls;
    for (int iKey = 0; iKey < kNumPlayerControls; ++iKey) {
      int keyValue = 0;
      settingsFile >> keyValue;
      controls[iKey] = static_cast<Keyboard::Key>(keyValue);
    }
    playerInfos_[iPlayer]->setControls(controls);
  }
}

void Game::saveSettings() {
  std::ofstream settingsFile(kSettingsFile);
  if (!settingsFile.good())
    throw std::runtime_error(ERR_FILE_WRITE_ERROR);   // TODO: format
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
  {
    settingsFile << (playerInfos_[iPlayer]->participates() ? 1 : 0) << " ";
    for (int key = 0; key < kNumPlayerControls; ++key)
      settingsFile << playerInfos_[iPlayer]->controls()[key] << " ";
  }
}

void Game::loadDefaultSettings() {
  assert(playerInfos_.size() == 4);
  PlayerControls controls;

  controls[Left] = Keyboard::A;
  controls[Right] = Keyboard::D;
  controls[RotateCCW] = Keyboard::W;
  controls[RotateCW] = Keyboard::E;
  controls[Down] = Keyboard::S;
  controls[Drop] = Keyboard::Tab;
  controls[NextVictim] = Keyboard::Q;
  playerInfos_[0]->setParticipates(true);
  playerInfos_[0]->setControls(controls);

  controls[Left] = Keyboard::H;
  controls[Right] = Keyboard::K;
  controls[RotateCCW] = Keyboard::U;
  controls[RotateCW] = Keyboard::I;
  controls[Down] = Keyboard::J;
  controls[Drop] = Keyboard::Space;
  controls[NextVictim] = Keyboard::L;
  playerInfos_[1]->setParticipates(false);
  playerInfos_[1]->setControls(controls);

  controls[Left] = Keyboard::Left;
  controls[Right] = Keyboard::Right;
  controls[RotateCCW] = Keyboard::Up;
  controls[RotateCW] = Keyboard::Delete;
  controls[Down] = Keyboard::Down;
  controls[Drop] = Keyboard::RShift;
  controls[NextVictim] = Keyboard::RControl;
  playerInfos_[2]->setParticipates(true);
  playerInfos_[2]->setControls(controls);

  controls[Left] = Keyboard::Numpad4;
  controls[Right] = Keyboard::Numpad6;
  controls[RotateCCW] = Keyboard::Numpad8;
  controls[RotateCW] = Keyboard::Numpad9;
  controls[Down] = Keyboard::Numpad5;
  controls[Drop] = Keyboard::Numpad0;
  controls[NextVictim] = Keyboard::Add;
  playerInfos_[3]->setParticipates(false);
  playerInfos_[3]->setControls(controls);
}

}  // namespace engine
