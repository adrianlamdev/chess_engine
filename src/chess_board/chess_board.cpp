#include "chess_board.h"
#include "../magics/magics.h"
#include <cstdint>
#include <iostream>

void removeCastlingRights(std::string &rights, char right) {
  size_t pos = rights.find(right);
  if (pos != std::string::npos) {
    rights.erase(pos, 1);
  }
}

ChessBoard::ChessBoard() {
  initMagics();
  initAttacks();
  reset();
}

// TODO: use this later
bool ChessBoard::isSquareAttacked(int square, bool byWhite) const {
  uint64_t potentialPawnLocations = pawn_attacks[byWhite ? 0 : 1][square];
  if (potentialPawnLocations & (byWhite ? whitePawns : blackPawns)) {
    return true;
  }

  uint64_t potentialKnightLocations = knight_attacks[square];
  if (potentialKnightLocations & (byWhite ? whiteKnights : blackKnights)) {
    return true;
  }

  uint64_t potentialKingLocations = king_attacks[square];
  if (potentialKingLocations & (byWhite ? whiteKing : blackKing)) {
    return true;
  }

  uint64_t allPieces = getWhitePieces() | getBlackPieces();

  // Rook + Queen
  uint64_t potentialRookLocations = getRookAttacks(square, allPieces);
  if (potentialRookLocations &
      (byWhite ? (whiteRooks | whiteQueens) : (blackRooks | blackQueens))) {
    return true;
  }

  // Bishop + Queen
  uint64_t potentialBishopLocations = getBishopAttacks(square, allPieces);
  if (potentialBishopLocations &
      (byWhite ? (whiteBishops | whiteQueens) : (blackBishops | blackQueens))) {
    return true;
  }

  return false;
}

bool ChessBoard::isInCheck(bool white) const {
  uint64_t king = white ? whiteKing : blackKing;

  int kingSquare = __builtin_ctzll(king);
  bool isKingAttacked = isSquareAttacked(kingSquare, !white);

  return isKingAttacked;
}

bool ChessBoard::doMove(Move &move) {
  Piece movingPiece = board[move.from];
  Piece capturedPiece = board[move.to];

  uint64_t toBB = 1ULL << move.to;
  uint64_t fromBB = 1ULL << move.from;

  // Remove captured piece if any
  if (capturedPiece != Piece::Empty) {
    switch (capturedPiece) {
    case Piece::WhitePawn:
      whitePawns &= ~toBB;
      break;
    case Piece::WhiteKnight:
      whiteKnights &= ~toBB;
      break;
    case Piece::WhiteBishop:
      whiteBishops &= ~toBB;
      break;
    case Piece::WhiteRook:
      whiteRooks &= ~toBB;
      break;
    case Piece::WhiteQueen:
      whiteQueens &= ~toBB;
      break;
    case Piece::WhiteKing:
      whiteKing &= ~toBB;
      break;
    case Piece::BlackPawn:
      blackPawns &= ~toBB;
      break;
    case Piece::BlackKnight:
      blackKnights &= ~toBB;
      break;
    case Piece::BlackBishop:
      blackBishops &= ~toBB;
      break;
    case Piece::BlackRook:
      blackRooks &= ~toBB;
      break;
    case Piece::BlackQueen:
      blackQueens &= ~toBB;
      break;
    case Piece::BlackKing:
      blackKing &= ~toBB;
      break;
    default:
      break;
    }
  }

  // Move the piece
  switch (movingPiece) {
  case Piece::WhitePawn:
    whitePawns = (whitePawns & ~fromBB) | toBB;
    break;
  case Piece::WhiteKnight:
    whiteKnights = (whiteKnights & ~fromBB) | toBB;
    break;
  case Piece::WhiteBishop:
    whiteBishops = (whiteBishops & ~fromBB) | toBB;
    break;
  case Piece::WhiteRook:
    whiteRooks = (whiteRooks & ~fromBB) | toBB;
    if (move.from == 0)
      removeCastlingRights(castlingRights, 'Q');
    if (move.from == 7)
      removeCastlingRights(castlingRights, 'K');
    break;
  case Piece::WhiteQueen:
    whiteQueens = (whiteQueens & ~fromBB) | toBB;
    break;
  case Piece::WhiteKing:
    whiteKing = (whiteKing & ~fromBB) | toBB;
    removeCastlingRights(castlingRights, 'K');
    removeCastlingRights(castlingRights, 'Q');
    break;
  case Piece::BlackPawn:
    blackPawns = (blackPawns & ~fromBB) | toBB;
    break;
  case Piece::BlackKnight:
    blackKnights = (blackKnights & ~fromBB) | toBB;
    break;
  case Piece::BlackBishop:
    blackBishops = (blackBishops & ~fromBB) | toBB;
    break;
  case Piece::BlackRook:
    blackRooks = (blackRooks & ~fromBB) | toBB;
    if (move.from == 56)
      removeCastlingRights(castlingRights, 'q');
    if (move.from == 63)
      removeCastlingRights(castlingRights, 'k');
    break;
  case Piece::BlackQueen:
    blackQueens = (blackQueens & ~fromBB) | toBB;
    break;
  case Piece::BlackKing:
    blackKing = (blackKing & ~fromBB) | toBB;
    removeCastlingRights(castlingRights, 'k');
    removeCastlingRights(castlingRights, 'q');
    break;
  default:
    return false;
  }

  // Update board array
  board[move.to] = board[move.from];
  board[move.from] = Piece::Empty;

  return true;
}

