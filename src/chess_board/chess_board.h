#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H
#include <array>
#include <cstdint>
#include <string>
#include <vector>

/**
 * Represents a chess move using source and destination squares.
 */
struct Move {
  uint8_t from;
  uint8_t to;
  // TODO: Add promotion, capture flags, etc.
};

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
 * Represents a chess board using both bitboard and 8x8 array representations.
 * Maintains move generation lookup tables and game state.
 *
 * Key features:
 * - Dual board representation (bitboards + 8x8 array)
 * - Pre-computed attack tables for pawns and knights
 * - Move generation for all piece types
 * - Full game state tracking (castling, en passant, etc.)
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
  uint8_t enPassantSquare;    /// Target square for en passant captures
  bool sideToMove;            /// false = white, true = black
  std::string castlingRights; /// "KQkq" format - uppercase for white
  uint8_t halfMoveClock;      /// Counts moves for 50-move rule
  uint16_t fullMoveNumber;    /// Incremented after black's move

  std::array<Piece, 64> board; /// 8x8 array representation
  std::vector<Move> moves;     /// Legal moves in current position

  // Pre-computed attack lookup tables
  uint64_t knight_attacks[64];  /// Knight move patterns for each square
  uint64_t pawn_attacks[2][64]; /// Pawn attacks for each color/square

  /**
   * Validates consistency between bitboard and 8x8 array representations.
   * TODO: Implement validation logic
   */
  void isValid();

  /**
   * Generates pseudo-legal knight moves for the given knights.
   *
   * @param knights Bitboard of knight positions to generate moves for
   * @param ownPieces Bitboard of all friendly pieces (for blocking)
   * @param enemyPieces Bitboard of all enemy pieces (for captures)
   */
  void generateKnightMoves(uint64_t knights, uint64_t ownPieces,
                           uint64_t enemyPieces);

  /**
   * Generates pseudo-legal pawn moves for the given side.
   *
   * @param side Color of pawns (0 = white, 1 = black)
   * @param pawns Bitboard of pawn positions to generate moves for
   * @param ownPieces Bitboard of all friendly pieces (for blocking)
   * @param enemyPieces Bitboard of all enemy pieces (for captures)
   */
  void generatePawnMoves(uint8_t side, uint64_t pawns, uint64_t ownPieces,
                         uint64_t enemyPieces);

  /**
   * Initializes pawn attack lookup table for both colors.
   */
  void initPawnAttacks();

  /**
   * Initializes knight movement lookup table.
   */
  void initKnightAttacks();

public:
  /**
   * Constructs a chess board in the standard starting position.
   * Initializes attack tables and resets game state.
   */
  ChessBoard();

  /**
   * Resets the board to the standard starting position.
   * Initializes both representations and game state variables.
   */
  void reset();

  /**
   * Displays the current board state.
   * Shows ASCII board and raw 8x8 array values for debugging.
   */
  void display() const;

  /**
   * Gets combined bitboard of all white pieces.
   * @return uint64_t Bitboard with white piece positions
   */
  uint64_t getWhitePieces() const;

  /**
   * Gets combined bitboard of all black pieces.
   * @return uint64_t Bitboard with black piece positions
   */
  uint64_t getBlackPieces() const;

  /**
   * Initializes all piece movement lookup tables.
   */
  void initAttacks();

  /**
   * Shows all legal knight moves from given square.
   * @param square Square index (0-63) to show moves from
   */
  void displayKnightAttacks(int square) const;

  /**
   * Shows all legal pawn moves for given side from square.
   * @param side Color (0 = white, 1 = black)
   * @param square Square index (0-63) to show moves from
   */
  void displayPawnAttacks(int side, int square) const;

  /**
   * Shows attack pattern on ASCII board.
   * @param square Source square index (0-63)
   * @param attacks Bitboard of attacked squares
   * @param piece Character to show at source square
   */
  void displayAttacks(int square, uint64_t attacks, char piece) const;

  /**
   * Generates all pseudo-legal moves in current position.
   * Updates internal moves vector.
   */
  void generateMoves();

  void displayBitboard(uint64_t bitboard) const;
};

#endif
