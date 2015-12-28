// TODO: add  #error  commands

// TODO: make more animations nonlinear

// TODO: change active player list

// TODO: use more throw's instead of assertions

// TODO: Realize  field.lock() / field.unlock():  the field may be make immutable for some time
//       add all events that what to change the field are delayed  (?)
// No, looks like it's better to check specific conditions before any change

// TODO: game speed up formula -- ?

#ifndef CRAZYTETRIS_ENGINE_H
#define CRAZYTETRIS_ENGINE_H

#include <climits>
#include <string>
#include <vector>
#include <bitset>
#include <set>
#include <map>
#include "Strings.h"
#include "Declarations.h"
#include "IOFunctions.h"
#include "VisualEffects.h"

using std::wstring;
using std::pair;
using std::make_pair;
using std::vector;
using std::bitset;
using std::multiset;
using std::map;




//================================ Time & speed ================================

// Continuous mode

/*const float  STARTING_SPEED = 1.0;
const float  ROUTINE_SPEED_UP_VALUE = 0.007f;
const Time   ROUTINE_SPEED_UP_INTERVAL = 2.0f;
const float  NORMAL_SPEED_LIMIT = 5.0;
const float  ABSOLUTE_SPEED_LIMIT = 10.0;

const Time   AUTO_LOWERING_TIME = 0.8f;
// Time necessary for a dropping piece to move one line down
const Time   DROPPING_PIECE_LOWERING_TIME = 0.04f;
const Time   LINE_DISAPPEAR_TIME = 1.0f;
const Time   LINE_COLLAPSE_TIME = 0.05f;

const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME = 0.8f;
const Time   PIECE_FORCED_LOWERING_ANIMATION_TIME = 0.1f;   // = DOWN_KEY_REACTIVATION_TIME
const Time   LINE_COLLAPSE_ANIMATION_TIME = 0.3f;
const Time   PIECE_MOVING_ANIMATION_TIME = 0.4f;
const Time   PIECE_ROTATING_ANIMATION_TIME = 0.4f;

const Time   HINT_APPERAING_TIME = 0.3f;
const Time   HINT_MATERIALIZATION_TIME = 0.2f;
const Time   PLAYER_DYING_ANIMATION_TIME = 1.0f;*/



const float  STARTING_SPEED = 1.0;
const float  ROUTINE_SPEED_UP_VALUE = 0.007f;
const Time   ROUTINE_SPEED_UP_INTERVAL = 2.0f;
const float  NORMAL_SPEED_LIMIT = 5.0;
const float  ABSOLUTE_SPEED_LIMIT = 10.0;

const Time   AUTO_LOWERING_TIME = 0.8f;
// Time necessary for a dropping piece to move one line down
const Time   DROPPING_PIECE_LOWERING_TIME = 0.02f;
const Time   LINE_DISAPPEAR_TIME = 0.6f;
const Time   LINE_COLLAPSE_TIME = 0.06f;

//const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME = AUTO_LOWERING_TIME;
const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME = 0.2f;
const Time   PIECE_FORCED_LOWERING_ANIMATION_TIME = 0.1f;   // = DOWN_KEY_REACTIVATION_TIME
const Time   LINE_COLLAPSE_ANIMATION_TIME = 0.06f;
const Time   PIECE_MOVING_ANIMATION_TIME = 0.08f;
const Time   PIECE_ROTATING_ANIMATION_TIME = 0.05f;

const Time   HINT_APPERAING_TIME = 0.3f;
const Time   HINT_MATERIALIZATION_TIME = 0.2f;
const Time   PLAYER_DYING_ANIMATION_TIME = 1.0f;



//================================== Keyboard ==================================

const int N_PLAYER_KEYS = 7;

enum PlayerKey
{
  keyLeft,
  keyRight,
  keyRotateCCW,
  keyRotateCW,
  keyDown,
  keyDrop,
  keyNextVictim
};

// TODO: find optimal value for KEY_REACTIVATION_TIMEs
const Time   MOVE_KEY_REACTIVATION_TIME = 0.12f;
const Time   ROTATE_KEY_REACTIVATION_TIME = 0.15f;
//const Time   DOWN_KEY_REACTIVATION_TIME = 0.08f;
const Time   DOWN_KEY_REACTIVATION_TIME = PIECE_FORCED_LOWERING_ANIMATION_TIME;
const Time   DROP_KEY_REACTIVATION_TIME = 0.25f;
const Time   CHANGE_VICTIM_KEY_REACTIVATION_TIME = 0.2f;