bool ChessBoard::testMove(Move &move) {
  ChessBoard tempBoard = *this;

  // Make the move on the temporary board
  if (!tempBoard.doMove(move)) {
    return false;
  }

  // Check if the move leaves the king in check
  bool leavesInCheck = tempBoard.isInCheck(!tempBoard.sideToMove);

  return !leavesInCheck;
}

bool ChessBoard::hasInsufficientMaterial() const {
  uint64_t allPieces = getWhitePieces() | getBlackPieces();
  int pieceCount = __builtin_popcountll(allPieces);

  // indicates kings only
  if (pieceCount == 2) {
    return true;
  }

  // king + bishop vs king || king + knight vs king
  if (pieceCount == 3) {
    bool onlyOneBishop =
        __builtin_popcountll(whiteBishops | blackBishops) == 1 &&
        __builtin_popcountll(whiteKnights | blackKnights | whiteRooks |
                             blackRooks | whiteQueens | blackQueens |
                             whitePawns | blackPawns) == 0;
    bool onlyOneKnight =
        __builtin_popcountll(whiteKnights | blackKnights) == 1 &&
        __builtin_popcountll(whiteBishops | blackBishops | whiteRooks |
                             blackRooks | whiteQueens | blackQueens |
                             whitePawns | blackPawns) == 0;
    return onlyOneBishop || onlyOneKnight;
  }

  // King + bishop vs king + bishop (same colored squares)
  if (pieceCount == 4 &&
      __builtin_popcountll(whiteBishops | blackBishops) == 2 &&
      __builtin_popcountll(whiteKnights | blackKnights | whiteRooks |
                           blackRooks | whitePawns | blackPawns | whiteQueens |
                           blackQueens) == 0) {
    // Get squares of both bishops
    int whiteBishopSquare = __builtin_ctzll(whiteBishops);
    int blackBishopSquare = __builtin_ctzll(blackBishops);

    // Check if bishops are on same colored squares
    bool whiteSquareWhiteBishop =
        ((whiteBishopSquare / 8 + whiteBishopSquare % 8) % 2) == 0;
    bool whiteSquareBlackBishop =
        ((blackBishopSquare / 8 + blackBishopSquare % 8) % 2) == 0;

    return whiteSquareWhiteBishop == whiteSquareBlackBishop;
  }

  return false;
}

