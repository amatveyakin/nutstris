#include "player.h"

#include <iostream>
#include <cstdio>

#include "engine/game.h"
#include "engine/io/sound.h"
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

Player::Player(const PlayerInfo* info__, GameRound* game__)
  : info_(info__)
  , game_(game__)
{
  for (int key = 0; key < kNumPlayerControls; ++key)
    nextKeyActivationTable_[key] = Time::min();

  visualEffects.lanternObject.bindTo(&fallingPieceFrame);
  visualEffects.lanternObject.placeAt(FloatFieldCoords((FIELD_WIDTH  - 1.0) / 2.0, (FIELD_HEIGHT - 1.0) / 2.0));
  visualEffects.lanternObject.setMaxSpeed(BONUS_LANTERN_MAX_SPEED);
  latestLineCollapse_ = Time::min();

  victimId_ = id();

  speed_ = STARTING_SPEED;

  backgroundSeed = rand();
  events_.push(etRoutineSpeedUp, currentTime() + ROUTINE_SPEED_UP_INTERVAL);
  resizePieceQueue(NORMAL_HINT_QUEUE_SIZE);
  planBonusAppearance();
  sendNewPiece();
}

Time Player::currentTime()
{
  return game_->currentTime();
}

Time Player::pieceLoweringInterval()
{
  return AUTO_LOWERING_TIME / speed_;
}

int Player::id() const {
  return info_->id();
}

std::string Player::name() const {
  return info_->name();
}

bool Player::active() const {
  return active_;
}

const std::vector<Piece>& Player::nextPieces() const {
  return nextPieces_;
}

int Player::victimId() const {
  return victimId_;
}

Player* Player::victim() const {
  return (victimId_ != id()) ? game_->playerById(victimId_) : nullptr;
}

void Player::takesBonus(Bonus bonus)
{
  playSound(Sound::BonusTaken);
  if (isKind(bonus))
    applyBonus(bonus);
  else
  {
    if (victim())
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
    events_.push(etHeal, currentTime());
//    heal();
    break;
  case Bonus::SlowDown:
    bonusSlowDown();
    break;
  case Bonus::ClearField:
    events_.push(etBeginClearField, currentTime());
    break;
  case Bonus::SpeedUp:
    bonusSpeedUp();
    break;
  default: break;  // TODO(Andrei): handle all values
  }
  if (isPermanent(bonus))
    bonuces_.add(bonus);
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
  bonuces_.remove(bonus);
  disableBonusVisualEffect(bonus);
}

void Player::heal()
{
  for (Bonus bonus : ForEachBonus()) {
    if (isDebuff(bonus) && bonuces_.test(bonus))
      disenchant(bonus);
  }
}

void Player::beginClearField()
{
  //  TODO: also handle a case when the bonus is taken somehow but a piece is still falling (?)
  fieldLocks_.isBeingCleared = true;
  field_ = Field();
  visualEffects.fieldCleaning.enable(currentTime());
  events_.push(etEndClearField, currentTime() + BONUS_CLEAR_FIELD_DURATION);
}

void Player::endClearField()
{
  fieldLocks_.isBeingCleared = false;
  lyingBlockImages.clear();
}

void Player::kill()
{
  events_.clear();
  game_->deactivatePlayer(id());
  visualEffects.playerDying.enable(currentTime());
  active_ = false;
}

void Player::cycleVictim()
{
  if (game_->activePlayers().size() < 2)
    return;
  do
  {
    victimId_ = (victimId_ + 1) % MAX_PLAYERS;
  } while (!(game_->playerById(victimId_) && game_->playerById(victimId_)->active_));
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
    events_.eraseEventType(etPieceLowering);
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

void Player::processInput() {
  for (int key = 0; key < kNumPlayerControls; ++key) {
    if (Keyboard::isKeyPressed(info_->controls()[key]) &&
        (currentTime() >= nextKeyActivationTable_[key])) {
      onKeyPress(PlayerControl(key));
      nextKeyActivationTable_[key] = currentTime() + playerControlCooldown(PlayerControl(key));
    }
    else if (!Keyboard::isKeyPressed(info_->controls()[key]))
      nextKeyActivationTable_[key] = currentTime();
  }
}

void Player::updateObjects() {
  visualEffects.lanternObject.update(currentTime());
  fallingPieceFrame.update(currentTime());
  for (auto& blockItem : lyingBlockImages)
    blockItem.second.update(currentTime());
  for (BlockImage& block : fallingBlockImages)
    block.update(currentTime());
}

void Player::onTimer()
{
  processInput();
  updateObjects();

  while ((!events_.empty()) && (currentTime() >= events_.top().activationTime))
  {
    Event currentEvent = events_.top();
    events_.pop();
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
    case etBonusDisappearanceAnimationStart:
      startBonusDisappearanceAnimations();
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
      throw std::runtime_error(ERR_INTERNAL_ERROR);
    }
    if (eventDelayed)
    {
      currentEvent.activationTime += EVENT_DELAY_TIME;
      events_.push(currentEvent);
    }
  }
}



