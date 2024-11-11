#include "chess_board.h"
#include <cstdint>
#include <iostream>

ChessBoard::ChessBoard() {
  initAttacks();
  reset();
}

void ChessBoard::initAttacks() {
  initKnightAttacks();
  initPawnAttacks();
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

void ChessBoard::generateMoves() {
  moves.clear();
  uint64_t ownPieces = (sideToMove == 0) ? getWhitePieces() : getBlackPieces();
  uint64_t enemyPieces =
      (sideToMove == 0) ? getBlackPieces() : getWhitePieces();

  generateKnightMoves((sideToMove == 0) ? whiteKnights : blackKnights,
                      ownPieces, enemyPieces);
  generatePawnMoves(sideToMove, (sideToMove == 0) ? whitePawns : blackPawns,
                    ownPieces, enemyPieces);
}

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

void ChessBoard::display() const {
  std::cout << "Side to move: " << (sideToMove == 0 ? "White" : "Black")
            << std::endl;
  std::cout << "Castling rights: " << castlingRights << std::endl;
  std::cout << "Half move clock: " << halfMoveClock << std::endl;
  std::cout << "Full move number: " << fullMoveNumber << std::endl;

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