bool ChessBoard::isCheckmate() const {
  if (!isInCheck(sideToMove)) {
    return false;
  }

  ChessBoard tempBoard = *this;
  tempBoard.generateMoves();

  for (Move move : tempBoard.moves) {
    ChessBoard testBoard = tempBoard;
    if (testBoard.makeMove(move)) {
      return false;
    }
  }

  return true;
}

bool ChessBoard::isStalemate() const {
  if (isInCheck(sideToMove)) {
    return false;
  }

  bool fiftyMoveRule = halfMoveClock >= 100;
  bool insufficientMaterial = hasInsufficientMaterial();

  if (fiftyMoveRule || insufficientMaterial) {
    return true;
  }

  ChessBoard tempBoard = *this;
  tempBoard.generateMoves();

  for (Move move : tempBoard.moves) {
    ChessBoard testBoard = tempBoard;
    if (testBoard.makeMove(move)) {
      return false;
    }
  }

  return true;
}

bool ChessBoard::isMoveLegal(Move &move) {
  Piece movingPiece = board[move.from];

  if (movingPiece == Piece::Empty) {
    return false;
  }

  bool isWhitePiece =
      static_cast<int>(movingPiece) <= static_cast<int>(Piece::WhiteKing);
  if (sideToMove != (isWhitePiece ? 0 : 1)) {
    return false;
  }

  // Check if the destination square is occupied by own piece
  uint64_t ownPieces = isWhitePiece ? getWhitePieces() : getBlackPieces();
  if (ownPieces & (1ULL << move.to)) {
    return false;
  }

  // Verify the move is physically possible for the piece type
  uint64_t fromBB = 1ULL << move.from;
  uint64_t toBB = 1ULL << move.to;
  uint64_t allPieces = getWhitePieces() | getBlackPieces();

  bool isPossible = false;
  switch (movingPiece) {
  case Piece::WhitePawn:
  case Piece::BlackPawn: {
    int forward = (isWhitePiece ? 8 : -8);
    int startRank = (isWhitePiece ? 1 : 6);

    // Single push
    if (move.to == move.from + forward && !(allPieces & toBB)) {
      isPossible = true;
    }
    // Double push
    else if (move.from / 8 == startRank && move.to == move.from + 2 * forward &&
             !(allPieces & toBB) &&
             !(allPieces & (1ULL << (move.from + forward)))) {
      isPossible = true;
    }
    // Captures
    else if ((move.to == move.from + forward + 1 ||
              move.to == move.from + forward - 1) &&
             ((isWhitePiece ? getBlackPieces() : getWhitePieces()) & toBB)) {
      isPossible = true;
    }
    break;
  }
  case Piece::WhiteKnight:
  case Piece::BlackKnight:
    isPossible = knight_attacks[move.from] & toBB;
    break;
  case Piece::WhiteBishop:
  case Piece::BlackBishop:
    isPossible = getBishopAttacks(move.from, allPieces) & toBB;
    break;
  case Piece::WhiteRook:
  case Piece::BlackRook:
    isPossible = getRookAttacks(move.from, allPieces) & toBB;
    break;
  case Piece::WhiteQueen:
  case Piece::BlackQueen:
    isPossible = (getBishopAttacks(move.from, allPieces) |
                  getRookAttacks(move.from, allPieces)) &
                 toBB;
    break;
  case Piece::WhiteKing:
  case Piece::BlackKing:
    isPossible = king_attacks[move.from] & toBB;
    break;
  default:
    return false;
  }

  return isPossible;
}

