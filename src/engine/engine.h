// TODO: add  #error  commands

// TODO: make more animations nonlinear

// TODO: change active player list

// TODO: use more throw's instead of assertions

// TODO: Implement  field.lock() / field.unlock():  the field may be made immutable for some time
//       add all events that what to change the field are delayed  (?)
// No, looks like it's better to check specific conditions before any change

// TODO: game speed up formula -- ?

#pragma once

#include <climits>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "engine/event.h"
#include "engine/keyboard.h"
#include "engine/visual.h"
#include "util/containers.h"
#include "util/file.h"


namespace engine {

//================================ Time & speed ================================

extern const float  STARTING_SPEED;
extern const float  ROUTINE_SPEED_UP_VALUE;
extern const Time   ROUTINE_SPEED_UP_INTERVAL;
extern const float  NORMAL_SPEED_LIMIT;
extern const float  ABSOLUTE_SPEED_LIMIT;

extern const Time   AUTO_LOWERING_TIME;
// Time necessary for a dropping piece to move one line down
extern const Time   DROPPING_PIECE_LOWERING_TIME;
extern const Time   LINE_DISAPPEAR_TIME;
extern const Time   LINE_COLLAPSE_TIME;

//extern const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME;
extern const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME;
extern const Time   PIECE_FORCED_LOWERING_ANIMATION_TIME;
extern const Time   LINE_COLLAPSE_ANIMATION_TIME;
extern const Time   PIECE_MOVING_ANIMATION_TIME;
extern const Time   PIECE_ROTATING_ANIMATION_TIME;

extern const Time   HINT_APPERAING_TIME;
extern const Time   HINT_MATERIALIZATION_TIME;
extern const Time   PLAYER_DYING_ANIMATION_TIME;


//================================== Keyboard ==================================

// TODO: find optimal value for KEY_REACTIVATION_TIMEs
extern const Time   MOVE_KEY_REACTIVATION_TIME;
extern const Time   ROTATE_KEY_REACTIVATION_TIME;
//extern const Time   DOWN_KEY_REACTIVATION_TIME;
extern const Time   DOWN_KEY_REACTIVATION_TIME;
extern const Time   DROP_KEY_REACTIVATION_TIME;
extern const Time   CHANGE_VICTIM_KEY_REACTIVATION_TIME;

extern const Time   PLAYER_KEY_REACTIVATION_TIME[];


static const int N_GLOBAL_KEYS = 1;

enum GlobalKey { };

extern const Time   GLOBAL_KEY_REACTIVATION_TIME[];


struct PlayerKeyList
{
  Keyboard::Key keyLeft;
  Keyboard::Key keyRight;
  Keyboard::Key keyRotateCCW;
  Keyboard::Key keyRotateCW;
  Keyboard::Key keyDown;
  Keyboard::Key keyDrop;
  Keyboard::Key keyNextVictim;
};

union Controls
{
  PlayerKeyList keyByName;
  Keyboard::Key keyArray[N_PLAYER_KEYS];
};


struct GlobalKeyList { };

union GlobalControls
{
  GlobalKeyList keyByName;
//  Keyboard::Key keyArray[N_GLOBAL_KEYS];
  Keyboard::Key keyArray[1];
};


//================================== Bonuses ===================================

#define SKIP_BUFFS          case Bonus::EnlargeHintQueue:  case Bonus::PieceTheft:

#define SKIP_KIND_SORCERIES case Bonus::Heal:  case Bonus::SlowDown:  case Bonus::ClearField:

#define SKIP_DEBUFFS        case Bonus::FlippedScreen:  case Bonus::CrazyPieces: \
                            case Bonus::TruncatedBlocks:  case Bonus::NoHint:

#define SKIP_EVIL_SORCERIES case Bonus::SpeedUp:  // case Bonus::FlipField:

#define SKIP_ENCHANTMENTS  SKIP_BUFFS  SKIP_DEBUFFS

#define SKIP_SORCERIES  SKIP_KIND_SORCERIES  SKIP_EVIL_SORCERIES

#define SKIP_ALL_BUT_SORCERIES  SKIP_ENCHANTMENTS  case Bonus::None:

#define SKIP_ALL_BUT_ENCHANTMENTS  SKIP_SORCERIES  case Bonus::None:

#define SKIP_ALL_BUT_BUFFS  SKIP_ALL_BUT_ENCHANTMENTS  SKIP_DEBUFFS

#define SKIP_ALL_BUT_DEBUFFS  SKIP_ALL_BUT_ENCHANTMENTS  SKIP_BUFFS



const int    BONUS_CHANCES[N_BONUSES] =
{
  0,  // Bonus::None
  0,  // Bonus::EnlargeHintQueue
  0,  // Bonus::PieceTheft
  10, // Bonus::Heal
  4,  // Bonus::SlowDown
  2,  // Bonus::ClearField
  4,  // Bonus::FlippedScreen
  3,  // Bonus::RotatingScreen
  4,  // Bonus::Wave
  3,  // Bonus::Lantern
  0,  // Bonus::CrazyPieces
  4,  // Bonus::TruncatedBlocks
  0,  // Bonus::NoHint
  4   // Bonus::SpeedUp
//  2   // Bonus::FlipField
};

const int    BONUS_ENLARGED_HINT_QUEUE_SIZE = 7;
const Time   BONUS_FADING_DURATION = 0.5s;

const float  BONUS_SPEED_UP_VALUE = 0.8f;
const float  BONUS_SLOW_DOWN_VALUE = 1.0f;

const Time   BONUS_FLIPPING_SCREEN_DURATION = 0.8s;
const Time   BONUS_ROTATING_SCREEN_PERIOD = 10.0s;
const Time   BONUS_WAVE_PERIOD = 2.0s;
const Time   BONUS_CLEAR_FIELD_DURATION = 1.0s;
const Time   BONUS_CUTTING_BLOCKS_DURATION = 0.5s;
const Time   BONUS_REMOVING_HINT_DURATION = 1.0s;
const Time   BONUS_LANTERN_FADING_TIME = 1.5s;
// const Time   BONUS_LANTERN_ANIMATION_TIME = PIECE_FORCED_LOWERING_ANIMATION_TIME;  // (?)
const Speed  BONUS_LANTERN_MAX_SPEED = 100.0f; // Requirements: BONUS_LANTERN_MAX_SPEED >= 1.0 / DROPPING_PIECE_LOWERING_TIME

const Time   MIN_BONUS_APPEAR_TIME = 4.0s;
const Time   MAX_BONUS_APPEAR_TIME = 6.0s;
const Time   MIN_BONUS_LIFE_TIME = 20.0s;
const Time   MAX_BONUS_LIFE_TIME = 25.0s;

/*const int    N_BONUS_CHOOSE_ATTEMPTS = 10;
const int    N_BONUS_GENERATION_ATTEMPTS = 5;
const float  BONUS_ONE_ROW_CHANCE = 0.3f;
const int    N_BONUS_ONE_ROW_ATTEMPTS = 2;*/

const int    N_BONUS_CHOOSE_ATTEMPTS = 10;
const int    N_BONUS_GENERATION_ATTEMPTS = 5;
const float  BONUS_ONE_ROW_CHANCE = 0.4f;
const int    N_BONUS_ONE_ROW_ATTEMPTS = 1;

const int    BONUS_HIGHEST_LINE_MAKING_CLEARING_USEFUL = FIELD_HEIGHT / 2;



const Bonus  FIRST_BUFF         = Bonus::EnlargeHintQueue;
const Bonus  LAST_BUFF          = Bonus::PieceTheft;

const Bonus  FIRST_DEBUFF       = Bonus::FlippedScreen;
const Bonus  LAST_DEBUFF        = Bonus::NoHint;

const Bonus  FIRST_KIND_BONUS   = FIRST_BUFF;
const Bonus  LAST_KIND_BONUS    = Bonus::ClearField;

const Bonus  FIRST_EVIL_BONUS   = FIRST_DEBUFF;
const Bonus  LAST_EVIl_BONUS    = Bonus::SpeedUp;

inline bool isKind(Bonus bonus)
{
  return (FIRST_KIND_BONUS <= bonus) && (bonus <= LAST_KIND_BONUS);
}

inline bool isEvil(Bonus bonus)
{
  return (FIRST_EVIL_BONUS <= bonus) && (bonus <= LAST_EVIl_BONUS);
}

inline bool isBuff(Bonus bonus)
{
  return (FIRST_BUFF <= bonus) && (bonus <= LAST_BUFF);
}

inline bool isDebuff(Bonus bonus)
{
  return (FIRST_DEBUFF <= bonus) && (bonus <= LAST_DEBUFF);
}

inline bool isEnchantment(Bonus bonus)  // (?) Is it necessary?
{
  return isBuff(bonus) || isDebuff(bonus);
}

typedef util::StronglyTypedBitSet<Bonus, FIRST_BUFF, LAST_BUFF> Buffs;

typedef util::StronglyTypedBitSet<Bonus, FIRST_DEBUFF, LAST_DEBUFF> Debuffs;



//=============================== Pieces & Field ===============================

const int    CENTRAL_PIECE_ROW = (MAX_PIECE_SIZE - 1) / 2; // is it necessary (?)
const int    CENTRAL_PIECE_COL = (MAX_PIECE_SIZE - 1) / 2; // is it necessary (?)
const int    N_PIECE_ROTATION_STATES = 4;

const int    SKY_HEIGHT = MAX_PIECE_SIZE;
// MAX_PIECE_SIZE / 2  is enough for  WALL_WIDTH  in most cases, but that's perfectly safe
const int    WALL_WIDTH = MAX_PIECE_SIZE - 1;  // TODO: may be, abadon walls and modify Field::operator(int, int)  ?

const int    BORDERED_FIELD_ROW_BEGIN = -WALL_WIDTH;
const int    BORDERED_FIELD_ROW_END   = FIELD_HEIGHT + SKY_HEIGHT;
const int    BORDERED_FIELD_COL_BEGIN = -WALL_WIDTH;
const int    BORDERED_FIELD_COL_END   = FIELD_WIDTH + WALL_WIDTH;

const int    NO_BLOCK_IMAGE = -1;
const int    NO_CHANGE = -2;



struct Bounds
{
  int bottom;
  int top;
  int left;
  int right;
};


struct BlockStructure
{
  std::vector<FieldCoords> blocks;
  Bounds bounds;

