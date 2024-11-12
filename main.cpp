#include "./src/chess_board/chess_board.h"
#include "./src/magics/magics.h"
#include <iostream>

int main() {
  ChessBoard board = ChessBoard();
  board.display();

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

  std::cout << "Rook mask on a1:\n";
  board.displayBitboard(getRookMask(0));

  std::cout << "Rook mask on b8:\n";
  board.displayBitboard(getRookMask(57));

  std::cout << "Rook mask on d4:\n";
  board.displayBitboard(getRookMask(27));

  std::cout << "Rook mask on c3:\n";
  board.displayBitboard(getRookMask(18));

  std::cout << "Bishop mask on c1:\n";
  board.displayBitboard(getBishopMask(2));

  std::cout << "Bishop mask on a1:\n";
  board.displayBitboard(getBishopMask(0));

  std::cout << "Bishop mask on d4:\n";
  board.displayBitboard(getBishopMask(27));

  std::cout << "Queen mask on d4:\n";
  board.displayBitboard(getQueenMask(27));

  std::cout << "Queen mask on a1:\n";
  board.displayBitboard(getQueenMask(0));
}
