#include "player.h"

#include <iostream>
#include <cstdio>

#include "engine/game.h"
#include "engine/strings.h"


namespace engine {

namespace {
Time randomTimeRange(Time minValue, Time maxValue) {
  return 1.0s * math::randomRange(minValue / 1.0s, maxValue / 1.0s);
}
}  // namespace

//================================ Time & speed ================================

// Continuous mode

/*const double STARTING_SPEED = 1.0;
const double ROUTINE_SPEED_UP_VALUE = 0.007;
const Time   ROUTINE_SPEED_UP_INTERVAL = 2.0s;
const double NORMAL_SPEED_LIMIT = 5.0;
const double ABSOLUTE_SPEED_LIMIT = 10.0;

const Time   AUTO_LOWERING_TIME = 0.8s;
const Time   DROPPING_PIECE_LOWERING_TIME = 0.04s;
const Time   LINE_DISAPPEAR_TIME = 1.0s;
const Time   LINE_COLLAPSE_TIME = 0.05s;

const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME = 0.8s;
const Time   PIECE_FORCED_LOWERING_ANIMATION_TIME = 0.1s;   // = DOWN_KEY_REACTIVATION_TIME
const Time   LINE_COLLAPSE_ANIMATION_TIME = 0.3s;
const Time   PIECE_MOVING_ANIMATION_TIME = 0.4s;
const Time   PIECE_ROTATING_ANIMATION_TIME = 0.4s;

const Time   HINT_APPERAING_TIME = 0.3s;
const Time   HINT_MATERIALIZATION_TIME = 0.2s;
const Time   PLAYER_DYING_ANIMATION_TIME = 1.0s;*/


const double STARTING_SPEED = 1.0;
const double ROUTINE_SPEED_UP_VALUE = 0.007;
const Time   ROUTINE_SPEED_UP_INTERVAL = 2.0s;
const double NORMAL_SPEED_LIMIT = 5.0;
const double ABSOLUTE_SPEED_LIMIT = 10.0;

const Time   AUTO_LOWERING_TIME = 0.8s;
const Time   DROPPING_PIECE_LOWERING_TIME = 0.02s;
const Time   LINE_DISAPPEAR_TIME = 0.6s;
const Time   LINE_COLLAPSE_TIME = 0.06s;

//const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME = AUTO_LOWERING_TIME;
const Time   PIECE_AUTO_LOWERING_ANIMATION_TIME = 0.2s;
const Time   PIECE_FORCED_LOWERING_ANIMATION_TIME = 0.1s;   // = DOWN_KEY_REACTIVATION_TIME
const Time   LINE_COLLAPSE_ANIMATION_TIME = 0.06s;
const Time   PIECE_MOVING_ANIMATION_TIME = 0.08s;
const Time   PIECE_ROTATING_ANIMATION_TIME = 0.05s;

const Time   HINT_APPERAING_TIME = 0.3s;
const Time   HINT_MATERIALIZATION_TIME = 0.2s;
const Time   PLAYER_DYING_ANIMATION_TIME = 1.0s;


//=================================== Player ===================================

void Player::init(Game* game__, int number__)
{
  game = game__;
  number = number__;
  for (int key = 0; key < kNumPlayerControls; ++key)
    nextKeyActivationTable[key] = Time::min();
}

void Player::prepareForNewMatch()
{
  score = 0;
  // ...
}

void Player::prepareForNewRound()
{
  events.clear();
  fieldLocks.clear();
  bonuces.clear();
  field = Field();
  lyingBlockImages.clear();
  lyingBlockIndices.clear();
  fallingBlockImages.clear();
  disappearingLines.clear();
  visualEffects = PlayerVisualEffects();
  visualEffects.lantern.bindTo(&fallingPieceFrame);  // TODO: move to initialization?
  visualEffects.lantern.placeAt(FloatFieldCoords((FIELD_WIDTH  - 1.0) / 2.0, (FIELD_HEIGHT - 1.0) / 2.0));
  visualEffects.lantern.setMaxSpeed(BONUS_LANTERN_MAX_SPEED);
  latestLineCollapse = Time::min();
  victimNumber = number;
  cycleVictim();
  speed = STARTING_SPEED;

  backgroundSeed = rand();
  events.push(etRoutineSpeedUp, currentTime() + ROUTINE_SPEED_UP_INTERVAL);
  initPieceQueue(NORMAL_HINT_QUEUE_SIZE);
  planBonusAppearance();
  sendNewPiece();
}

Time Player::currentTime()
{
  return game->currentTime;
}

Time Player::pieceLoweringInterval()
{
  return AUTO_LOWERING_TIME / speed;
}

std::string Player::name() const
{
  return "Player " + std::to_string(number);
}

Player* Player::victim() const
{
  return (victimNumber != number) ? &game->players[victimNumber] : NULL;
}

void Player::takesBonus(Bonus bonus)
{
  if (isKind(bonus))
    applyBonus(bonus);
  else
  {
    if (victim() != NULL)
      victim()->applyBonus(bonus);
  }
}

void Player::applyBonus(Bonus bonus)
{
  switch (bonus)
  {
  case Bonus::EnlargeHintQueue:
    resizePieceQueue(BONUS_ENLARGED_HINT_QUEUE_SIZE);
    break;
  case Bonus::PieceTheft:
    break;
  case Bonus::Heal:
    events.push(etHeal, currentTime());
//    heal();
    break;
  case Bonus::SlowDown:
    bonusSlowDown();
    break;
  case Bonus::ClearField:
    events.push(etBeginClearField, currentTime());
    break;
  case Bonus::SpeedUp:
    bonusSpeedUp();
    break;
  default: break;  // TODO(Andrei): handle all values
  }
  if (isPermanent(bonus))
    bonuces.add(bonus);
  enableBonusVisualEffect(bonus);
}

void Player::disenchant(Bonus bonus)
{
  assert(isPermanent(bonus));
  switch (bonus)
  {
  case Bonus::EnlargeHintQueue:
    resizePieceQueue(NORMAL_HINT_QUEUE_SIZE);
    break;
  case Bonus::PieceTheft:
    // ...
    break;
  // ...
  default: break;  // TODO(Andrei): handle all values
  }
  bonuces.remove(bonus);
  disableBonusVisualEffect(bonus);
}

void Player::heal()
{
  for (Bonus bonus : ForEachBonus()) {
    if (isDebuff(bonus) && bonuces.test(bonus))
      disenchant(bonus);
  }
}

void Player::beginClearField()
{
  //  TODO: also handle a case when the bonus is taken somehow but a piece is still falling (?)
  fieldLocks.isBeingCleared = true;
  field = Field();
  visualEffects.fieldCleaning.enable(BONUS_CLEAR_FIELD_DURATION);
  events.push(etEndClearField, currentTime() + BONUS_CLEAR_FIELD_DURATION);
}

void Player::endClearField()
{
  fieldLocks.isBeingCleared = false;
  lyingBlockImages.clear();
  lyingBlockIndices.clear();
}

void Player::kill()
{
  for (auto i = game->activePlayers.begin(); i != game->activePlayers.end(); ++i)
  {
    if ((*i)->victimNumber == number)
    {
      (*i)->cycleVictim();
      if ((game->activePlayers.size() >= 2) && ((*i)->victimNumber == (*i)->number))
        (*i)->cycleVictim();
    }
  }

  for (auto i = game->activePlayers.begin(); i != game->activePlayers.end(); ++i)
  {
    if (*i == this)
    {
      game->activePlayers.erase(i);
      break;
    }
  }

  if (game->activePlayers.empty())
    game->endRound();

  visualEffects.playerDying.enable(PLAYER_DYING_ANIMATION_TIME);
  active = false;
}

void Player::onKeyPress(PlayerControl key)
{
  switch (key) {
  case PlayerControl::Left:
    movePiece(-1);
    break;
  case PlayerControl::Right:
    movePiece(1);
    break;
  case PlayerControl::RotateCW:
    rotatePiece(-1);
    break;
  case PlayerControl::RotateCCW:
    rotatePiece(1);
    break;
  case PlayerControl::Down:
    events.eraseEventType(etPieceLowering);
    lowerPiece(true /* forced */);
    break;
  case PlayerControl::Drop:
    dropPiece();
    break;
  case PlayerControl::NextVictim:
    cycleVictim();
    break;
  }
}

void Player::onTimer()
{
  while ((!events.empty()) && (currentTime() >= events.top().activationTime))
  {
    Event currentEvent = events.top();
    events.pop();
    bool eventDelayed = false;
    switch (currentEvent.type)
    {
    case etPieceLowering:
      lowerPiece(false /* auto */);
      break;
    case etLineCollapse:
      collapseLine(currentEvent.parameters.lineCollapse.row);
      break;
    case etNewPiece:
      if (!sendNewPiece())
        eventDelayed = true;
      break;
    case etRoutineSpeedUp:   // TODO: [FIXED?] why is it called in the very beginning?
      routineSpeedUp();
      break;
    case etBonusAppearance:
      if (!generateBonus())
        eventDelayed = true;
      break;
    case etBonusDisappearance:
      removeBonuses();   // TODO: remove only one (?)
      break;
    case etHeal:
      heal();
      break;
    case etBeginClearField:
      beginClearField();
      break;
    case etEndClearField:
      endClearField();
      break;
    case etKill:
      kill();
      break;
    default:
      throw std::runtime_error(ERR_INTERNAL_ERROR);  // TODO: formal  "Events queue crashed (event %d found)"
    }
    if (eventDelayed)
    {
      currentEvent.activationTime += EVENT_DELAY_TIME;
      events.push(currentEvent);
    }
  }
}



int Player::highestNonemptyRow() const
{
  for (int row = FIELD_HEIGHT - 1; row >= 0; --row)
    for (int col = 0; col < FIELD_WIDTH; ++col)
      if (field({col, row}).blocked())
        return row;
  return -1;
}

bool Player::canDisposePiece(FieldCoords position, const BlockStructure& piece) const
{
  for (size_t i = 0; i < piece.blocks.size(); ++i)
    if (field(position + piece.blocks[i]).blocked())
      return false;
  return true;
}

bool Player::fallingPieceCannotReachSky() const
{
  for (int i = 0; i < fallingPiece.nRotationStates(); ++i)
    if (fallingPiece.pieceTemplate()->structure[i].bounds.top + fallingPiece.position().y() >= FIELD_HEIGHT)
      return false;
  return true;
}

bool Player::canSendNewPiece() const
{
  return disappearingLines.empty() && !fieldLocks.isBeingCleared;
}

Piece Player::randomPiece() const
{
  int pieceTemplatesIndex = game->randomPieceTable[rand() % game->randomPieceTable.size()];
  Piece piece(&game->pieceTemplates[pieceTemplatesIndex]);
  piece.setRotationState(rand() % piece.nRotationStates());
  piece.moveTo({MAX_PIECE_SIZE + rand() % (FIELD_WIDTH - 2 * MAX_PIECE_SIZE),  // TODO: modify the formula
                FIELD_HEIGHT - piece.currentStructure().bounds.bottom});
  return piece;
}

bool Player::bonusIsUseful(Bonus bonus) const
{
  // TODO: process all debuffs uniformly  (may be, they are just always useful?)
  if (isBuff(bonus))
    return !bonuces.test(bonus);

  switch (bonus)
  {
  case Bonus::None:
    return false;
  case Bonus::Heal:
    for (Bonus b : ForEachBonus()) {
      if (isDebuff(b) && bonuces.test(b))
        return true;
    }
    return false;
  case Bonus::SlowDown:
    return speed > STARTING_SPEED + BONUS_SLOW_DOWN_VALUE;  // (?)
    break;
  case Bonus::ClearField:
    return highestNonemptyRow() > BONUS_HIGHEST_LINE_MAKING_CLEARING_USEFUL;
    break;

  case Bonus::FlippedScreen:
    // ...
    break;
  case Bonus::RotatingScreen:
    // ...
    break;
  case Bonus::Wave:
    // ...
    break;
  case Bonus::Lantern:
    // ...
    break;
  case Bonus::CrazyPieces:
    // ...
    break;
  case Bonus::TruncatedBlocks:
    // ...
    break;
  case Bonus::NoHint:
    // ...
    break;

  case Bonus::SpeedUp:
    // ...
    break;

  default: break;  // TODO(Andrei): handle all values
  }

  return true;
}

Bonus Player::randomBonus() const
{
  Bonus bonus;
  for (int iAttempt = 0; iAttempt < N_BONUS_CHOOSE_ATTEMPTS; ++iAttempt) {
    bonus = game->randomBonusTable[rand() % game->randomBonusTable.size()];
    if (bonusIsUseful(bonus))
      return bonus;
  }
  return Bonus::None;
}

void Player::setUpPiece()
{
  for (size_t i = 0; i < fallingPiece.nBlocks(); ++i)
  {
    FieldCoords cell = fallingPiece.absoluteCoords(i);
    if (cell.y() >= FIELD_HEIGHT)
    {
      events.push(etKill, currentTime());
      return;
    }

    field.mutableCell(cell).setBlock(fallingPiece.color());

    // TODO: copy images from one array to another with motion (?)

    lyingBlockImages.push_back(BlockImage(NULL, fallingPiece.color(), cell));
    lyingBlockIndices.insert(std::make_pair(cell, lyingBlockImages.size() - 1));
  }
  fallingBlockImages.clear();
  fallingPieceState = psAbsent;

  bool fullLinesFound = removeFullLines();
  Time newPieceDelay = fullLinesFound ? std::max(HINT_MATERIALIZATION_TIME, LINE_DISAPPEAR_TIME) :
                                        HINT_MATERIALIZATION_TIME;
  events.pushWithUniquenessCheck(etNewPiece, currentTime() + newPieceDelay);
  visualEffects.hintMaterialization.enable(newPieceDelay);
  visualEffects.hint.enable(HINT_APPERAING_TIME);
//  visualEffects.lantern.resetBinding();
}

void Player::initPieceQueue(int size)
{
  nextPieces.clear();
  resizePieceQueue(size);
}

void Player::resizePieceQueue(int newSize)
{
  int oldSize = nextPieces.size();
  nextPieces.resize(newSize);
  if (newSize > oldSize)
    for (int i = oldSize; i < newSize; ++i)
      nextPieces[i] = randomPiece();
}

bool Player::sendNewPiece()
{
  if (!canSendNewPiece())
    return false;

  fallingPiece = nextPieces[0];
  assert(fallingPiece.valid());

  visualEffects.hint.disable();
  visualEffects.hintMaterialization.disable();

  fallingPieceState = psNormal;
  fallingPieceFrame.placeAt(FloatFieldCoords(fallingPiece.position()));
  for (size_t i = 0; i < fallingPiece.nBlocks(); ++i)
    fallingBlockImages.push_back(BlockImage(&fallingPieceFrame, fallingPiece.color(), fallingPiece.relativeCoords(i)));
//  visualEffects.lantern.bindTo(&fallingPieceFrame);
  events.push(etPieceLowering, currentTime() + pieceLoweringInterval());

  nextPieces.erase(nextPieces.begin());
  nextPieces.push_back(randomPiece());
  /*for (size_t i = 0; i < nextPieces.size() - 1; ++i)
    nextPieces[i] = nextPieces[i + 1];
  nextPieces.back() = randomPiece();*/
  return true;
}

void Player::lowerPiece(bool forced)
{
  if (fallingPieceState == psAbsent)
    return;

  FieldCoords newPosition = fallingPiece.position() + FieldCoords(0, -1);
  // We should not forget about the case when a piece gets stuck as the result of
  // field modification. Such pieces should not continue falling.
  if (canDisposePiece(fallingPiece.position(), fallingPiece.currentStructure()) &&
      canDisposePiece(newPosition,             fallingPiece.currentStructure()))
  {
    Time loweringTime = (fallingPieceState == psDropping) ? DROPPING_PIECE_LOWERING_TIME :
                        (forced ? PIECE_FORCED_LOWERING_ANIMATION_TIME :
                                  PIECE_AUTO_LOWERING_ANIMATION_TIME);
    fallingPieceFrame.addMotion(FloatFieldCoords(newPosition - fallingPiece.position()), currentTime(), loweringTime);

    fallingPiece.moveTo(newPosition);

    if (fallingPieceCannotReachSky())
      visualEffects.hint.enable(HINT_APPERAING_TIME);

    events.push(etPieceLowering,
                currentTime() +
                ((fallingPieceState == psDropping) ?
                 DROPPING_PIECE_LOWERING_TIME : pieceLoweringInterval()));
  }
  else
    setUpPiece();
}

// TODO: Optimize Player::removeFullLines: don't check all lines
// TODO: Does Player::removeFullLines have to return a bool any more?
bool Player::removeFullLines()
{
  bool fullLinesExisted = false;

  for (int row = 0; row < FIELD_HEIGHT; ++row)
  {
    bool rowIsFull = true;
    for (int col = 0; col < FIELD_WIDTH; ++col)
    {
      if (!field({col, row}).blocked())
      {
        rowIsFull = false;
        break;
      }
    }

    if (rowIsFull)
    {
      fullLinesExisted = true;

      disappearingLines.resize(disappearingLines.size() + 1);
      disappearingLines.back().startTime = currentTime();
      disappearingLines.back().duration = LINE_DISAPPEAR_TIME;
      disappearingLines.back().row = row;
      for (int col = 0; col < FIELD_WIDTH; ++col)
      {
        if (field({col, row}).bonus() != Bonus::None)
        {
          takesBonus(field({col, row}).bonus());
          events.eraseEventType(etBonusDisappearance);
          planBonusAppearance();
        }
        disappearingLines.back().blockColor[col] = field({col, row}).color();
        field.mutableCell({col, row}).clear();

        lyingBlockImages[lyingBlockIndices[FieldCoords(col, row)]] = lyingBlockImages.back();
        // TODO: No, *that* was not the point of  lyingBlockIndices !
        for (auto it = lyingBlockIndices.begin(); it != lyingBlockIndices.end(); ++it)
        {
          if (it->second == static_cast<int>(lyingBlockImages.size()) - 1) {
            it->second = lyingBlockIndices[FieldCoords(col, row)];
            break;
          }
        }
        lyingBlockImages.pop_back();
        lyingBlockIndices.erase(FieldCoords(col, row));
      }

      if (latestLineCollapse < currentTime() + LINE_DISAPPEAR_TIME)
        latestLineCollapse = currentTime() + LINE_DISAPPEAR_TIME;
      latestLineCollapse += LINE_COLLAPSE_TIME;
      Event event(etLineCollapse, latestLineCollapse);
      event.parameters.lineCollapse.row = row;
      events.push(event);
    }
  }

  return fullLinesExisted;
}

void Player::collapseLine(int row)  // TODO: optimize
{
  for (int curRow = row; curRow < FIELD_HEIGHT; ++curRow)
  {
    for (int col = 0; col < FIELD_WIDTH; ++col)
    {
      field.mutableCell({col, curRow}) = field({col, curRow + 1});
      if (field({col, curRow}).blocked())
        moveLyingBlockImage(FieldCoords(col, curRow + 1), FieldCoords(col, curRow), LINE_COLLAPSE_ANIMATION_TIME);
    }
  }

  for (auto i = disappearingLines.begin();
       i != disappearingLines.end(); ++i)
  {
    if (i->row == row)
    {
      disappearingLines.erase(i);
      break;
    }
  }
  for (auto i = disappearingLines.begin();
       i != disappearingLines.end(); ++i)
    if (i->row > row)
      --i->row;
  for (auto myEvent = events.begin(); myEvent != events.end(); ++myEvent)
    if ((myEvent->type == etLineCollapse) && (myEvent->parameters.lineCollapse.row > row))
      --const_cast<int&>(myEvent->parameters.lineCollapse.row);
}

void Player::movePiece(int direction)
{
  if (fallingPieceState != psNormal)
    return;
  FieldCoords newPosition = fallingPiece.position() + FieldCoords(direction, 0);
  if (canDisposePiece(newPosition, fallingPiece.currentStructure()))
  {
    fallingPieceFrame.addMotion(FloatFieldCoords(newPosition - fallingPiece.position()), currentTime(), PIECE_MOVING_ANIMATION_TIME);
    fallingPiece.moveTo(newPosition);
  }
}

void Player::dropPiece()
{
  if (fallingPieceState != psNormal)
    return;
  fallingPieceState = psDropping;
  events.eraseEventType(etPieceLowering);
  events.push(etPieceLowering, currentTime() + DROPPING_PIECE_LOWERING_TIME);
}

void Player::rotatePiece(int direction)
{
  if (fallingPieceState != psNormal)
    return;

  // TODO: simply create a  Piece  copy instead (?)
  FieldCoords oldPosition = fallingPiece.position();
  int oldRotationState = fallingPiece.rotationState();
  int newFallingPieceRotationState = math::modFloored(fallingPiece.rotationState() + direction, fallingPiece.nRotationStates());

  if (canDisposePiece(fallingPiece.position(),
      fallingPiece.pieceTemplate()->structure[newFallingPieceRotationState]))
  {
    fallingPiece.setRotationState(newFallingPieceRotationState);
  }
  else if (canDisposePiece(fallingPiece.position() + FieldCoords(1, 0),  // TODO: optimize
      fallingPiece.pieceTemplate()->structure[newFallingPieceRotationState]))
  {
    fallingPiece.moveBy({1, 0});
    fallingPiece.setRotationState(newFallingPieceRotationState);
  }
  else if (canDisposePiece(fallingPiece.position() - FieldCoords(1, 0),  // TODO: optimize
      fallingPiece.pieceTemplate()->structure[newFallingPieceRotationState]))
  {
    fallingPiece.moveBy({-1, 0});
    fallingPiece.setRotationState(newFallingPieceRotationState);
  }

  for (size_t i = 0; i < fallingPiece.nBlocks(); ++i)
  {
    fallingBlockImages[i].addMotion(
            FloatFieldCoords(fallingPiece.absoluteCoords(i) -
                             (oldPosition + fallingPiece.pieceTemplate()->structure[oldRotationState].blocks[i])),
            currentTime(), PIECE_ROTATING_ANIMATION_TIME);
  }
}

bool Player::generateBonus()  // TODO: remake
{
  Bonus bonus = randomBonus();  // Is that possible (?)
  if (bonus == Bonus::None)
    return false;

  for (int iter = 0; iter < N_BONUS_GENERATION_ATTEMPTS; ++iter)
  {
    for (int row = highestNonemptyRow(); row >= 0; --row)
    {
      if (double(rand()) / RAND_MAX > BONUS_ONE_ROW_CHANCE)
        continue;
      for (int colIter = 0; colIter < N_BONUS_ONE_ROW_ATTEMPTS; ++colIter)
      {
        int col = rand() % FIELD_WIDTH;
        if (field({col, row}).blocked())
        {
          field.mutableCell({col, row}).setBonus(bonus);
          lyingBlockImages[lyingBlockIndices[FieldCoords(col, row)]].setBonus(bonus);
          lyingBlockImages[lyingBlockIndices[FieldCoords(col, row)]].bonusImage().enable(BONUS_FADING_DURATION);
          planBonusDisappearance(FieldCoords(col, row));
          return true;
        }
      }
    }
  }
  return false;
}

void Player::removeBonuses()
{
  for (int row = 0; row < FIELD_HEIGHT; ++row)
  {
    for (int col = 0; col < FIELD_WIDTH; ++col)
    {
      if (field({col, row}).blocked())
      {
        field.mutableCell({col, row}).setBonus(Bonus::None);
        lyingBlockImages[lyingBlockIndices[FieldCoords(col, row)]].bonusImage().disable();
      }
    }
  }
  planBonusAppearance();
}

void Player::planBonusAppearance()
{
  events.push(etBonusAppearance, currentTime() + randomTimeRange(MIN_BONUS_APPEAR_TIME, MAX_BONUS_APPEAR_TIME));
}

void Player::planBonusDisappearance(FieldCoords bonusCoords)
{
  Time bonusDisappearTime = currentTime() + randomTimeRange(MIN_BONUS_LIFE_TIME, MAX_BONUS_LIFE_TIME);
  events.push(etBonusDisappearance, bonusDisappearTime);
  lyingBlockImages[lyingBlockIndices[bonusCoords]].bonusImage().stopAt(
          bonusDisappearTime - BONUS_FADING_DURATION);
}

void Player::moveLyingBlockImage(FieldCoords movingFrom, FieldCoords movingTo, Time movingDuration) {
  if (movingFrom == movingTo)
    return;
  lyingBlockImages[lyingBlockIndices[movingFrom]].addMotion(FloatFieldCoords(movingTo - movingFrom), currentTime(), movingDuration);
  lyingBlockIndices[movingTo] = lyingBlockIndices[movingFrom];
  lyingBlockIndices.erase(movingFrom);
}

void Player::routineSpeedUp()
{
  double maxSpeed = std::max(NORMAL_SPEED_LIMIT, speed);
  speed += ROUTINE_SPEED_UP_VALUE;
  speed = std::min(speed, maxSpeed);
  events.push(etRoutineSpeedUp, currentTime() + ROUTINE_SPEED_UP_INTERVAL);
}

void Player::bonusSpeedUp()
{
  speed += BONUS_SPEED_UP_VALUE;
  speed = std::min(speed, ABSOLUTE_SPEED_LIMIT);
}

void Player::bonusSlowDown()
{
  speed -= BONUS_SLOW_DOWN_VALUE;
  speed = std::max(speed, STARTING_SPEED);
}

void Player::cycleVictim()
{
  if (game->activePlayers.size() < 2)
    return;
  do
  {
    victimNumber = (victimNumber + 1) % MAX_PLAYERS;
  } while (!game->players[victimNumber].active);
}

void Player::enableBonusVisualEffect(Bonus bonus)
{
  switch (bonus)
  {
  case Bonus::Heal:
    // no effect
    break;
  case Bonus::SlowDown:
    // no effect
    break;
  case Bonus::ClearField:
    // (!) It is enabled in beginClearField().  May be the conceptions needs changing
    //visualEffects.fieldCleaning.enable(BONUS_CLEAR_FIELD_DURATION / 2);  // (!) Change if effect type is changed
    break;
  case Bonus::FlippedScreen:
    visualEffects.flippedScreen.enable(BONUS_FLIPPING_SCREEN_DURATION);
    break;
  case Bonus::RotatingScreen:
    visualEffects.rotatingField.enable(BONUS_ROTATING_SCREEN_PERIOD);
    break;
  case Bonus::Wave:
    visualEffects.wave.enable(BONUS_WAVE_PERIOD);
    break;
  case Bonus::Lantern:
    visualEffects.lantern.enable(BONUS_LANTERN_FADING_TIME);
    break;
  case Bonus::CrazyPieces:
    // no effect
    break;
  case Bonus::TruncatedBlocks:
    visualEffects.semicubes.enable(BONUS_CUTTING_BLOCKS_DURATION);
    break;
  case Bonus::NoHint:
    visualEffects.noHint.enable(BONUS_REMOVING_HINT_DURATION);
    break;
  case Bonus::SpeedUp:
    // no effect
    break;
//  case Bonus::FlipField:
//    // no effect *at this moment*
//    break;
  case Bonus::None: ;
  default: break;  // TODO(Andrei): handle all values
  }
}

void Player::disableBonusVisualEffect(Bonus bonus)
{
  switch (bonus)
  {
  case Bonus::Heal:
    // no effect
    break;
  case Bonus::SlowDown:
    // no effect
    break;
  case Bonus::ClearField:
    // the effect ends itself
    break;
  case Bonus::FlippedScreen:
    visualEffects.flippedScreen.disable();
    break;
  case Bonus::RotatingScreen:
    visualEffects.rotatingField.disable();
    break;
  case Bonus::Wave:
    visualEffects.wave.disable();
    break;
  case Bonus::Lantern:
    visualEffects.lantern.disable();
    break;
  case Bonus::CrazyPieces:
    // no effect
    break;
  case Bonus::TruncatedBlocks:
    visualEffects.semicubes.disable();
    break;
  case Bonus::NoHint:
    visualEffects.noHint.disable();
    break;
  case Bonus::SpeedUp:
    // no effect
    break;
//  case Bonus::FlipField:
//    // no effect *at this moment*
//    break;
  case Bonus::None: ;
  default: break;  // TODO(Andrei): handle all values
  }
}

void Player::stealPiece()
{
  if (victim() == NULL)
    return;
  PieceTheftEffect pieceTheftEffect;
  pieceTheftEffect.enable(currentTime());
  pieceTheftEffect.sender = number;
  pieceTheftEffect.target = victimNumber;
  game->globalEffects.pieceThefts.push_back(pieceTheftEffect);
  visualEffects.pieceTheftPtr = &game->globalEffects.pieceThefts.back();
}

}  // namespace engine