  void setBounds()
  {
    if (blocks.empty())
      throw std::runtime_error("Empty piece found");
    bounds.bottom = blocks[0].y();
    bounds.top = blocks[0].y();
    bounds.left = blocks[0].x();
    bounds.right = blocks[0].x();
    for (auto& b : blocks)
    {
      if (b.y() < bounds.bottom)
        bounds.bottom = b.y();
      if (b.y() > bounds.top)
        bounds.top = b.y();
      if (b.x() < bounds.left)
        bounds.left = b.x();
      if (b.x() > bounds.right)
        bounds.right = b.x();
    }
  }
};


struct PieceTemplate
{
  Color color;
  int chance;
  BlockStructure structure[N_PIECE_ROTATION_STATES];
};


struct Piece
{
  PieceTemplate* pieceTemplate;
  int rotationState;
  FieldCoords position;   // ``center'' coordinates

  void clear()
  {
    pieceTemplate = NULL;
  }

  bool empty()
  {
    return pieceTemplate == NULL;
  }

  Color color()
  {
    return pieceTemplate->color;
  }

  const BlockStructure& currentStructure()
  {
    return pieceTemplate->structure[rotationState];
  }

  size_t nBlocks()
  {
    return currentStructure().blocks.size();
  }

  FieldCoords relativeCoords(int index)
  {
    return currentStructure().blocks[index];
  }

