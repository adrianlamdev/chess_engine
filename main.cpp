#include "./src/chess_board/chess_board.h"
#include "./src/magics/magics.h"
#include <iostream>

int main() {
  ChessBoard board = ChessBoard();
  board.display();

  // testing
  board.displayKnightAttacks(7); // h1 corner
  board.displayKnightAttacks(10);
  board.displayKnightAttacks(34); // middle of board
                                  //
  board.displayPawnAttacks(0, 8); // a2
  board.displayPawnAttacks(0, 9);
  board.displayPawnAttacks(1, 54);

  std::cout << "Rook mask on a1:\n";
  board.displayBitboard(getRookMask(0));

  std::cout << "Rook mask on d3:\n";
  board.displayBitboard(getRookMask(27));

  std::cout << "Rook mask on c2:\n";
  board.displayBitboard(getRookMask(18));

  /*std::cout << "Bishop mask on c1:\n";*/
  /*board.displayBitboard(getBishopMask(2));*/
}
