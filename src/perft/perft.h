#ifndef PERFT_H
#define PERFT_H
#include "../chess_board/chess_board.h"
#include <cstdint>

class Perft {
private:
  ChessBoard board;

public:
  Perft();

  /**
   * Calculates the number of leaf nodes for the given depth.
   *
   * @param depth Depth to calculate perft for
   * @param isRoot True if this is the root call
   * @return Number of leaf nodes for the given depth
   */
  uint64_t perft(int depth, bool isRoot = true);
};

#endif
