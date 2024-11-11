#include "chess_board.h"
#include <iostream>

ChessBoard::ChessBoard() { reset(); }

void ChessBoard::reset() {
  sideToMove = 0;
  castlingRights = "KQkq";
  halfMoveClock = 0;
  fullMoveNumber = 1;

  // bitboard representation
  // White pieces on ranks 1-2 (bottom)
  whitePawns = 0x000000000000FF00;
  whiteKnights = 0x0000000000000042;
  whiteBishops = 0x0000000000000024;
  whiteRooks = 0x0000000000000081;
  whiteQueens = 0x0000000000000008;
  whiteKing = 0x0000000000000010;

  // Black pieces on ranks 7-8 (top)
  blackPawns = 0x00FF000000000000;
  blackKnights = 0x4200000000000000;
  blackBishops = 0x2400000000000000;
  blackRooks = 0x8100000000000000;
  blackQueens = 0x0800000000000000;
  blackKing = 0x1000000000000000;

  // 8x8 board array
  for (int i = 0; i < 64; i++) {
    board[i] = Piece::Empty;
  }

  // first rank
  board[0] = Piece::WhiteRook;
  board[1] = Piece::WhiteKnight;
  board[2] = Piece::WhiteBishop;
  board[3] = Piece::WhiteQueen;
  board[4] = Piece::WhiteKing;
  board[5] = Piece::WhiteBishop;
  board[6] = Piece::WhiteKnight;
  board[7] = Piece::WhiteRook;

  // second rank
  for (int i = 8; i < 16; i++) {
    board[i] = Piece::WhitePawn;
  }

  // seventh rank
  for (int i = 48; i < 56; i++) {
    board[i] = Piece::BlackPawn;
  }

  // eighth rank
  board[56] = Piece::BlackRook;
  board[57] = Piece::BlackKnight;
  board[58] = Piece::BlackBishop;
  board[59] = Piece::BlackQueen;
  board[60] = Piece::BlackKing;
  board[61] = Piece::BlackBishop;
  board[62] = Piece::BlackKnight;
  board[63] = Piece::BlackRook;
}

uint64_t ChessBoard::getWhitePieces() const {
  return whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens |
         whiteKing;
}

uint64_t ChessBoard::getBlackPieces() const {
  return blackPawns | blackKnights | blackBishops | blackRooks | blackQueens |
         blackKing;
}

void ChessBoard::display() const {
  // Display bitboard representation
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << (rank + 1) << " ";
    for (int file = 0; file < 8; file++) {
      int square = rank * 8 + file;
      uint64_t bit = 1ULL << square;
      char piece = '.';
      if (whitePawns & bit)
        piece = 'P';
      if (whiteKnights & bit)
        piece = 'N';
      if (whiteBishops & bit)
        piece = 'B';
      if (whiteRooks & bit)
        piece = 'R';
      if (whiteQueens & bit)
        piece = 'Q';
      if (whiteKing & bit)
        piece = 'K';
      if (blackPawns & bit)
        piece = 'p';
      if (blackKnights & bit)
        piece = 'n';
      if (blackBishops & bit)
        piece = 'b';
      if (blackRooks & bit)
        piece = 'r';
      if (blackQueens & bit)
        piece = 'q';
      if (blackKing & bit)
        piece = 'k';
      std::cout << piece << ' ';
    }
    std::cout << '\n';
  }
  std::cout << "  a b c d e f g h" << std::endl;

  // Display raw 8x8 values
  std::cout << "\n8x8 values:\n";
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << (rank + 1) << " ";
    for (int file = 0; file < 8; file++) {
      int square = rank * 8 + file;
      std::cout << int(board[square]) << "\t";
    }
    std::cout << '\n';
  }
  std::cout << "  a b c d e f g h" << std::endl;
};
