#include "engine/settings.h"

#include <fstream>
#include <cstdio>

#include "engine/engine.h"
#include "engine/strings.h"
#include "util/file.h"


namespace engine {

void setGlobalKeys(Game* game) {
  // TODO(Andrei): save/load global keys
  game->globalControls[SayHi] = Keyboard::F1;
}

void loadSettings(Game* game) {
  setGlobalKeys(game);
  std::ifstream settingsFile(SETTINGS_FILE);
  if (!settingsFile.good()) {
    loadDefaultSettings(game);
    return;
  }
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer) {
    int tmp;
    settingsFile >> tmp;
    switch (tmp) {
    case 0:
      game->players[iPlayer].participates = false;
      break;
    case 1:
      game->players[iPlayer].participates = true;
      break;
    default:
      throw std::runtime_error(ERR_FILE_CORRUPTED);
    }
    for (int iKey = 0; iKey < kNumPlayerControls; ++iKey) {
      int keyValue = 0;
      settingsFile >> keyValue;
      game->players[iPlayer].controls[iKey] = static_cast<Keyboard::Key>(keyValue);
    }
  }
}

void saveSettings(Game* game) {
  std::ofstream settingsFile(SETTINGS_FILE);
  if (!settingsFile.good())
    throw std::runtime_error(ERR_FILE_WRITE_ERROR);   // TODO: format
  for (int iPlayer = 0; iPlayer < MAX_PLAYERS; ++iPlayer)
  {
    settingsFile << (game->players[iPlayer].participates ? 1 : 0) << " ";
    for (int key = 0; key < kNumPlayerControls; ++key)
      settingsFile << game->players[iPlayer].controls[key] << " ";
  }
}

void loadDefaultSettings(Game* game) {
  game->players[0].participates = true;
  game->players[0].controls[Left] = Keyboard::A;
  game->players[0].controls[Right] = Keyboard::D;
  game->players[0].controls[RotateCCW] = Keyboard::W;
  game->players[0].controls[RotateCW] = Keyboard::E;
  game->players[0].controls[Down] = Keyboard::S;
  game->players[0].controls[Drop] = Keyboard::Tab;
  game->players[0].controls[NextVictim] = Keyboard::Q;

  game->players[1].participates = false;
  game->players[1].controls[Left] = Keyboard::H;
  game->players[1].controls[Right] = Keyboard::K;
  game->players[1].controls[RotateCCW] = Keyboard::U;
  game->players[1].controls[RotateCW] = Keyboard::I;
  game->players[1].controls[Down] = Keyboard::J;
  game->players[1].controls[Drop] = Keyboard::Space;
  game->players[1].controls[NextVictim] = Keyboard::L;

  game->players[2].participates = true;
  game->players[2].controls[Left] = Keyboard::Left;
  game->players[2].controls[Right] = Keyboard::Right;
  game->players[2].controls[RotateCCW] = Keyboard::Up;
  game->players[2].controls[RotateCW] = Keyboard::Delete;
  game->players[2].controls[Down] = Keyboard::Down;
  game->players[2].controls[Drop] = Keyboard::RShift;
  game->players[2].controls[NextVictim] = Keyboard::RControl;

  game->players[3].participates = false;
  game->players[3].controls[Left] = Keyboard::Numpad4;
  game->players[3].controls[Right] = Keyboard::Numpad6;
  game->players[3].controls[RotateCCW] = Keyboard::Numpad8;
  game->players[3].controls[RotateCW] = Keyboard::Numpad9;
  game->players[3].controls[Down] = Keyboard::Numpad5;
  game->players[3].controls[Drop] = Keyboard::Numpad0;
  game->players[3].controls[NextVictim] = Keyboard::Add;
}

}  // ENGINE_ENGINE_H
