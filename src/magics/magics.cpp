#include "magics.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>

uint64_t BISHOP_MAGICS[64] = {
    0x40040844404084ULL,   0x2004208a004208ULL,   0x10190041080202ULL,
    0x108060845042010ULL,  0x581104180800210ULL,  0x2112080446200010ULL,
    0x1080820820060210ULL, 0x3c0808410220200ULL,  0x4050404440404ULL,
    0x21001420088ULL,      0x24d0080801082102ULL, 0x1020a0a020400ULL,
    0x40308200402ULL,      0x4011002100800ULL,    0x401484104104005ULL,
    0x801010402020200ULL,  0x400210c3880100ULL,   0x404022024108200ULL,
    0x810018200204102ULL,  0x4002801a02003ULL,    0x85040820080400ULL,
    0x810102c808880400ULL, 0xe900410884800ULL,    0x8002020480840102ULL,
    0x220200865090201ULL,  0x2010100a02021202ULL, 0x152048408022401ULL,
    0x20080002081110ULL,   0x4001001021004000ULL, 0x800040400a011002ULL,
    0xe4004081011002ULL,   0x1c004001012080ULL,   0x8004200962a00220ULL,
    0x8422100208500202ULL, 0x2000402200300c08ULL, 0x8646020080080080ULL,
    0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL,
    0x42008c03402092ULL,   0x209188240001000ULL,  0x400408a884001800ULL,
    0x110400a6080400ULL,   0x1840060a44020800ULL, 0x90080104000041ULL,
    0x201011000808101ULL,  0x1a2208080504f080ULL, 0x8012020600211212ULL,
    0x500861011240000ULL,  0x180806108200800ULL,  0x4000020e01040044ULL,
    0x300000261044000aULL, 0x802241102020002ULL,  0x20906061210001ULL,
    0x5a84841004010310ULL, 0x4010801011c04ULL,    0xa010109502200ULL,
    0x4a02012000ULL,       0x500201010098b028ULL, 0x8040002811040900ULL,
    0x28000010020204ULL,   0x6000020202d0240ULL,  0x8918844842082200ULL,
    0x4010011029020020ULL};

uint64_t ROOK_MAGICS[64] = {
    0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,
    0x100081001000420ULL,  0x200020010080420ULL,  0x3001c0002010008ULL,
    0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL,
    0x2024401000200040ULL, 0x100802000801000ULL,  0x120800800801000ULL,
    0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL,
    0x801000060821100ULL,  0x80044006422000ULL,   0x100808020004000ULL,
    0x12108a0010204200ULL, 0x140848010000802ULL,  0x481828014002800ULL,
    0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,
    0x100400080208000ULL,  0x2040002120081000ULL, 0x21200680100081ULL,
    0x20100080080080ULL,   0x2000a00200410ULL,    0x20080800400ULL,
    0x80088400100102ULL,   0x80004600042881ULL,   0x4040008040800020ULL,
    0x440003000200801ULL,  0x4200011004500ULL,    0x188020010100100ULL,
    0x14800401802800ULL,   0x2080040080800200ULL, 0x124080204001001ULL,
    0x200046502000484ULL,  0x480400080088020ULL,  0x1000422010034000ULL,
    0x30200100110040ULL,   0x100021010009ULL,     0x2002080100110004ULL,
    0x202008004008002ULL,  0x20020004010100ULL,   0x2048440040820001ULL,
    0x101002200408200ULL,  0x40802000401080ULL,   0x4008142004410100ULL,
    0x2060820c0120200ULL,  0x1001004080100ULL,    0x20c020080040080ULL,
    0x2935610830022400ULL, 0x44440041009200ULL,   0x280001040802101ULL,
    0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL,    0x12001008414402ULL,   0x2006104900a0804ULL,
    0x1004081002402ULL};

uint64_t ROOK_ATTACKS[64][4096];
uint64_t BISHOP_ATTACKS[64][4096];

struct Magic {
  uint64_t mask;
  uint64_t magic;
  int shift;
};

