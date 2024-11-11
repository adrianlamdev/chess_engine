#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <array>
#include <cstdint>
#include <string>

/**
 * Represents different chess pieces using distinct numerical values.
 *
 * Values are chosen to allow easy distinction between piece types and colors:
 * - Empty square = 0
 * - White pieces = 1-8
 * - Black pieces = 9-16
 */
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
 * Represents a chess board with both bitboard and mailbox
 * representations.
 *
 * This class maintains two synchronized board representations:
 * 1. Bitboards for efficient move generation and position evaluation
 * 2. Mailbox array for easier board manipulation and display
 *
 * The board state includes:
 * - Piece positions (both representations)
 * - Side to move
 * - Castling rights
 * - En passant target square
 * - Move counts for draw rules
 */
class ChessBoard {
private:
  // Bitboard representation - one 64-bit integer per piece type
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

  // Game state variables
  uint8_t enPassantSquare;
  bool sideToMove;
  std::string castlingRights;
  uint8_t halfMoveClock;
  uint16_t fullMoveNumber;

  std::array<Piece, 64> board; /// Mailbox representation - one piece per square

  /**
   * Validates the consistency between bitboard and mailbox
   * representations.
   *
   * TODO: Implement validation logic
   */
  void isValid();

public:
  /**
   * Constructs a chess board in the standard starting position.
   */
  ChessBoard();

  /**
   * Resets the board to the standard starting position.
   *
   * Initializes both bitboard and mailbox representations to the standard
   * chess starting position. Also resets all game state variables.
   */
  void reset();

  /**
   * Displays the current board state.
   *
   * Outputs two representations:
   * 1. ASCII diagram of the board with pieces
   * 2. Numerical values from the mailbox array for debugging
   */
  void display() const;

  /**
   * Gets a bitboard of all white pieces.
   *
   * @returns uint64_t Bitboard with 1s in positions of all white pieces
   */
  uint64_t getWhitePieces() const;

  /**
   * Gets a bitboard of all black pieces.
   *
   * @returns uint64_t Bitboard with 1s in positions of all black pieces
   */
  uint64_t getBlackPieces() const;
};

#endif
