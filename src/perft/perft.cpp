
#include "./perft.h"
#include "../chess_board/chess_board.h"
#include <iostream>
#include <map>

Perft::Perft() : board() {};

uint64_t Perft::perft(int depth, bool isRoot) {
  if (depth == 0)
    return 1;

  std::vector<Move> moves = board.generateMoves();
  uint64_t nodes = 0;

  // Print moves and their counts if we're at the top level
  if (isRoot) { // Only print at the root of our search
    std::map<std::string, Move> sortedMoves;
    for (const Move &move : moves) {
      std::string moveStr;
      moveStr += 'a' + (move.from % 8);
      moveStr += std::to_string(1 + (move.from / 8));
      moveStr += 'a' + (move.to % 8);
      moveStr += std::to_string(1 + (move.to / 8));
      sortedMoves[moveStr] = move;
    }

    for (auto &pair : sortedMoves) {
      if (board.makeMove(pair.second)) {
        uint64_t moveNodes =
            perft(depth - 1, false); // Pass false for recursive calls
        std::cout << pair.first << ": " << moveNodes << std::endl;
        nodes += moveNodes;
        board.unmakeMove();
      }
    }
    std::cout << "\nNodes searched: " << nodes << std::endl;
  } else {
    for (Move move : moves) {
      if (board.makeMove(move)) {
        nodes += perft(depth - 1, false);
        board.unmakeMove();
      }
    }
  }
  return nodes;
}