  FieldCoords absoluteCoords(int index)
  {
    return relativeCoords(index) + position;
  }
};


struct FieldCell
{
  bool blocked;
  Color color;
  Bonus bonus;

  void clear()
  {
    blocked = false;
  }

  void setBlock(Color color__, Bonus bonus__ = Bonus::None)
  {
    blocked = true;
    color = color__;
    bonus = bonus__;
  }

  bool isFree() const
  {
    return !blocked;
  }

  bool isBlocked() const
  {
    return blocked;
  }
};


struct Field : public util::Fixed2DArray<FieldCell, -WALL_WIDTH, -WALL_WIDTH,
                                         FIELD_HEIGHT + SKY_HEIGHT, FIELD_WIDTH + WALL_WIDTH>
{
  Field();

  // TODO: swap and rename arguments
  FieldCell& operator()(int row, int col)
  {
    return Fixed2DArray<FieldCell, -WALL_WIDTH, -WALL_WIDTH,
                        FIELD_HEIGHT + SKY_HEIGHT, FIELD_WIDTH + WALL_WIDTH>::
                        operator()(row, col);
  }

  const FieldCell& operator()(int row, int col) const
  {
    return Fixed2DArray<FieldCell, -WALL_WIDTH, -WALL_WIDTH,
                        FIELD_HEIGHT + SKY_HEIGHT, FIELD_WIDTH + WALL_WIDTH>::
                        operator()(row, col);
  }

  FieldCell& operator()(FieldCoords position)
  {
    return operator()(position.y(), position.x());
  }

  const FieldCell& operator()(FieldCoords position) const
  {
    return operator()(position.y(), position.x());
  }

  void clear();
};


enum FallingPieceState { psAbsent, psNormal, psDropping };

const char   PIECE_TEMPLATE_FREE_CHAR  = '.';


struct FieldLocks
{
  bool isBeingCleared;