int Player::highestNonemptyRow() const
{
  for (int row = FIELD_HEIGHT - 1; row >= 0; --row)
    for (int col = 0; col < FIELD_WIDTH; ++col)
      if (field_({col, row}).blocked())
        return row;
  return -1;
}

bool Player::canDisposePiece(FieldCoords position, const BlockStructure& piece) const
{
  for (size_t i = 0; i < piece.blocks.size(); ++i)
    if (field_(position + piece.blocks[i]).blocked())
      return false;
  return true;
}

bool Player::fallingPieceCannotReachSky() const
{
  for (int i = 0; i < fallingPiece_.nRotationStates(); ++i)
    if (fallingPiece_.pieceTemplate()->structure[i].bounds.top + fallingPiece_.position().y() >= FIELD_HEIGHT)
      return false;
  return true;
}

bool Player::canSendNewPiece() const
{
  return disappearingLines.empty() && !fieldLocks_.isBeingCleared;
}

Piece Player::randomPiece() const
{
  int pieceTemplatesIndex = game_->basics().randomPieceTable[rand() % game_->basics().randomPieceTable.size()];
  Piece piece(&game_->basics().pieceTemplates[pieceTemplatesIndex]);
  piece.setRotationState(rand() % piece.nRotationStates());
  piece.moveTo({MAX_PIECE_SIZE + rand() % (FIELD_WIDTH - 2 * MAX_PIECE_SIZE),  // TODO: modify the formula
                FIELD_HEIGHT - piece.currentStructure().bounds.bottom});
  return piece;
}

bool Player::bonusIsUseful(Bonus bonus) const
{
  // TODO: process all debuffs uniformly  (may be, they are just always useful?)
  if (isBuff(bonus))
    return !bonuces_.test(bonus);

  switch (bonus)
  {
  case Bonus::None:
    return false;
  case Bonus::Heal:
    for (Bonus b : ForEachBonus()) {
      if (isDebuff(b) && bonuces_.test(b))
        return true;
    }
    return false;
  case Bonus::SlowDown:
    return speed_ > STARTING_SPEED + BONUS_SLOW_DOWN_VALUE;  // (?)
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
    bonus = game_->basics().randomBonusTable[rand() % game_->basics().randomBonusTable.size()];
    if (bonusIsUseful(bonus))
      return bonus;
  }
  return Bonus::None;
}

void Player::setUpPiece()
{
  for (size_t i = 0; i < fallingPiece_.nBlocks(); ++i)
  {
    FieldCoords cell = fallingPiece_.absoluteCoords(i);
    if (cell.y() >= FIELD_HEIGHT)
    {
      events_.push(etKill, currentTime());
      return;
    }

    // TODO: Make falling pieces look differently. Add materialization animation.
    field_.mutableCell(cell).setBlock(fallingPiece_.color());
    util::mapInsertUnique(lyingBlockImages, {cell, BlockImage(nullptr, fallingPiece_.color(), cell)});
  }
  fallingBlockImages.clear();
  fallingPieceState_ = psAbsent;

  int nLinesRemoved = removeFullLines();
  if (nLinesRemoved)
    playSoundRepeatedly(Sound::LineCompleted, nLinesRemoved, kLineCompletedSoundRepeteationInterval);
  Time newPieceDelay = nLinesRemoved ? std::max(HINT_MATERIALIZATION_TIME, LINE_DISAPPEAR_TIME) :
                                       HINT_MATERIALIZATION_TIME;
  events_.pushWithUniquenessCheck(etNewPiece, currentTime() + newPieceDelay);
  visualEffects.hintMaterialization.setDuration(newPieceDelay);
  visualEffects.hintMaterialization.enable(currentTime());
  visualEffects.hintAppearance.enable(currentTime());
//  visualEffects.lantern.resetBinding();
}

