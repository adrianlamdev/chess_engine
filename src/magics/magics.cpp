#include "magics.h"

uint64_t getRookMask(int square) {
  int rank = square / 8;
  int file = square % 8;
  uint64_t mask = 0;

  uint64_t rankMask = 0xFF << (rank * 8);
  if (rank > 0 && rank < 7) {
    rankMask &= ~0x81 << (rank * 8);
  };

  uint64_t fileMask = 0x0101010101010101ULL << file;
  if (file > 0 && file < 7) {
    fileMask &= ~0xff000000000000ffULL;
  };

  mask = rankMask | fileMask;
  mask ^= 1ULL << square; // remove own square

  return mask;
};

uint64_t getRookAttacks(int square, uint64_t block);
