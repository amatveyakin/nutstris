#include "piece.h"

#include <fstream>

#include "engine/strings.h"


namespace engine {

const char PIECE_TEMPLATE_FREE_CHAR  = '.';
const int  N_PIECE_ROTATION_STATES = 4;   // TODO(Andrei): Make it dynamic

// TODO(Andrei): rewrite it cleaner
std::vector<PieceTemplate> loadPieces() {
  std::vector<PieceTemplate> pieceTemplates;
  std::ifstream piecesFile(PIECE_TEMPLATES_FILE);
  if (!piecesFile.good())
    throw std::runtime_error(ERR_FILE_NOT_FOUND);

  pieceTemplates.clear();
  std::vector<std::string> pieceBlock(MAX_PIECE_SIZE);
  int nPieceTemplates;
  piecesFile >> nPieceTemplates;
  pieceTemplates.resize(nPieceTemplates);

  for (int iPiece = 0; iPiece < nPieceTemplates; ++iPiece) {
    piecesFile >> pieceTemplates[iPiece].chance;
    double r, g, b;
    piecesFile >> r >> g >> b;
    pieceTemplates[iPiece].color = colorFromFloat(r, g, b, 1.f);

    for (int rotationState = 0; rotationState < N_PIECE_ROTATION_STATES; ++rotationState) {
      for (int row = MAX_PIECE_SIZE - 1; row >= 0; --row)
        piecesFile >> pieceBlock[row];

      int nBlockInCurrentPiece = 0;
      int maxBlockNumber = -1;
      for (int row = 0; row < MAX_PIECE_SIZE; ++row) {
        for (int col = 0; col < MAX_PIECE_SIZE; ++col) {
          if (pieceBlock[row][col] != PIECE_TEMPLATE_FREE_CHAR) {
            if ((pieceBlock[row][col] < '0') || (pieceBlock[row][col] > '9'))
              throw std::runtime_error(ERR_FILE_CORRUPTED);
            maxBlockNumber = math::max(maxBlockNumber, pieceBlock[row][col] - '0');
            ++nBlockInCurrentPiece;
          }
        }
      }
      if (maxBlockNumber + 1 != nBlockInCurrentPiece)
        throw std::runtime_error(ERR_FILE_CORRUPTED);

      std::vector<FieldCoords> blocks(nBlockInCurrentPiece);
      for (int row = 0; row < MAX_PIECE_SIZE; ++row) {
        for (int col = 0; col < MAX_PIECE_SIZE; ++col) {
          if (pieceBlock[row][col] != PIECE_TEMPLATE_FREE_CHAR)
            blocks[pieceBlock[row][col] - '0'] = FieldCoords(col - CENTRAL_PIECE_COL, row - CENTRAL_PIECE_ROW);
        }
      }
      pieceTemplates[iPiece].structure.emplace_back(blocks);
    }
  }
  return pieceTemplates;
}

}  // namespace engine
