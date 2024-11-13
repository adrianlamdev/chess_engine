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

/*
 * Represents the full game state including castling rights, en passant square,
 * and half/full move counters.
 */
struct BoardState {
  uint8_t enPassantSquare;
  bool sideToMove;
  std::string castlingRights;
  uint8_t halfMoveClock;
  uint16_t fullMoveNumber;
  Piece capturedPiece;
  std::vector<uint64_t> positions;
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
  std::vector<BoardState> stateHistory;

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
  std::string castlingRights; /// "KQkq" format - uppercase for white
  uint8_t halfMoveClock;      /// Counts moves for 50-move rule
  uint16_t fullMoveNumber;    /// Incremented after black's move
  std::vector<uint64_t> positions;

  std::array<Piece, 64> board; /// 8x8 array representation
  std::vector<Move> moves;     /// Legal moves in current position

  // Pre-computed attack lookup tables
  uint64_t knight_attacks[64];  /// Knight move patterns for each square
  uint64_t pawn_attacks[2][64]; /// Pawn attacks for each color/square
  uint64_t king_attacks[64];

  /**
   * Checks if specified square is attacked by any enemy pieces
   *
   * @param square to check for attacks
   * @param byWhite True to check for attacks by white pieces
   * @return true if square is attacked
   */
  bool isSquareAttacked(int square, bool byWhite) const;

  /**
   * Checks if a side's king is in check
   *
   * @param white True to check white king, false for black
   * @return true if king is in check
   */
  bool isInCheck(bool white) const;

  /**
   * Tests if a move would leave king in check
   *
   * @param move Move to check
   * @return true if move is legal with checks
   */
  bool testMove(Move &move);

  /**
   * Validates consistency between bitboard and 8x8 array representations.
   *
   * @param Move to evaluate
   */
  bool isMoveLegal(Move &move);

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
   * Generates pseudo-legal rook moves for the given rooks.
   *
   * @param rooks Bitboard of rook positions to generate moves for
   * @param ownPieces Bitboard of all friendly pieces (for blocking)
   * @param enemyPieces Bitboard of all enemy pieces (for captures)
   */
  void generateRookMoves(uint64_t rooks, uint64_t ownPieces,
                         uint64_t enemyPieces);

  /**
   * Generates pseudo-legal bishop moves for the given bishops.
   *
   * @param bishops Bitboard of bishop positions to generate moves for
   * @param ownPieces Bitboard of all friendly pieces (for blocking)
   * @param enemyPieces Bitboard of all enemy pieces (for captures
   */
  void generateBishopMoves(uint64_t bishops, uint64_t ownPieces,
                           uint64_t enemyPieces);

  /**
   * Generates pseudo-legal queen moves for the given queens.
   *
   * @param queens Bitboard of queen positions to generate moves for
   * @param ownPieces Bitboard of all friendly pieces (for blocking)
   * @param enemyPieces Bitboard of all enemy pieces (for captures)
   */
  void generateQueenMoves(uint64_t queens, uint64_t ownPieces,
                          uint64_t enemyPieces);

  void generateKingMoves(uint64_t king, uint64_t ownPieces,
                         uint64_t enemyPieces);

  /**
   * Initializes pawn attack lookup table for both colors.
   */
  void initPawnAttacks();

  /**
   * Initializes knight movement lookup table.
   */
  void initKnightAttacks();

  /**
   * Initializes king movement lookup table.
   */
  void initKingAttacks();

  /**
   * Checks if current position has insufficient material for checkmate.
   */
  bool hasInsufficientMaterial() const;

public:
  bool doMove(Move &move);

  bool sideToMove; /// false = white, true = black

  /**
   * Constructs a chess board in the standard starting position.
   * Initializes attack tables and resets game state.
   */
  ChessBoard();

  /**
   * Checks if current position is a checkmate or stalemate
   *
   * @return true if game is over
   */
  bool isCheckmate() const;

  /**
   * Checks if current position is a stalemate
   *
   * @return true if game is a stalemate
   */
  bool isStalemate() const;

  /**
   * Constructs a chess board from a given FEN string.
   * Initializes attack tables and resets game state.
   *
   * @param move Move to make
   */
  bool makeMove(Move &move);

  /**
   * Undoes the last move made on the board.
   * Restores the board state to the previous position
   *
   */
  void unmakeMove();

  /**
   * Resets the board to the standard starting position.
   * Initializes both representations and game state variables.
   */
  void reset();

  /**
   * Displays the current board state.
   * Shows ASCII board and raw 8x8 array values for debugging.
   *
   * @param debug True to show game state variables
   */
  void display(bool debug) const;

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
   * Shows all legal king moves from given square.
   * @param square Square index (0-63) to show moves from
   */
  void displayKingAttacks(int square) const;

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
  std::vector<Move> generateMoves();

  void displayBitboard(uint64_t bitboard) const;
};

#endif