bool ChessBoard::makeMove(Move &move) {
  if (!isMoveLegal(move)) {
    return false;
  }

  // Store the current state before making the move
  BoardState prevState = {enPassantSquare,
                          sideToMove,
                          castlingRights,
                          halfMoveClock,
                          fullMoveNumber,
                          board[move.to],
                          {whitePawns, whiteKnights, whiteBishops, whiteRooks,
                           whiteQueens, whiteKing, blackPawns, blackKnights,
                           blackBishops, blackRooks, blackQueens, blackKing}};

  // Make sure the move doesn't leave king in check
  if (!testMove(move)) {
    return false;
  }

  // make the move
  if (!doMove(move)) {
    return false;
  }

  if (sideToMove) {
    fullMoveNumber++;
  }

  enPassantSquare = 0xFF; // Reset en passant
  Piece movingPiece = board[move.to];
  if ((movingPiece == Piece::WhitePawn && move.from / 8 == 1 &&
       move.to / 8 == 3) ||
      (movingPiece == Piece::BlackPawn && move.from / 8 == 6 &&
       move.to / 8 == 4)) {
    enPassantSquare = (move.from + move.to) / 2;
  }

  // Update half move clock
  if (prevState.capturedPiece != Piece::Empty ||
      movingPiece == Piece::WhitePawn || movingPiece == Piece::BlackPawn) {
    halfMoveClock = 0;
  } else {
    halfMoveClock++;
  }

  sideToMove = !sideToMove;
  stateHistory.push_back(prevState);

  return true;
}

void ChessBoard::unmakeMove() {
  if (stateHistory.empty()) {
    return;
  }

  BoardState prevState = stateHistory.back();
  stateHistory.pop_back();

  // Restore game state
  enPassantSquare = prevState.enPassantSquare;
  sideToMove = prevState.sideToMove;
  castlingRights = prevState.castlingRights;
  halfMoveClock = prevState.halfMoveClock;
  fullMoveNumber = prevState.fullMoveNumber;

  // Restore bitboards
  whitePawns = prevState.positions[0];
  whiteKnights = prevState.positions[1];
  whiteBishops = prevState.positions[2];
  whiteRooks = prevState.positions[3];
  whiteQueens = prevState.positions[4];
  whiteKing = prevState.positions[5];
  blackPawns = prevState.positions[6];
  blackKnights = prevState.positions[7];
  blackBishops = prevState.positions[8];
  blackRooks = prevState.positions[9];
  blackQueens = prevState.positions[10];
  blackKing = prevState.positions[11];

  // Rebuild board array from bitboards
  for (int square = 0; square < 64; square++) {
    uint64_t bit = 1ULL << square;
    if (whitePawns & bit)
      board[square] = Piece::WhitePawn;
    else if (whiteKnights & bit)
      board[square] = Piece::WhiteKnight;
    else if (whiteBishops & bit)
      board[square] = Piece::WhiteBishop;
    else if (whiteRooks & bit)
      board[square] = Piece::WhiteRook;
    else if (whiteQueens & bit)
      board[square] = Piece::WhiteQueen;
    else if (whiteKing & bit)
      board[square] = Piece::WhiteKing;
    else if (blackPawns & bit)
      board[square] = Piece::BlackPawn;
    else if (blackKnights & bit)
      board[square] = Piece::BlackKnight;
    else if (blackBishops & bit)
      board[square] = Piece::BlackBishop;
    else if (blackRooks & bit)
      board[square] = Piece::BlackRook;
    else if (blackQueens & bit)
      board[square] = Piece::BlackQueen;
    else if (blackKing & bit)
      board[square] = Piece::BlackKing;
    else
      board[square] = Piece::Empty;
  }

  moves.clear();
}

void ChessBoard::initAttacks() {
  initKingAttacks();
  initKnightAttacks();
  initPawnAttacks();
}

