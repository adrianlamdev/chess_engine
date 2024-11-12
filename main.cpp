#include "./src/chess_board/chess_board.h"
#include "./src/magics/magics.h"
#include <iostream>

int main() {
  ChessBoard board = ChessBoard();
  board.display();

  while (true) {
    std::cout << "Enter move (from to): ";
    int from, to;
    if (!(std::cin >> from >> to)) {
      // Handle invalid input
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Invalid input format. Please enter two numbers.\n";
      continue;
    }

    // Check input ranges
    if (from < 0 || from > 63 || to < 0 || to > 63) {
      std::cout << "Invalid square numbers. Must be between 0 and 63.\n";
      continue;
    }

    Move move = Move{(uint8_t)from, (uint8_t)to};
    if (board.makeMove(move)) {
      if (board.isCheckmate()) {
        std::cout << "Checkmate! " << (board.sideToMove ? "White" : "Black")
                  << " wins!\n";
        break;
      } else if (board.isStalemate()) {
        std::cout << "Game drawn by stalemate!\n";
        break;
      }
      board.display();
    }
  }

  // testing
  /*board.displayKnightAttacks(7); // h1 corner*/
  /*board.displayKnightAttacks(10);*/
  /*board.displayKnightAttacks(34); // middle of board*/
  /*                                //*/
  /*board.displayPawnAttacks(0, 8); // a2*/
  /*board.displayPawnAttacks(0, 9);*/
  /*board.displayPawnAttacks(1, 54);*/

  /*board.displayKingAttacks(4);  // e1*/
  /*board.displayKingAttacks(0);  // a1*/
  /*board.displayKingAttacks(63); // h8*/

  /*std::cout << "Rook mask on a1:\n";*/
  /*board.displayBitboard(getRookMask(0));*/
  /**/
  /*std::cout << "Rook mask on b8:\n";*/
  /*board.displayBitboard(getRookMask(57));*/
  /**/
  /*std::cout << "Rook mask on d4:\n";*/
  /*board.displayBitboard(getRookMask(27));*/
  /**/
  /*std::cout << "Rook mask on c3:\n";*/
  /*board.displayBitboard(getRookMask(18));*/
  /**/
  /*std::cout << "Bishop mask on c1:\n";*/
  /*board.displayBitboard(getBishopMask(2));*/
  /**/
  /*std::cout << "Bishop mask on a1:\n";*/
  /*board.displayBitboard(getBishopMask(0));*/
  /**/
  /*std::cout << "Bishop mask on d4:\n";*/
  /*board.displayBitboard(getBishopMask(27));*/
  /**/
  /*std::cout << "Queen mask on d4:\n";*/
  /*board.displayBitboard(getQueenMask(27));*/
  /**/
  /*std::cout << "Queen mask on a1:\n";*/
  /*board.displayBitboard(getQueenMask(0));*/
}
