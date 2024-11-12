#ifndef MAGICS_H
#define MAGICS_H

#include <cstdint>

uint64_t getBishopMask(int square);
uint64_t getBishopAttacks(int square, uint64_t block);

uint64_t getRookMask(int square);
uint64_t getRookAttacks(int square, uint64_t block);

uint64_t getQueenMask(int square);
uint64_t getQueenAttacks(int square, uint64_t block);

#endif