void Player::resizePieceQueue(int newSize)
{
  int oldSize = nextPieces_.size();
  nextPieces_.resize(newSize);
  for (int i = oldSize; i < newSize; ++i)
    nextPieces_[i] = randomPiece();
}

bool Player::sendNewPiece()
{
  if (!canSendNewPiece())
    return false;

  fallingPiece_ = nextPieces_[0];
  assert(fallingPiece_.valid());

  visualEffects.hintAppearance.reset(currentTime());
  visualEffects.hintMaterialization.reset(currentTime());

  fallingPieceState_ = psNormal;
  fallingPieceFrame.placeAt(FloatFieldCoords(fallingPiece_.position()));
  for (size_t i = 0; i < fallingPiece_.nBlocks(); ++i)
    fallingBlockImages.push_back(BlockImage(&fallingPieceFrame, fallingPiece_.color(), fallingPiece_.relativeCoords(i)));
//  visualEffects.lantern.bindTo(&fallingPieceFrame);
  events_.push(etPieceLowering, currentTime() + pieceLoweringInterval());

  nextPieces_.erase(nextPieces_.begin());
  nextPieces_.push_back(randomPiece());
  return true;
}

void Player::lowerPiece(bool forced)
{
  if (fallingPieceState_ == psAbsent)
    return;

  FieldCoords newPosition = fallingPiece_.position() + FieldCoords(0, -1);
  // We should not forget about the case when a piece gets stuck as the result of
  // field modification. Such pieces should not continue falling.
  if (canDisposePiece(fallingPiece_.position(), fallingPiece_.currentStructure()) &&
      canDisposePiece(newPosition,             fallingPiece_.currentStructure()))
  {
    Time loweringTime = (fallingPieceState_ == psDropping) ? DROPPING_PIECE_LOWERING_TIME :
                        (forced ? PIECE_FORCED_LOWERING_ANIMATION_TIME :
                                  PIECE_AUTO_LOWERING_ANIMATION_TIME);
    fallingPieceFrame.addMotion(FloatFieldCoords(newPosition - fallingPiece_.position()), currentTime(), loweringTime);

    fallingPiece_.moveTo(newPosition);

    if (fallingPieceCannotReachSky())
      visualEffects.hintAppearance.enable(currentTime());

    events_.push(etPieceLowering,
                currentTime() +
                ((fallingPieceState_ == psDropping) ?
                 DROPPING_PIECE_LOWERING_TIME : pieceLoweringInterval()));
  }
  else
    setUpPiece();
}

int Player::removeFullLines() {
  int nLinesRemoved = 0;

  for (int row = 0; row < FIELD_HEIGHT; ++row) {
    bool rowIsFull = true;
    for (int col = 0; col < FIELD_WIDTH; ++col) {
      if (!field_({col, row}).blocked()) {
        rowIsFull = false;
        break;
      }
    }

    if (rowIsFull) {
      ++nLinesRemoved;

      std::vector<BlockImage> disappearingBlockImages;
      for (int col = 0; col < FIELD_WIDTH; ++col) {
        if (field_({col, row}).bonus() != Bonus::None) {
          takesBonus(field_({col, row}).bonus());
          events_.eraseEventType(etBonusDisappearance);
          planBonusAppearance();
        }
        field_.mutableCell({col, row}).clear();
        disappearingBlockImages.push_back(util::mapExtract(lyingBlockImages, {col, row}));
      }

      disappearingLines.push_back({row, std::move(disappearingBlockImages), currentTime()});

      if (latestLineCollapse_ < currentTime() + LINE_DISAPPEAR_TIME)
        latestLineCollapse_ = currentTime() + LINE_DISAPPEAR_TIME;
      latestLineCollapse_ += LINE_COLLAPSE_TIME;
      Event event(etLineCollapse, latestLineCollapse_);
      event.parameters.lineCollapse.row = row;
      events_.push(event);
    }
  }

  return nLinesRemoved;
}