void ChessBoard::displayBitboard(uint64_t bitboard) const {
  std::cout << "\n";
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << rank + 1 << "  ";
    for (int file = 0; file < 8; file++) {
      int square = rank * 8 + file;
      if (bitboard & (1ULL << square)) {
        std::cout << "x ";
      } else {
        std::cout << ". ";
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n   a b c d e f g h\n\n";
}

// TODO: implement this later
void ChessBoard::displayAttacks(int square, uint64_t attacks,
                                char piece) const {
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << (rank + 1) << " ";
    for (int file = 0; file < 8; file++) {
      int sq = rank * 8 + file;
      uint64_t bit = 1ULL << sq;

      if (sq == square)
        std::cout << piece << " ";
      else if (attacks & bit)
        std::cout << "x ";
      else
        std::cout << ". ";
    }
    std::cout << '\n';
  }
  std::cout << "  a b c d e f g h\n";
}

void ChessBoard::displayKingAttacks(int square) const {
  std::cout << "King attacks from square " << square << ":\n";
  uint64_t attacks = king_attacks[square];

  for (int rank = 7; rank >= 0; rank--) {
    std::cout << (rank + 1) << " ";
    for (int file = 0; file < 8; file++) {
      int sq = rank * 8 + file;
      uint64_t bit = 1ULL << sq;

      if (sq == square)
        std::cout << "K ";
      else if (attacks & bit)
        std::cout << "x ";
      else
        std::cout << ". ";
    }
    std::cout << '\n';
  }
  std::cout << "  a b c d e f g h" << std::endl;
}

void ChessBoard::displayKnightAttacks(int square) const {
  std::cout << "Knight attacks from square " << square << ":\n";
  uint64_t attacks = knight_attacks[square];

  // Display board with possible knight moves
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << (rank + 1) << " ";
    for (int file = 0; file < 8; file++) {
      int sq = rank * 8 + file;
      uint64_t bit = 1ULL << sq;

      if (sq == square)
        std::cout << "N ";
      else if (attacks & bit)
        std::cout << "x ";
      else
        std::cout << ". ";
    }
    std::cout << '\n';
  }
  std::cout << "  a b c d e f g h" << std::endl;
}

bool ChessBoard::isPathClear(int from, int to, uint64_t allPieces) const {
  int fromRank = from / 8;
  int fromFile = from % 8;
  int toRank = to / 8;
  int toFile = to % 8;

  // Determine the direction of movement
  int rankStep = (toRank == fromRank) ? 0 : (toRank > fromRank) ? 1 : -1;
  int fileStep = (toFile == fromFile) ? 0 : (toFile > fromFile) ? 1 : -1;

  int currentRank = fromRank + rankStep;
  int currentFile = fromFile + fileStep;

  // Check each square between from and to (exclusive)
  while (currentRank != toRank || currentFile != toFile) {
    int currentSquare = currentRank * 8 + currentFile;
    if (allPieces & (1ULL << currentSquare)) {
      return false;
    }
    currentRank += rankStep;
    currentFile += fileStep;
  }
  return true;
}

void ChessBoard::generateRookMoves(uint64_t rooks, uint64_t ownPieces,
                                   uint64_t enemyPieces) {
  uint64_t allPieces = ownPieces | enemyPieces;
  while (rooks) {
    int from = __builtin_ctzll(rooks);
    uint64_t potentialMoves = getRookAttacks(from, allPieces);
    potentialMoves &= ~ownPieces; // Remove squares occupied by own pieces

    uint64_t validMoves = potentialMoves;
    while (validMoves) {
      int to = __builtin_ctzll(validMoves);
      if (isPathClear(from, to, allPieces)) {
        moves.push_back(Move{(uint8_t)from, (uint8_t)to});
      }
      validMoves &= validMoves - 1;
    }
    rooks &= rooks - 1;
  }
}

void ChessBoard::generateBishopMoves(uint64_t bishops, uint64_t ownPieces,
                                     uint64_t enemyPieces) {
  uint64_t allPieces = ownPieces | enemyPieces;
  while (bishops) {
    int from = __builtin_ctzll(bishops);
    uint64_t potentialMoves = getBishopAttacks(from, allPieces);
    potentialMoves &= ~ownPieces; // Remove squares occupied by own pieces

    uint64_t validMoves = potentialMoves;
    while (validMoves) {
      int to = __builtin_ctzll(validMoves);
      if (isPathClear(from, to, allPieces)) {
        moves.push_back(Move{(uint8_t)from, (uint8_t)to});
      }
      validMoves &= validMoves - 1;
    }
    bishops &= bishops - 1;
  }
}