  void clear()
  {
    isBeingCleared = false;
  }
};


//=================================== Player ===================================

const int    MAX_PLAYER_NAME_LENGTH = 16;
const int    NORMAL_HINT_QUEUE_SIZE = 1;

class Game;

struct Statistics
{
  int lineCleared;

  void clear()
  {
    lineCleared = 0;
  }
};

struct AccountInfo
{
  std::string name;
//  Statistics totalStatistics;
  // TODO: other stats
};

// TODO: make sove variabes private
// Initialize at:  [N]ever, on [C]reation, new [M]atch, new [R]ound, game [S]ettings change
class Player
{
public:
  int           number;         // C
  int           accountNumber;  // S  // TODO: changeSystem, use some ID's instead of numbers
  Game*         game;           // C
  int           backgroundSeed; // R

  bool          participates;   // S
  bool          active;         // R (in Game::newRound)
  int           score;          // M
  Controls      controls;       // S
  Statistics    statistics;     // R

  float         speed;          // R
  Field         field;          // C (boders) / R (content)
  FieldLocks    fieldLocks;     // R
  Time          latestLineCollapse; // R

  FallingPieceState   fallingPieceState;  // R
  Piece               fallingPiece;       // R
  std::vector<Piece>  nextPieces;         // R

  Buffs         buffs;          // R
  Debuffs       debuffs;        // R
  int           victimNumber;   // R

  EventSet      events;         // R

  std::vector<BlockImage>                         lyingBlockImages;   // R  // TODO: make be, simply use an  std::map  of  lyingBlockImages ?
  std::map<FieldCoords, int, CompareFieldCoords>  lyingBlockIndices;  // R
  MovingObject                                    fallingPieceFrame;  // R
  std::vector<BlockImage>                         fallingBlockImages; // R  (based on fallingPieceFrame)
  std::vector<DisappearingLine>                   disappearingLines;  // R
  PlayerVisualEffects                             visualEffects;      // R