const wstring PLAYER_KEY_NAMES[N_PLAYER_KEYS] =
{
  L"Влево: ",
  L"Вправо: ",
  L"Вращать против часовой: ",
  L"Вращать по часовой: ",
  L"Вниз: ",
  L"Бросить: ",
  L"Следующая цель: "
};

const Time   PLAYER_KEY_REACTIVATION_TIME[N_PLAYER_KEYS] =
{
  MOVE_KEY_REACTIVATION_TIME,
  MOVE_KEY_REACTIVATION_TIME,
  ROTATE_KEY_REACTIVATION_TIME,
  ROTATE_KEY_REACTIVATION_TIME,
  DOWN_KEY_REACTIVATION_TIME,
  DROP_KEY_REACTIVATION_TIME,
  CHANGE_VICTIM_KEY_REACTIVATION_TIME
};



const int N_GLOBAL_KEYS = 0;

enum GlobalKey { };

// const string GLOBAL_KEY_NAME[N_GLOBAL_KEYS] = { };

// const Time   GLOBAL_KEY_REACTIVATION_TIME[N_GLOBAL_KEYS] = { };

const wstring GLOBAL_KEY_NAME[1] = { L"qwerty" };

const Time   GLOBAL_KEY_REACTIVATION_TIME[1] = { 123.0 };



struct PlayerKeyList
{
  KeyboardKey keyLeft;
  KeyboardKey keyRight;
  KeyboardKey keyRotateCCW;
  KeyboardKey keyRotateCW;
  KeyboardKey keyDown;
  KeyboardKey keyDrop;
  KeyboardKey keyNextVictim;
};

union Controls
{
  PlayerKeyList keyByName;
  KeyboardKey keyArray[N_PLAYER_KEYS];
};



struct GlobalKeyList { };

union GlobalControls
{
  GlobalKeyList keyByName;
//  KeyboardKey keyArray[N_GLOBAL_KEYS];
  KeyboardKey keyArray[1];
};



//================================== Bonuses ===================================

#define SKIP_BUFFS          case bnEnlargeHintQueue:  case bnPieceTheft:

#define SKIP_KIND_SORCERIES case bnHeal:  case bnSlowDown:  case bnClearField:

#define SKIP_DEBUFFS        case bnFlippedScreen:  case bnCrazyPieces: \
                            case bnTruncatedBlocks:  case bnNoHint:

#define SKIP_EVIL_SORCERIES case bnSpeedUp:  // case bnFlipField:

#define SKIP_ENCHANTMENTS  SKIP_BUFFS  SKIP_DEBUFFS

#define SKIP_SORCERIES  SKIP_KIND_SORCERIES  SKIP_EVIL_SORCERIES

#define SKIP_ALL_BUT_SORCERIES  SKIP_ENCHANTMENTS  case bnNoBonus:

#define SKIP_ALL_BUT_ENCHANTMENTS  SKIP_SORCERIES  case bnNoBonus:

#define SKIP_ALL_BUT_BUFFS  SKIP_ALL_BUT_ENCHANTMENTS  SKIP_DEBUFFS

#define SKIP_ALL_BUT_DEBUFFS  SKIP_ALL_BUT_ENCHANTMENTS  SKIP_BUFFS



const int    BONUS_CHANCES[N_BONUSES] =
{
  0,  // bnNoBonus
  0,  // bnEnlargeHintQueue
  0,  // bnPieceTheft
  10, // bnHeal
  4,  // bnSlowDown
  2,  // bnClearField
  4,  // bnFlippedScreen
  3,  // bnRotatingScreen
  4,  // bnWave
  3,  // bnLantern
  0,  // bnCrazyPieces
  4,  // bnTruncatedBlocks
  0,  // bnNoHint
  4   // bnSpeedUp
//  2   // bnFlipField
};

const int    BONUS_ENLARGED_HINT_QUEUE_SIZE = 7;
const Time   BONUS_FADING_DURATION = 0.5f;

const float  BONUS_SPEED_UP_VALUE = 0.8f;
const float  BONUS_SLOW_DOWN_VALUE = 1.0f;