void ChessBoard::generateQueenMoves(uint64_t queens, uint64_t ownPieces,
                                    uint64_t enemyPieces) {
  uint64_t allPieces = ownPieces | enemyPieces;
  while (queens) {
    int from = __builtin_ctzll(queens);
    uint64_t potentialMoves = getQueenAttacks(from, allPieces);
    potentialMoves &= ~ownPieces; // Remove squares occupied by own pieces

    uint64_t validMoves = potentialMoves;
    while (validMoves) {
      int to = __builtin_ctzll(validMoves);
      if (isPathClear(from, to, allPieces)) {
        moves.push_back(Move{(uint8_t)from, (uint8_t)to});
      }
      validMoves &= validMoves - 1;
    }
    queens &= queens - 1;
  }
}

void ChessBoard::initKingAttacks() {
  for (int sq = 0; sq < 64; sq++) {
    uint64_t attacks = 0;
    int rank = sq / 8;
    int file = sq % 8;

    // Up
    if (rank < 7) {
      attacks |= 1ULL << (sq + 8); // up
      if (file > 0)
        attacks |= 1ULL << (sq + 7); // Up
      if (file < 7)
        attacks |= 1ULL << (sq + 9); // Up-left
    }

    // Same rank
    if (file > 0)
      attacks |= 1ULL << (sq - 1); // Left
    if (file < 7)
      attacks |= 1ULL << (sq + 1); // Right

    // Down
    if (rank > 0) {
      attacks |= 1ULL << (sq - 8); // Down
      if (file > 0)
        attacks |= 1ULL << (sq - 9); // Down-left
      if (file < 7)
        attacks |= 1ULL << (sq - 7); // Down-right
    }

    king_attacks[sq] = attacks;
  }
}

void ChessBoard::initKnightAttacks() {
  for (int sq = 0; sq < 64; sq++) {
    uint64_t attacks = 0;
    int rank = sq / 8;
    int file = sq % 8;

    if (rank < 6 && file < 7)
      attacks |= 1ULL << (sq + 17);
    if (rank < 6 && file > 0)
      attacks |= 1ULL << (sq + 15);
    if (rank < 7 && file < 6)
      attacks |= 1ULL << (sq + 10);
    if (rank > 0 && file < 6)
      attacks |= 1ULL << (sq - 6);
    if (rank > 1 && file < 7)
      attacks |= 1ULL << (sq - 15);
    if (rank > 1 && file > 0)
      attacks |= 1ULL << (sq - 17);
    if (rank < 7 && file > 1)
      attacks |= 1ULL << (sq + 6);
    if (rank > 0 && file > 1)
      attacks |= 1ULL << (sq - 10);

    knight_attacks[sq] = attacks;
  }
};

void ChessBoard::displayPawnAttacks(int side, int square) const {
  std::cout << "Side: " << side << "\nPawn attacks from square " << square
            << ":\n";
  uint64_t attacks = pawn_attacks[side][square];

  for (int rank = 7; rank >= 0; rank--) {
    std::cout << (rank + 1) << " ";
    for (int file = 0; file < 8; file++) {
      int sq = rank * 8 + file;
      uint64_t bit = 1ULL << sq;

      if (sq == square)
        if (side == 0)
          std::cout << "P ";
        else
          std::cout << "p ";
      else if (attacks & bit)
        std::cout << "x ";
      else
        std::cout << ". ";
    }
    std::cout << '\n';
  }
  std::cout << "  a b c d e f g h" << std::endl;
}

