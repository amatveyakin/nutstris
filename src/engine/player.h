// TODO: Implement  field.lock() / field.unlock():  the field may be made immutable for some time
//       add all events that what to change the field are delayed  (?)
// No, looks like it's better to check specific conditions before any change

// TODO: game speed up formula -- ?

#pragma once

#include <array>
#include <climits>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "engine/bonus.h"
#include "engine/event.h"
#include "engine/field.h"
#include "engine/io/control.h"
#include "engine/piece.h"
#include "engine/visual.h"
#include "util/containers.h"
#include "util/file.h"


namespace engine {

class GameRound;

// TODO(Andrei): Move the constants to a separate file.

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
const Time   BONUS_PIECE_THEFT_DURATION = 0.7s;

const Time   MIN_BONUS_APPEAR_TIME = 4.0s;
const Time   MAX_BONUS_APPEAR_TIME = 6.0s;
const Time   MIN_BONUS_LIFE_TIME = 20.0s;
const Time   MAX_BONUS_LIFE_TIME = 25.0s;

const int    N_BONUS_CHOOSE_ATTEMPTS = 10;
const int    N_BONUS_GENERATION_ATTEMPTS = 5;
const double BONUS_ONE_ROW_CHANCE = 0.4;
const int    N_BONUS_ONE_ROW_ATTEMPTS = 1;

const int    BONUS_HIGHEST_LINE_MAKING_CLEARING_USEFUL = FIELD_HEIGHT / 2;


//=================================== Other ====================================

const int    NORMAL_HINT_QUEUE_SIZE = 1;


//=================================== Field ====================================

struct FieldLocks {
  bool isBeingCleared = false;
};


//================================= Player Info ================================

class PlayerInfo {
public:
  PlayerInfo(int id__) : id_(id__) {}

  int id() const                      { return id_; }

  std::string name() const            { return name_; }
  void setName(const std::string& v)  { name_ = v; }

  bool participates() const           { return participates_; }
  void setParticipates(bool v)        { participates_ = v; }

  const PlayerControls& controls() const    { return controls_; }
  void setControls(const PlayerControls& v) { controls_ = v; }

  int score() const                   { return score_; }
  void setScore(int v)                { score_ = v; }

private:
  int id_ = 0;
  std::string name_;
  bool participates_ = false;
  PlayerControls controls_;
  int score_ = 0;
};


//=================================== Player ===================================

// TODO: make all variables private
class Player
{
public:
  Player(const PlayerInfo* info__, GameRound* game__);

  const PlayerInfo* info;
  GameRound*        game;  // TODO: rename (?)

  bool          active = true;

  double        speed = 0.;
  Field         field;
  FieldLocks    fieldLocks;
  Time          latestLineCollapse = Time::zero();

  FallingPieceState   fallingPieceState = {};
  Piece               fallingPiece;
  std::vector<Piece>  nextPieces;

  BonusesBitSet bonuces;
  int           victimId = 0;

  EventSet      events;

  int                                             backgroundSeed = 0;
  std::vector<BlockImage>                         lyingBlockImages;  // TODO: make be, simply use an  std::map  of  lyingBlockImages ?
  std::map<FieldCoords, int, CompareFieldCoords>  lyingBlockIndices;
  MovingObject                                    fallingPieceFrame;
  std::vector<BlockImage>                         fallingBlockImages;  // (based on fallingPieceFrame)
  std::vector<DisappearingLine>                   disappearingLines;
  PlayerVisualEffects                             visualEffects;

  std::array<Time, kNumPlayerControls> nextKeyActivationTable;

  Time          currentTime();
  Time          pieceLoweringInterval();

  int           id() const;
  std::string   name() const;
  Player*       victim() const;

  void          takesBonus(Bonus bonus);
  void          applyBonus(Bonus bonus);
  void          disenchant(Bonus bonus);
  void          heal();  // make private (?)
  void          kill();
  void          cycleVictim();

  void          onKeyPress(PlayerControl key);
  void          processInput();
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
  int           removeFullLines();
  void          collapseLine(int row);
  void          movePiece(int direction);
  void          dropPiece();
  void          rotatePiece(int direction);

  bool          generateBonus();
  void          startBonusDisappearanceAnimations();
  void          removeBonuses();
  void          planBonusAppearance();
  void          planBonusDisappearance();

  void          moveLyingBlockImage(FieldCoords movingFrom, FieldCoords movingTo, Time movingDuration);

  void          routineSpeedUp();
  void          bonusSpeedUp();
  void          bonusSlowDown();

  void          enableBonusVisualEffect(Bonus bonus);
  void          disableBonusVisualEffect(Bonus bonus);
};

}  // namespace engine