void Player::collapseLine(int row)
{
  for (int curRow = row; curRow < FIELD_HEIGHT; ++curRow)
  {
    for (int col = 0; col < FIELD_WIDTH; ++col)
    {
      field_.mutableCell({col, curRow}) = field_({col, curRow + 1});
      if (field_({col, curRow}).blocked())
        moveLyingBlockImage(FieldCoords(col, curRow + 1), FieldCoords(col, curRow), LINE_COLLAPSE_ANIMATION_TIME);
    }
  }

  for (auto i = disappearingLines.begin();
       i != disappearingLines.end(); ++i)
  {
    if (i->row() == row)
    {
      disappearingLines.erase(i);
      break;
    }
  }
  for (auto i = disappearingLines.begin();
       i != disappearingLines.end(); ++i)
    if (i->row() > row)
      --i->row();
  for (auto myEvent = events_.begin(); myEvent != events_.end(); ++myEvent)
    if ((myEvent->type == etLineCollapse) && (myEvent->parameters.lineCollapse.row > row))
      --const_cast<int&>(myEvent->parameters.lineCollapse.row);
}

void Player::movePiece(int direction)
{
  if (fallingPieceState_ != psNormal)
    return;
  FieldCoords newPosition = fallingPiece_.position() + FieldCoords(direction, 0);
  if (canDisposePiece(newPosition, fallingPiece_.currentStructure()))
  {
    fallingPieceFrame.addMotion(FloatFieldCoords(newPosition - fallingPiece_.position()), currentTime(), PIECE_MOVING_ANIMATION_TIME);
    fallingPiece_.moveTo(newPosition);
  }
}

void Player::dropPiece()
{
  if (fallingPieceState_ != psNormal)
    return;
  fallingPieceState_ = psDropping;
  events_.eraseEventType(etPieceLowering);
  events_.push(etPieceLowering, currentTime() + DROPPING_PIECE_LOWERING_TIME);
}

void Player::rotatePiece(int direction)
{
  if (fallingPieceState_ != psNormal)
    return;

  // TODO: simply create a  Piece  copy instead (?)
  FieldCoords oldPosition = fallingPiece_.position();
  int oldRotationState = fallingPiece_.rotationState();
  int newFallingPieceRotationState = math::modFloored(fallingPiece_.rotationState() + direction, fallingPiece_.nRotationStates());

  if (canDisposePiece(fallingPiece_.position(),
      fallingPiece_.pieceTemplate()->structure[newFallingPieceRotationState]))
  {
    fallingPiece_.setRotationState(newFallingPieceRotationState);
  }
  else if (canDisposePiece(fallingPiece_.position() + FieldCoords(1, 0),
      fallingPiece_.pieceTemplate()->structure[newFallingPieceRotationState]))
  {
    fallingPiece_.moveBy({1, 0});
    fallingPiece_.setRotationState(newFallingPieceRotationState);
  }
  else if (canDisposePiece(fallingPiece_.position() - FieldCoords(1, 0),
      fallingPiece_.pieceTemplate()->structure[newFallingPieceRotationState]))
  {
    fallingPiece_.moveBy({-1, 0});
    fallingPiece_.setRotationState(newFallingPieceRotationState);
  }

  for (size_t i = 0; i < fallingPiece_.nBlocks(); ++i)
  {
    fallingBlockImages[i].addMotion(
            FloatFieldCoords(fallingPiece_.absoluteCoords(i) -
                             (oldPosition + fallingPiece_.pieceTemplate()->structure[oldRotationState].blocks[i])),
            currentTime(), PIECE_ROTATING_ANIMATION_TIME);
  }
}

bool Player::generateBonus()  // TODO: rewrite
{
  Bonus bonus = randomBonus();
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
        if (field_({col, row}).blocked())
        {
          field_.mutableCell({col, row}).setBonus(bonus);
          lyingBlockImages[{col, row}].setBonus(bonus);
          lyingBlockImages[{col, row}].bonusEffect().enable(currentTime());
          planBonusDisappearance();
          return true;
        }
      }
    }
  }
  return false;
}

void Player::startBonusDisappearanceAnimations() {
  for (int row = 0; row < FIELD_HEIGHT; ++row) {
    for (int col = 0; col < FIELD_WIDTH; ++col) {
      if (field_({col, row}).blocked()) {
        lyingBlockImages[{col, row}].bonusEffect().disable(currentTime());
      }
    }
  }
}

