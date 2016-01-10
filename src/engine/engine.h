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

#include "engine/bonus.h"
#include "engine/event.h"
#include "engine/field.h"
#include "engine/input/control.h"
#include "engine/visual.h"
#include "util/containers.h"
#include "util/file.h"


namespace engine {

//================================ Time & speed ================================

extern const double STARTING_SPEED;
extern const double ROUTINE_SPEED_UP_VALUE;
extern const Time   ROUTINE_SPEED_UP_INTERVAL;
extern const double NORMAL_SPEED_LIMIT;
extern const double ABSOLUTE_SPEED_LIMIT;

extern const Time   AUTO_LOWERING_TIME;
extern const Time   DROPPING_PIECE_LOWERING_TIME;
extern const Time   LINE_DISAPPEAR_TIME;
extern const Time   LINE_COLLAPSE_TIME;

extern const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME;
extern const Time   PIECE_FORCED_LOWERING_ANIMATION_TIME;
extern const Time   LINE_COLLAPSE_ANIMATION_TIME;
extern const Time   PIECE_MOVING_ANIMATION_TIME;
extern const Time   PIECE_ROTATING_ANIMATION_TIME;

extern const Time   HINT_APPERAING_TIME;
extern const Time   HINT_MATERIALIZATION_TIME;
extern const Time   PLAYER_DYING_ANIMATION_TIME;


//================================== Bonuses ===================================

const int    BONUS_ENLARGED_HINT_QUEUE_SIZE = 7;
const Time   BONUS_FADING_DURATION = 0.5s;

const double BONUS_SPEED_UP_VALUE = 0.8;
const double BONUS_SLOW_DOWN_VALUE = 1.0;

const Time   BONUS_FLIPPING_SCREEN_DURATION = 0.8s;
const Time   BONUS_ROTATING_SCREEN_PERIOD = 10.0s;
const Time   BONUS_WAVE_PERIOD = 2.0s;
const Time   BONUS_CLEAR_FIELD_DURATION = 1.0s;
const Time   BONUS_CUTTING_BLOCKS_DURATION = 0.5s;
const Time   BONUS_REMOVING_HINT_DURATION = 1.0s;
const Time   BONUS_LANTERN_FADING_TIME = 1.5s;
// const Time   BONUS_LANTERN_ANIMATION_TIME = PIECE_FORCED_LOWERING_ANIMATION_TIME;  // (?)
const Speed  BONUS_LANTERN_MAX_SPEED = 100.0 / 1.0s;

const Time   MIN_BONUS_APPEAR_TIME = 4.0s;
const Time   MAX_BONUS_APPEAR_TIME = 6.0s;
const Time   MIN_BONUS_LIFE_TIME = 20.0s;
const Time   MAX_BONUS_LIFE_TIME = 25.0s;

const int    N_BONUS_CHOOSE_ATTEMPTS = 10;
const int    N_BONUS_GENERATION_ATTEMPTS = 5;
const double BONUS_ONE_ROW_CHANCE = 0.4;
const int    N_BONUS_ONE_ROW_ATTEMPTS = 1;

const int    BONUS_HIGHEST_LINE_MAKING_CLEARING_USEFUL = FIELD_HEIGHT / 2;


//=============================== Pieces & Field ===============================

const int    CENTRAL_PIECE_ROW = (MAX_PIECE_SIZE - 1) / 2; // is it necessary (?)
const int    CENTRAL_PIECE_COL = (MAX_PIECE_SIZE - 1) / 2; // is it necessary (?)
const int    N_PIECE_ROTATION_STATES = 4;

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

// TODO: make sove variabes private
// Initialize at:  [N]ever, on [C]reation, new [M]atch, new [R]ound, game [S]ettings change
class Player
{
public:
  int           number;         // C
  Game*         game;           // C
  int           backgroundSeed; // R

  bool          participates;   // S
  bool          active;         // R (in Game::newRound)
  int           score;          // M
  PlayerControls controls;      // S

  double        speed;          // R
  Field         field;          // C (boders) / R (content)
  FieldLocks    fieldLocks;     // R
  Time          latestLineCollapse; // R

  FallingPieceState   fallingPieceState;  // R
  Piece               fallingPiece;       // R
  std::vector<Piece>  nextPieces;         // R

  BonusesBitSet bonuces;        // R
  int           victimNumber;   // R

  EventSet      events;         // R

  std::vector<BlockImage>                         lyingBlockImages;   // R  // TODO: make be, simply use an  std::map  of  lyingBlockImages ?
  std::map<FieldCoords, int, CompareFieldCoords>  lyingBlockIndices;  // R
  MovingObject                                    fallingPieceFrame;  // R
  std::vector<BlockImage>                         fallingBlockImages; // R  (based on fallingPieceFrame)
  std::vector<DisappearingLine>                   disappearingLines;  // R
  PlayerVisualEffects                             visualEffects;      // R

  std::array<Time, kNumPlayerControls> nextKeyActivationTable; // C

  void          init(Game* game__, int number__);
  void          prepareForNewMatch();
  void          prepareForNewRound();

  Time          currentTime();
  Time          pieceLoweringInterval();

  std::string   name() const;
  Player*       victim() const;

  void          takesBonus(Bonus bonus);
  void          applyBonus(Bonus bonus);
  void          disenchant(Bonus bonus);
  void          heal();  // make private (?)
  void          kill();

  void          onKeyPress(PlayerControl key);
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

  void          moveLyingBlockImage(FieldCoords movingFrom, FieldCoords movingTo, Time movingDuration);

  void          routineSpeedUp();
  void          bonusSpeedUp();
  void          bonusSlowDown();
  void          cycleVictim();

  void          enableBonusVisualEffect(Bonus bonus);
  void          disableBonusVisualEffect(Bonus bonus);
};


//==================================== Game ====================================

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

  void          loadPieces();
  void          loadBonuses();
};

}  // namespace engine
