#pragma once

#include <string>

#include "engine/bonus.h"
#include "engine/defs.h"
#include "engine/input/control.h"
#include "util/file.h"


namespace engine {

//=============================== Resource files ===============================

// TODO: add ..._NAME to file name constants

const std::string RECOURCE_FOLDER = "resources";
const std::string PIECE_TEMPLATES_FILE = util::joinPath(RECOURCE_FOLDER, "Pieces");
const std::string SETTINGS_FILE = "Settings";
const std::string ACCOUNTS_FILE = "Accounts";

const std::string TEXTURES_FOLDER = util::joinPath(RECOURCE_FOLDER, "Textures");
const std::string BONUS_IMAGES_FOLDER = util::joinPath(TEXTURES_FOLDER, "Bonuses");


//=============================== Error messages ===============================

const std::string  ERR_INTERNAL_ERROR = "Internal error: \"%s\"";
const std::string  ERR_FILE_NOT_FOUND = "File not found: \"%s\".";
const std::string  ERR_FILE_CORRUPTED = "File is corrupt: \"%s\".";
const std::string  ERR_FILE_READ_ERROR = "Cannot read file: \"%s\".";
const std::string  ERR_FILE_WRITE_ERROR = "Cannot write file: \"%s\".";


//==================================== Misc ====================================

inline std::string getBonusName(Bonus bonus) {
  switch (bonus) {
    case Bonus::None:             return "NoBonus";
    case Bonus::EnlargeHintQueue: return "EnlargeHintQueue";
    case Bonus::PieceTheft:       return "PieceTheft";
    case Bonus::Heal:             return "Heal";
    case Bonus::SlowDown:         return "SlowDown";
    case Bonus::ClearField:       return "ClearField";
    case Bonus::FlippedScreen:    return "FlippedScreen";
    case Bonus::RotatingScreen:   return "RotatingScreen";
    case Bonus::Wave:             return "Wave";
    case Bonus::Lantern:          return "Lantern";
    case Bonus::CrazyPieces:      return "CrazyPieces";
    case Bonus::TruncatedBlocks:  return "TruncatedBlocks";
    case Bonus::NoHint:           return "NoHint";
    case Bonus::SpeedUp:          return "SpeedUp";
  }
  abort();
};

inline std::string getPlayerControlName(PlayerControl key) {
  switch (key) {
    case PlayerControl::Left:         return "Left";
    case PlayerControl::Right:        return "Right";
    case PlayerControl::RotateCCW:    return "Rotate CCW";
    case PlayerControl::RotateCW:     return "Rotate CW";
    case PlayerControl::Down:         return "Down";
    case PlayerControl::Drop:         return "Drop";
    case PlayerControl::NextVictim:   return "Next Victim";
  }
  abort();
};

}  // namespace engine