const Time   BONUS_FLIPPING_SCREEN_DURATION = 0.8f;
const Time   BONUS_ROTATING_SCREEN_PERIOD = 10.0f;
const Time   BONUS_WAVE_PERIOD = 2.0f;
const Time   BONUS_CLEAR_FIELD_DURATION = 1.0f;
const Time   BONUS_CUTTING_BLOCKS_DURATION = 0.5f;
const Time   BONUS_REMOVING_HINT_DURATION = 1.0f;
const Time   BONUS_LANTERN_FADING_TIME = 1.5f;
// const Time   BONUS_LANTERN_ANIMATION_TIME = PIECE_FORCED_LOWERING_ANIMATION_TIME;  // (?)
const Time   BONUS_LANTERN_MAX_SPEED = 100.0f; // Requirements: BONUS_LANTERN_MAX_SPEED >= 1.0 / DROPPING_PIECE_LOWERING_TIME

const Time   MIN_BONUS_APPEAR_TIME = 4.0f;
const Time   MAX_BONUS_APPEAR_TIME = 6.0f;
const Time   MIN_BONUS_LIFE_TIME = 20.0;
const Time   MAX_BONUS_LIFE_TIME = 25.0;

/*const int    N_BONUS_CHOOSE_ATTEMPTS = 10;
const int    N_BONUS_GENERATION_ATTEMPTS = 5;
const float  BONUS_ONE_ROW_CHANCE = 0.3f;
const int    N_BONUS_ONE_ROW_ATTEMPTS = 2;*/

const int    N_BONUS_CHOOSE_ATTEMPTS = 10;
const int    N_BONUS_GENERATION_ATTEMPTS = 5;
const float  BONUS_ONE_ROW_CHANCE = 0.4f;
const int    N_BONUS_ONE_ROW_ATTEMPTS = 1;

const int    BONUS_HIGHEST_LINE_MAKING_CLEARING_USEFUL = FIELD_HEIGHT / 2;



const Bonus  FIRST_BUFF         = bnEnlargeHintQueue;
const Bonus  LAST_BUFF          = bnPieceTheft;
const int    N_BUFFS            = LAST_BUFF - FIRST_BUFF + 1;

const Bonus  FIRST_DEBUFF       = bnFlippedScreen;
const Bonus  LAST_DEBUFF        = bnNoHint;
const int    N_DEBUFFS          = LAST_DEBUFF - FIRST_DEBUFF + 1;

const Bonus  FIRST_KIND_BONUS   = FIRST_BUFF;
const Bonus  LAST_KIND_BONUS    = bnClearField;

const Bonus  FIRST_EVIL_BONUS   = FIRST_DEBUFF;
const Bonus  LAST_EVIl_BONUS    = bnSpeedUp;

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

typedef ShiftedBitSet<N_BUFFS, FIRST_BUFF> Buffs;