void Player::removeBonuses() {
  for (int row = 0; row < FIELD_HEIGHT; ++row) {
    for (int col = 0; col < FIELD_WIDTH; ++col) {
      if (field_({col, row}).blocked()) {
        field_.mutableCell({col, row}).setBonus(Bonus::None);
        lyingBlockImages[{col, row}].bonusEffect().disable(currentTime());
      }
    }
  }
  planBonusAppearance();
}

void Player::planBonusAppearance()
{
  events_.eraseEventType(etBonusDisappearanceAnimationStart);
  events_.eraseEventType(etBonusDisappearance);
  events_.push(etBonusAppearance, currentTime() + randomTimeRange(MIN_BONUS_APPEAR_TIME, MAX_BONUS_APPEAR_TIME));
}

void Player::planBonusDisappearance()
{
  Time bonusDisappearTime = currentTime() + randomTimeRange(MIN_BONUS_LIFE_TIME, MAX_BONUS_LIFE_TIME);
  events_.push(etBonusDisappearance, bonusDisappearTime);
  events_.push(etBonusDisappearanceAnimationStart, bonusDisappearTime - BONUS_FADING_DURATION);
}

void Player::moveLyingBlockImage(FieldCoords movingFrom, FieldCoords movingTo, Time movingDuration) {
  if (movingFrom == movingTo)
    return;
  BlockImage block = util::mapExtract(lyingBlockImages, movingFrom);
  block.addMotion(FloatFieldCoords(movingTo - movingFrom), currentTime(), movingDuration);
  util::mapInsertUnique(lyingBlockImages, {movingTo, std::move(block)});
}

void Player::routineSpeedUp()
{
  double maxSpeed = std::max(NORMAL_SPEED_LIMIT, speed_);
  speed_ += ROUTINE_SPEED_UP_VALUE;
  speed_ = std::min(speed_, maxSpeed);
  events_.push(etRoutineSpeedUp, currentTime() + ROUTINE_SPEED_UP_INTERVAL);
}

void Player::bonusSpeedUp()
{
  speed_ += BONUS_SPEED_UP_VALUE;
  speed_ = std::min(speed_, ABSOLUTE_SPEED_LIMIT);
}

void Player::bonusSlowDown()
{
  speed_ -= BONUS_SLOW_DOWN_VALUE;
  speed_ = std::max(speed_, STARTING_SPEED);
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
    //visualEffects.fieldCleaning.enable(currentTime());  // (!) Change if effect type is changed
    break;
  case Bonus::FlippedScreen:
    visualEffects.flippedScreen.enable(currentTime());
    break;
  case Bonus::RotatingScreen:
    visualEffects.rotatingField.enable(currentTime());
    break;
  case Bonus::Wave:
    visualEffects.wave.enable(currentTime());
    break;
  case Bonus::Lantern:
    visualEffects.lantern.enable(currentTime());
    break;
  case Bonus::CrazyPieces:
    // no effect
    break;
  case Bonus::TruncatedBlocks:
    visualEffects.semicubes.enable(currentTime());
    break;
  case Bonus::NoHint:
    visualEffects.noHint.enable(currentTime());
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
    visualEffects.flippedScreen.disable(currentTime());
    break;
  case Bonus::RotatingScreen:
    visualEffects.rotatingField.disable(currentTime());
    break;
  case Bonus::Wave:
    visualEffects.wave.disable(currentTime());
    break;
  case Bonus::Lantern:
    visualEffects.lantern.disable(currentTime());
    break;
  case Bonus::CrazyPieces:
    // no effect
    break;
  case Bonus::TruncatedBlocks:
    visualEffects.semicubes.disable(currentTime());
    break;
  case Bonus::NoHint:
    visualEffects.noHint.disable(currentTime());
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
  if (!victim())
    return;
  PieceTheftEffect pieceTheftEffect;
  pieceTheftEffect.enable(currentTime());
  pieceTheftEffect.sender = id();
  pieceTheftEffect.target = victimId_;
  game_->globalEffects().pieceThefts.push_back(pieceTheftEffect);
  visualEffects.pieceTheftPtr = &game_->globalEffects().pieceThefts.back();
}

}  // namespace engine