static uint64_t generateSlidingAttacks(int square, uint64_t blockers,
                                       bool isBishop) {
  uint64_t attacks = 0;
  int rank = square / 8;
  int file = square % 8;

  // directions of each sliding piece
  int bishopDeltas[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  int rookDeltas[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  int(*deltas)[2] = isBishop ? bishopDeltas : rookDeltas;

  for (int dir = 0; dir < 4; dir++) {
    int r = rank;
    int f = file;

    while (true) {
      r += deltas[dir][0];
      f += deltas[dir][1];

      if (r < 0 || r > 7 || f < 0 || f > 7)
        break;

      uint64_t squareMask = 1ULL << (r * 8 + f);
      attacks |= squareMask;

      if (blockers & squareMask) {
        break;
      }
    }
  }

  return attacks;
}

uint64_t getQueenMask(int square) {
  return getBishopMask(square) | getRookMask(square);
}

uint64_t getQueenAttacks(int square, uint64_t blockers) {
  return getBishopAttacks(square, blockers) | getRookAttacks(square, blockers);
}

uint64_t getBishopMask(int square) {
  uint64_t mask = 0;
  int rank = square / 8;
  int file = square % 8;

  int diag = 0;

  int r = rank;
  int f = file;

  // northeast
  while (r < 6 && f < 7) {
    r++;
    f++;
    mask |= 1ULL << (r * 8 + f);
  }

  r = rank;
  f = file;

  // northwest
  while (r < 6 && f > 0) {
    r++;
    f--;
    mask |= 1ULL << (r * 8 + f);
  }

  r = rank;
  f = file;

  // southeast
  while (r > 1 && f < 7) {
    r--;
    f++;
    mask |= 1ULL << (r * 8 + f);
  }

  r = rank;
  f = file;

  // southwest
  while (r > 1 && f > 0) {
    r--;
    f--;
    mask |= 1ULL << (r * 8 + f);
  }

  return mask;
}

uint64_t getBishopAttacks(int square, uint64_t blockers) {
  uint64_t mask = getBishopMask(square);
  uint64_t relevantBlockers = blockers & mask;
  uint64_t magic = BISHOP_MAGICS[square];

  int shift = 64 - __builtin_popcountll(mask);
  int index = (relevantBlockers * magic) >> shift;

  return BISHOP_ATTACKS[square][index];
}

uint64_t getRookMask(int square) {
  int rank = square / 8;
  int file = square % 8;
  uint64_t mask = 0;

  uint64_t rankMask = 0xFFULL << (rank * 8);
  // Remove the edge squares on the rank if they exist
  if (file > 0)
    rankMask &= ~(1ULL << (rank * 8));
  if (file < 7)
    rankMask &= ~(1ULL << (rank * 8 + 7));

  uint64_t fileMask = 0x0101010101010101ULL << file;
  // Remove the edge squares on the file if they exist
  if (rank > 0)
    fileMask &= ~(1ULL << file);
  if (rank < 7)
    fileMask &= ~(1ULL << (56 + file));

  mask = rankMask | fileMask;
  mask ^= 1ULL << square; // Remove own square

  return mask;
}

uint64_t getRookAttacks(int square, uint64_t blockers) {
  uint64_t mask = getRookMask(square);
  uint64_t relevantBlockers = blockers & mask;
  uint64_t magic = ROOK_MAGICS[square];

  int shift = 64 - __builtin_popcountll(mask);
  int index = (blockers * magic) >> shift;

  return ROOK_ATTACKS[square][index];
};

void initMagicTable(bool isBishop) {
  uint64_t *magics = isBishop ? BISHOP_MAGICS : ROOK_MAGICS;
  uint64_t(*attacks)[4096] = isBishop ? BISHOP_ATTACKS : ROOK_ATTACKS;

  std::cout << "Initializing " << (isBishop ? "bishop" : "rook") << " magics\n";

  for (int square = 0; square < 64; square++) {
    uint64_t mask = isBishop ? getBishopMask(square) : getRookMask(square);
    int bits = __builtin_popcountll(mask);
    int n = 1 << bits;

    // Generate all possible blocker combinations for this square
    for (int i = 0; i < n; i++) {
      uint64_t blockers = 0;
      uint64_t m = mask;
      int j = 0;

      // Convert index to blocker configuration
      while (m) {
        int bit = __builtin_ctzll(m);
        if (i & (1 << j)) {
          blockers |= 1ULL << bit;
        }
        m &= m - 1;
        j++;
      }

      // Calculate magic index
      uint64_t magic = magics[square];
      int shift = 64 - bits;
      int index = (blockers * magic) >> shift;

      // Store attack pattern
      uint64_t attack = generateSlidingAttacks(square, blockers, isBishop);
      attacks[square][index] = attack;
    }
  }
  std::cout << "Finished initializing " << (isBishop ? "bishop" : "rook")
            << " magics\n";
}

void initMagics() {
  std::cout << "Initializing magics" << std::endl;
  // init both rook and bishop magics (attack tables)
  initMagicTable(true);  // bishops
  initMagicTable(false); // rooks
}