  util::FixedZeroBasedArray<Time, N_PLAYER_KEYS> nextKeyActivationTable; // C

  void          init(Game* game__, int number__);
  void          loadAccountInfo(int newAccount);
  AccountInfo*  account();
  void          prepareForNewMatch();
  void          prepareForNewRound();

  Time          currentTime();
  Time          pieceLoweringInterval();

  Player*       victim() const;
  std::string   victimName() const;

  // TODO: standardize terminology:  fantasy  OR  formal (?)
  void          takesBonus(Bonus bonus);
  void          applyBonus(Bonus bonus);
  void          disenchant(Bonus bonus);
  void          heal();  // make private (?)
  void          kill();

  void          onKeyPress(PlayerKey key);
  void          onTimer();

  void          stealPiece();  // (!) make private

private:
  int           highestNonemptyRow() const;  // TODO: make it a variable (?)
  bool          canDisposePiece(FieldCoords position, const BlockStructure& piece) const;
  bool          canSendNewPiece() const;
  bool          fallingPieceCannotReachSky() const;
  Piece         randomPiece() const;
  bool          bonusIsUseful(Bonus bonus) const;
  Bonus         randomBonus() const;

  void          beginClearField();
  void          endClearField();
  void          setUpPiece();
  void          initPieceQueue(int size);
  void          resizePieceQueue(int newSize);
//  void          sendNewPiece();
  bool          sendNewPiece();
  void          lowerPiece(bool forced);
  bool          removeFullLines();
  void          collapseLine(int row);
  void          movePiece(int direction);
  void          dropPiece();
  void          rotatePiece(int direction);

  bool          generateBonus();
  void          removeBonuses();
  void          planBonusAppearance();
  void          planBonusDisappearance(FieldCoords bonusCoords);

  /*void          addStandingBlockImage(vector<BlockImage>& imageArray, VisualObject* parent,
                                      Color color, FieldCoords position);  // name (?)
  void          moveBlockImage(vector<BlockImage>& imageArray, FieldCoords movingFrom,
                               FieldCoords movingTo, Time movingDuration);
  void          removeBlockImage(vector<BlockImage>& imageArray, FieldCoords position);*/
  void          moveLyingBlockImage(FieldCoords movingFrom, FieldCoords movingTo, Time movingDuration);

  void          routineSpeedUp();
  void          bonusSpeedUp();
  void          bonusSlowDown();
  void          cycleVictim();

  void          enableBonusVisualEffect(Bonus bonus);
  void          disableBonusVisualEffect(Bonus bonus);
//  void          stealPiece();
  void          flipBlocks();
};


//==================================== Game ====================================

class Game
{
public:
  std::vector<AccountInfo> accounts;

  // TODO: participants array (to speed up booting)
  util::FixedZeroBasedArray<Player, MAX_PLAYERS> players;
  std::vector<Player*> participants;
  std::vector<Player*> activePlayers;

  std::vector<PieceTemplate> pieceTemplates;
  std::vector<int>   randomPieceTable;

  std::vector<Bonus> randomBonusTable;

  Time          currentTime;
  GlobalVisualEffects   globalEffects;

//  FixedZeroBasedArray<Time, N_GLOBAL_KEYS> nextGlobalKeyActivationTable;
  util::FixedZeroBasedArray<Time, 1> nextGlobalKeyActivationTable;
  GlobalControls globalControls;

  void          init();

  void          saveAccounts();
  void          saveSettings();

  void          newMatch();
  void          newRound(Time currentTime__);
  void          endRound();

  void          onGlobalKeyPress(GlobalKey key);
  void          onTimer(Time currentTime);

private:
  void          loadPieces();
  void          loadBonuses();
  void          loadAccounts();
  void          loadDefaultAccounts();
  void          loadSettings();
  void          loadDefaultSettings();
};

}  // namespace engine