void ChessBoard::initPawnAttacks() {
  // side 0: white, side 1: black
  for (int sq = 0; sq < 64; sq++) {
    uint64_t white_attacks = 0;
    uint64_t black_attacks = 0;
    int rank = sq / 8;
    int file = sq % 8;

    if (rank < 7) {
      if (file == 0) {
        white_attacks |= 1ULL << (sq + 9);
      } else if (file == 7) {
        white_attacks |= 1ULL << (sq + 7);
      } else {
        white_attacks |= 1ULL << (sq + 7);
        white_attacks |= 1ULL << (sq + 9);
      }
    }

    if (rank > 0) {
      if (file == 0) {
        black_attacks |= 1ULL << (sq - 7);
      } else if (file == 7) {
        black_attacks |= 1ULL << (sq - 9);
      } else {
        black_attacks |= 1ULL << (sq - 7);
        black_attacks |= 1ULL << (sq - 9);
      }
    }

    pawn_attacks[0][sq] = white_attacks;
    pawn_attacks[1][sq] = black_attacks;
  }
}

void ChessBoard::generatePawnMoves(uint8_t side, uint64_t pawns,
                                   uint64_t ownPieces, uint64_t enemyPieces) {
  int forward = (side == 0) ? 8 : -8; // white moves up, black moves down
  uint64_t emptySquares = ~(ownPieces | enemyPieces);

  while (pawns != 0) {
    uint8_t from = __builtin_ctzll(pawns);

    uint64_t to = 1ULL << (from + forward);
    if (to & emptySquares) {
      moves.push_back(Move{from, uint8_t(from + forward)});

      // check for double pawn push
      if ((side == 0 && from >= 8 && from < 16) ||
          (side == 1 && from >= 48 && from < 56)) {
        uint64_t double_push = 1ULL << (from + 2 * forward);
        if (double_push & emptySquares) {
          moves.push_back(Move{from, uint8_t(from + 2 * forward)});
        }
      }
    }

    // capturing
    uint64_t captures = pawn_attacks[side][from] & enemyPieces;
    while (captures != 0) {
      uint8_t to = __builtin_ctzll(captures);
      moves.push_back({from, to});
      captures &= captures - 1;
    }

    // clear bit
    pawns &= pawns - 1;
  }
}

void ChessBoard::generateKingMoves(uint64_t king, uint64_t ownPieces,
                                   uint64_t enemyPieces) {
  while (king != 0) {
    uint8_t from = __builtin_ctzll(king); // get index of least significant bit

    uint64_t destinations = king_attacks[from];
    destinations &= ~ownPieces; // remove own pieces

    while (destinations != 0) {
      uint8_t to = __builtin_ctzll(destinations);
      moves.push_back(Move{from, to});
      destinations &= destinations - 1;
    }

    // clear bit
    king &= king - 1;
  }
}

void ChessBoard::generateKnightMoves(uint64_t knights, uint64_t ownPieces,
                                     uint64_t enemyPieces) {
  while (knights != 0) {
    uint8_t from =
        __builtin_ctzll(knights); // get index of least significant bit

    uint64_t destinations = knight_attacks[from];
    destinations &= ~ownPieces; // remove own pieces

    while (destinations != 0) {
      uint8_t to = __builtin_ctzll(destinations);
      moves.push_back(Move{from, to});
      destinations &= destinations - 1;
    }

    // clear bit
    knights &= knights - 1;
  }
}

std::vector<Move> ChessBoard::generateMoves() {
  moves.clear();
  uint64_t ownPieces = (sideToMove == 0) ? getWhitePieces() : getBlackPieces();
  uint64_t enemyPieces =
      (sideToMove == 0) ? getBlackPieces() : getWhitePieces();

  generateKnightMoves((sideToMove == 0) ? whiteKnights : blackKnights,
                      ownPieces, enemyPieces);
  generatePawnMoves(sideToMove, (sideToMove == 0) ? whitePawns : blackPawns,
                    ownPieces, enemyPieces);
  generateRookMoves((sideToMove == 0) ? whiteRooks : blackRooks, ownPieces,
                    enemyPieces);
  generateBishopMoves((sideToMove == 0) ? whiteBishops : blackBishops,
                      ownPieces, enemyPieces);
  generateQueenMoves((sideToMove == 0) ? whiteQueens : blackQueens, ownPieces,
                     enemyPieces);
  generateKingMoves((sideToMove == 0) ? whiteKing : blackKing, ownPieces,
                    enemyPieces);

  return moves;
}

