#ifndef MAGICS_H
#define MAGICS_H

#include <cstdint>

uint64_t getRookMask(int square);

uint64_t getRookAttacks(int square, uint64_t block);

#endif
