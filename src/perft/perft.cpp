
#include "./perft.h"
#include "../chess_board/chess_board.h"
#include <iostream>

Perft::Perft() : board() {};

uint64_t Perft::perft(int depth) {
  if (depth == 0)
    return 1;

  std::vector<Move> moves = board.generateMoves();

  uint64_t nodes = 0;
  for (Move move : moves) {
    if (board.makeMove(move)) {
      nodes += perft(depth - 1);
      board.unmakeMove();
    }
  }
  return nodes;
}

uint64_t Perft::perftDebug(uint8_t depth) {
  if (depth == 0)
    return 1ULL;

  std::vector<Move> moves = board.generateMoves();
  std::cout << "\nCurrent position at depth " << (int)depth << ":\n";
  board.display(false);
  std::cout << "Number of moves: " << moves.size() << "\n";

  uint64_t total_nodes = 0;
  for (Move move : moves) {
    if (board.makeMove(move)) {
      uint64_t current_nodes = (depth == 1) ? 1 : perft(depth - 1);
      total_nodes += current_nodes;
      board.unmakeMove();
      std::cout << "Move " << (int)move.from << "->" << (int)move.to
                << " produces " << current_nodes << " nodes\n";
    }
  }

  std::cout << "Total nodes at depth " << (int)depth << ": " << total_nodes
            << "\n";
  return total_nodes;
}