void ChessBoard::reset() {
  sideToMove = 0;
  castlingRights = "KQkq";
  halfMoveClock = 0;
  fullMoveNumber = 1;

  // White pieces (rank 1-2)
  whitePawns = 0x000000000000FF00ULL;
  whiteKnights = 0x0000000000000042ULL;
  whiteBishops = 0x0000000000000024ULL;
  whiteRooks = 0x0000000000000081ULL;
  whiteQueens = 0x0000000000000008ULL;
  whiteKing = 0x0000000000000010ULL;

  // Black pieces (rank 7-8)
  blackPawns = 0x00FF000000000000ULL;
  blackKnights = 0x4200000000000000ULL;
  blackBishops = 0x2400000000000000ULL;
  blackRooks = 0x8100000000000000ULL;
  blackQueens = 0x0800000000000000ULL;
  blackKing = 0x1000000000000000ULL;

  // Initialize array representation
  for (int i = 0; i < 64; i++) {
    board[i] = Piece::Empty;
  }

  // First rank (white pieces)
  board[0] = Piece::WhiteRook;
  board[1] = Piece::WhiteKnight;
  board[2] = Piece::WhiteBishop;
  board[3] = Piece::WhiteQueen; // Changed order
  board[4] = Piece::WhiteKing;  // Changed order
  board[5] = Piece::WhiteBishop;
  board[6] = Piece::WhiteKnight;
  board[7] = Piece::WhiteRook;

  // Rest unchanged as array indexes already match the standard convention
  for (int i = 8; i < 16; i++)
    board[i] = Piece::WhitePawn; // 2nd rank
  for (int i = 48; i < 56; i++)
    board[i] = Piece::BlackPawn; // 7th rank

  // Eighth rank (black pieces)
  board[56] = Piece::BlackRook;
  board[57] = Piece::BlackKnight;
  board[58] = Piece::BlackBishop;
  board[59] = Piece::BlackQueen; // Changed order
  board[60] = Piece::BlackKing;  // Changed order
  board[61] = Piece::BlackBishop;
  board[62] = Piece::BlackKnight;
  board[63] = Piece::BlackRook;

  std::cout << "Game at state 0" << std::endl;
}

uint64_t ChessBoard::getWhitePieces() const {
  return whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens |
         whiteKing;
}

uint64_t ChessBoard::getBlackPieces() const {
  return blackPawns | blackKnights | blackBishops | blackRooks | blackQueens |
         blackKing;
}

void ChessBoard::display(bool debug) const {
  if (debug) {

    std::cout << "Side to move: " << (sideToMove == 0 ? "White" : "Black")
              << " " << sideToMove << std::endl;
    std::cout << "Castling rights: " << castlingRights << std::endl;
    std::cout << "Half move clock: " << halfMoveClock << std::endl;
    std::cout << "Full move number: " << fullMoveNumber << std::endl;
  }

  // Display bitboard representation
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << 8 * rank << " " << (rank + 1) << " ";
    for (int file = 0; file < 8; file++) {
      int square = rank * 8 + file; // Removed (7 - file)
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
  std::cout << "     a b c d e f g h" << std::endl;

  // Display raw 8x8 values
  /*std::cout << "\n8x8 values:\n";*/
  /*for (int rank = 7; rank >= 0; rank--) {*/
  /*  std::cout << (rank + 1) << " ";*/
  /*  for (int file = 0; file < 8; file++) {*/
  /*    int square = rank * 8 + file; // Removed (7 - file)*/
  /*    std::cout << int(board[square]) << "\t";*/
  /*  }*/
  /*  std::cout << '\n';*/
  /*}*/
  /*std::cout << "  a b c d e f g h" << std::endl;*/
}
