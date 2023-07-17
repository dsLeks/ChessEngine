#include <iostream>
#include <stdio.h>
#include "defs.h"

const int BitTable[64] = {
    63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
    58, 20, 37, 17, 36, 8};

int PopBit(U64 *bb) // PopBit() pops the least significant bit that is set to 1
{
    U64 b = *bb ^ (*bb - 1);                                      // Set the least significant 1-bit in bb to 0 and set all other bits to 1
    unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32)); // Fold the 64-bit value into 32 bits
    *bb &= (*bb - 1);                                             // Reset the least significant bit in bb to 0
    return BitTable[(fold * 0x783a9b23) >> 26];                   // Return the index of the least significant 1-bit
}

int CountBits(U64 b) // CountBits() counts the number of bits set to 1 in a 64-bit integer
{
    int c;
    for (c = 0; b != 0; c++)
    {
        (b = b & (b - 1));
    }

    return c;
}

void PrintBitBoard(U64 board)
{
    U64 shiftMe = 1ULL;

    int rank = 0;
    int file = 0;
    int sq = 0;
    int sq64 = 0;

    for (rank = RANK_8; rank >= RANK_1; rank--)
    {
        for (file = FILE_A; file <= FILE_H; file++)
        {
            sq = FR2SQ(file, rank); // Convert file and rank to square => 120 based Index board
            sq64 = Sq120ToSq64[sq]; // Convert square to 64 square => Get the 64 based index board
            if (shiftMe << sq64 & board)
                std::cout << "X";
            else
                std::cout << "-";
        }
        std::cout << std::endl;
    }
}