typedef ShiftedBitSet<N_DEBUFFS, FIRST_DEBUFF> Debuffs;



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
  vector<FieldCoords> blocks;
  Bounds bounds;

  void setBounds()
  {
    if (blocks.empty())
      throw ERR_EMPTY_BLOCK;
    bounds.bottom = blocks[0].row;
    bounds.top = blocks[0].row;
    bounds.left = blocks[0].col;
    bounds.right = blocks[0].col;
    for (vector<FieldCoords>::iterator i = blocks.begin() + 1; i != blocks.end(); ++i)
    {
      if (i->row < bounds.bottom)
        bounds.bottom = i->row;
      if (i->row > bounds.top)
        bounds.top = i->row;
      if (i->col < bounds.left)
        bounds.left = i->col;
      if (i->col > bounds.right)
        bounds.right = i->col;
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

  /*void setPiece(PieceTemplate* pieceTemplate__, int rotationState__, FieldCoords position__)
  {
    pieceTemplate = pieceTemplate__;
    rotationState = rotationState__;
    position = position__;
  }*/

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

  /*void assign(const FieldCell& a)
  {
    blocked = a.blocked;
    color = a.color;
    bonus = a.bonus;
  }*/

  void clear()
  {
    blocked = false;
  }

  void setBlock(Color color__, Bonus bonus__ = bnNoBonus)
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



struct Field : public Fixed2DArray<FieldCell, -WALL_WIDTH, -WALL_WIDTH,
                                   FIELD_HEIGHT + SKY_HEIGHT, FIELD_WIDTH + WALL_WIDTH>
{
  Field();

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
    return operator()(position.row, position.col);
  }

  const FieldCell& operator()(FieldCoords position) const
  {
    return operator()(position.row, position.col);
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



//=================================== Events ===================================

const Time   EVENT_DELAY_TIME = 0.01f; // (?)

enum EventType
{
  etPieceLowering,
  etLineCollapse,
  etNewPiece,
  etRoutineSpeedUp,
  etBonusAppearance,
  etBonusDisappearance,
  etHeal,
  etBeginClearField,
  etEndClearField,
  etKill
};



struct LineCollapseParameters
{
  int row;
};

union EventParameters
{
  LineCollapseParameters lineCollapse;
};



struct Event
{
  EventType type;
  Time activationTime;
  EventParameters parameters;

  Event() { }
  Event(const Event& otherEvent) : type(otherEvent.type), activationTime(otherEvent.activationTime),
                                   parameters(otherEvent.parameters) { }
  Event(EventType type__, Time activationTime__) : type(type__), activationTime(activationTime__) { }

  bool operator<(const Event& otherEvent) const
  {
    return activationTime < otherEvent.activationTime;
  }
};



// TODO: reorganize that (may be add counter of events of a certain type or use several sets ot maps)
class EventSet
{
public:
  typedef multiset<Event>::iterator iterator;

  iterator begin()
  {
    return events_.begin();
  }

  iterator end()
  {
    return events_.end();
  }

  void push(const Event& event)
  {
    events_.insert(event);
  }

  void push(const EventType eventType, Time activationTime)
  {
    events_.insert(Event(eventType, activationTime));
  }

  void pushWithUniquenessCheck(const EventType eventType, Time activationTime)
  {
    for (iterator i = begin(); i != end(); ++i)
      assert(i->type != eventType);
    push(eventType, activationTime);
  }

  const Event& top()
  {
    return *events_.begin();
  }

  void pop()
  {
    events_.erase(events_.begin());
  }

  void erase(iterator it)
  {
    events_.erase(it);
  }

  void eraseEventType(EventType eventType)
  {
    for (iterator i = begin(); i != end(); )
    {
      if (i->type == eventType)
        events_.erase(i++);
      else
        ++i;
    }
  }

  /*void delay(iterator it)
  {
    Event event = *it;
    erase(it);
    event.activationTime += EVENT_DELAY_TIME;
    push(event);
  }*/

  void clear()
  {
    events_.clear();
  }

  bool empty()
  {
    return events_.empty();
  }

private:
  multiset<Event> events_;
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
  wstring name;
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

  FallingPieceState     fallingPieceState;  // R
  Piece                 fallingPiece;       // R
  vector<const Piece>   nextPieces;         // R

  Buffs         buffs;          // R
  Debuffs       debuffs;        // R
  int           victimNumber;   // R

  EventSet      events;         // R

  vector<BlockImage>            lyingBlockImages;   // R  // TODO: make be, simply use an  std::map  of  lyingBlockImages ?
  map<FieldCoords, int>         lyingBlockIndices;  // R
  MovingObject                  fallingPieceFrame;  // R
  vector<BlockImage>            fallingBlockImages; // R  (based on fallingPieceFrame)
  vector<DisappearingLine>      disappearingLines;  // R
  PlayerVisualEffects           visualEffects;      // R

  FixedZeroBasedArray<Time, N_PLAYER_KEYS> nextKeyActivationTable; // C

  void          init(Game* game__, int number__);
  void          loadAccountInfo(int newAccount);
  AccountInfo*  account();
  void          prepareForNewMatch();
  void          prepareForNewRound();

  Time          currentTime();
  Time          pieceLoweringInterval();

  Player*       victim() const;
  wstring       victimName() const;

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
  vector<AccountInfo> accounts;

  // TODO: participants array (to speed up booting)
  FixedZeroBasedArray<Player, MAX_PLAYERS> players;
  vector<Player*> participants;
  vector<Player*> activePlayers;

  vector<PieceTemplate> pieceTemplates;
  vector<int>   randomPieceTable;

  vector<Bonus> randomBonusTable;

  Time          currentTime;
  GlobalVisualEffects   globalEffects;

//  FixedZeroBasedArray<Time, N_GLOBAL_KEYS> nextGlobalKeyActivationTable;
  FixedZeroBasedArray<Time, 1> nextGlobalKeyActivationTable;
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

#endif
