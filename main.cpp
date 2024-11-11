#include "./src/chess_board/chess_board.h"

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
}
