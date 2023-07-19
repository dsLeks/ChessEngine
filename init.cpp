#include "defs.h"
#include "stdlib.h"

#define RAND_64 ((U64)rand() | ((U64)rand() << 15) | ((U64)rand() << 30) | ((U64)rand() << 45) | (((U64)rand() & 0xf) << 60))

int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];
U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey; // Side to move key will hash a random number if it is white to move. If it is black to move it will hash a different random number.
U64 CastleKeys[16];

int FilesBrd[BRD_SQ_NUM];
int RanksBrd[BRD_SQ_NUM];

void InitFilesRankBrd()
{
    for (int index = 0; index < BRD_SQ_NUM; ++index)
    {
        FilesBrd[index] = 120;
        RanksBrd[index] = 120;
    }

    for (int rank = RANK_1; rank <= RANK_8; rank++)
    {
        for (int file = FILE_A; file <= FILE_H; file++)
        {
            int sq = FR2SQ(file, rank);
            FilesBrd[sq] = file;
            RanksBrd[sq] = rank;
        }
    }
}

void InitHashKeys()
{
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 120; j++)
        {
            PieceKeys[i][j] = RAND_64;
        }
    }

    SideKey = RAND_64;

    for (int i = 0; i < 16; i++)
    {
        CastleKeys[i] = RAND_64;
    }
}

void InitBitMasks()
{
    int index = 0;

    for (index = 0; index < 64; index++)
    {
        SetMask[index] = 0ULL;
        ClearMask[index] = 0ULL;
    }

    for (index = 0; index < 64; index++)
    {
        SetMask[index] |= (1ULL << index);
        ClearMask[index] = ~SetMask[index];
    }
}

void InitSq120To64()
{
    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq64 = 0;

    // Initialize all squares in the Sq64ToSq120 array to 120 (off board)
    for (index = 0; index < 64; index++)
    {
        Sq64ToSq120[index] = 120;
    }

    // Initialize all squares in the Sq120ToSq64 array to 65 (off board)
    for (index = 0; index < BRD_SQ_NUM; index++)
    {
        Sq120ToSq64[index] = 65;
    }

    for (rank = RANK_1; rank <= RANK_8; rank++)
    {
        for (file = FILE_A; file <= FILE_H; file++)
        {
            int sq = FR2SQ(file, rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

void AllInit()
{
    InitSq120To64();
    InitBitMasks();
    InitHashKeys();
    InitFilesRankBrd();
}
