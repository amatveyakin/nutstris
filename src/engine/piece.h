#pragma once

#include <vector>
#include <stdexcept>

#include "engine/defs.h"


namespace engine {

const int    MAX_PIECE_SIZE = 4;

const int    CENTRAL_PIECE_ROW = (MAX_PIECE_SIZE - 1) / 2; // is it necessary (?)
const int    CENTRAL_PIECE_COL = (MAX_PIECE_SIZE - 1) / 2; // is it necessary (?)

const int    NO_BLOCK_IMAGE = -1;
const int    NO_CHANGE = -2;


struct Bounds
{
  int bottom = 0;
  int top = 0;
  int left = 0;
  int right = 0;
};


struct BlockStructure
{
  explicit BlockStructure(const std::vector<FieldCoords>& blocks__)
    : blocks(blocks__)
    , bounds(computeBounds(blocks__)) {}

  const std::vector<FieldCoords> blocks;
  const Bounds bounds;

private:
  static Bounds computeBounds(const std::vector<FieldCoords>& blocks__) {
    Bounds bounds;
    if (blocks__.empty())
      throw std::runtime_error("Empty piece found");
    bounds.bottom = blocks__[0].y();
    bounds.top = blocks__[0].y();
    bounds.left = blocks__[0].x();
    bounds.right = blocks__[0].x();
    for (auto& b : blocks__)
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
    return bounds;
  }
};


struct PieceTemplate {
  Color color;
  int chance;
  std::vector<BlockStructure> structure;
};


class Piece {
public:
  Piece() {}
  Piece(const PieceTemplate* pieceTemplate__)
    : pieceTemplate_(pieceTemplate__) {}

  bool valid() const {
    return pieceTemplate_ != nullptr;
  }

  const PieceTemplate* pieceTemplate() const {
    return pieceTemplate_;
  }

  Color color() const {
    assert(pieceTemplate_);
    return pieceTemplate_->color;
  }

  const BlockStructure& currentStructure() const {
    assert(pieceTemplate_);
    return pieceTemplate_->structure[rotationState_];
  }

  size_t nBlocks() const {
    return currentStructure().blocks.size();
  }

  int nRotationStates() const {
    assert(pieceTemplate_);
    return pieceTemplate_->structure.size();
  }
  int rotationState() const {
    return rotationState_;
  }
  void setRotationState(int newRotationState) {
    rotationState_ = newRotationState;
  }

  FieldCoords position() const {
    return position_;
  }
  void moveTo(FieldCoords newPosition) {
    position_ = newPosition;
  }
  void moveBy(FieldCoords delta) {
    position_ += delta;
  }
  FieldCoords relativeCoords(int index) const {
    return currentStructure().blocks[index];
  }
  FieldCoords absoluteCoords(int index) const {
    return relativeCoords(index) + position_;
  }

private:
  const PieceTemplate* pieceTemplate_ = nullptr;
  int rotationState_ = 0;
  FieldCoords position_;   // ``center'' coordinates
};


enum FallingPieceState { psAbsent, psNormal, psDropping };


std::vector<PieceTemplate> loadPieces();

}  // namespace engine
