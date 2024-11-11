#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <array>
#include <cstdint>
#include <string>

enum class Piece : uint8_t {
  Empty = 0,
  WhitePawn = 1,
  WhiteKnight = 2,
  WhiteBishop = 3,
  WhiteRook = 4,
  WhiteQueen = 5,
  WhiteKing = 6,
  BlackPawn = 9,
  BlackKnight = 10,
  BlackBishop = 11,
  BlackRook = 12,
  BlackQueen = 13,
  BlackKing = 14
};

/**
 * Represents a chess board using both bitboard and mailbox representations.
 * Maintains game state including castling rights, en passant, and move counts.
 */
class ChessBoard {
private:
  // bitboard representation of the board
  uint64_t whitePawns;
  uint64_t whiteKnights;
  uint64_t whiteBishops;
  uint64_t whiteRooks;
  uint64_t whiteQueens;
  uint64_t whiteKing;
  uint64_t blackPawns;
  uint64_t blackKnights;
  uint64_t blackBishops;
  uint64_t blackRooks;
  uint64_t blackQueens;
  uint64_t blackKing;

  uint8_t enPassantSquare; // TODO: implement this
  bool sideToMove;
  std::string castlingRights;
  uint8_t halfMoveClock;
  uint16_t fullMoveNumber;

  std::array<Piece, 64> board;

  void isValid();

public:
  ChessBoard();

  /**
   * Resets the board to the starting position.
   * Initializes both bitboard and mailbox representations.
   */
  void reset();
  void display() const;

  uint64_t getWhitePieces() const;
  uint64_t getBlackPieces() const;
};

#endif
