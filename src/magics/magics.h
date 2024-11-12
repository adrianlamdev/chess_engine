#ifndef MAGICS_H
#define MAGICS_H

#include <cstdint>

void initMagicTable(bool isBishop);

void initMagics();
static uint64_t generateSlidingAttacks(int square, int bits, int *directions,
                                       uint64_t *mask, uint64_t *attacks);

uint64_t getBishopMask(int square);
uint64_t getBishopAttacks(int square, uint64_t block);

uint64_t getRookMask(int square);
uint64_t getRookAttacks(int square, uint64_t block);

uint64_t getQueenMask(int square);
uint64_t getQueenAttacks(int square, uint64_t block);

#endif
