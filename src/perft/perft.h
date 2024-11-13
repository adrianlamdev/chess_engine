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
   * @return Number of leaf nodes for the given depth
   */
  uint64_t perft(int depth);

  /**
   * Calculates the number of leaf nodes for the given depth, and prints the
   * number of nodes at each depth.
   *
   * @param depth Depth to calculate perft for
   * @return Number of leaf nodes for the given depth
   */
  uint64_t perftDebug(uint8_t depth);
};

#endif
