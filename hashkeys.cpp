#include "defs.h"

/*This file is used to determine the posKey. To get a unique key for each position. This can be used to determine if a 3 fold repitiion has occured.*/

// a ^ b ^ c (if you want to take out c then xor it again) => a ^ b = a ^ b ^ c ^ c

U64 GeneratePosKey(const S_BOARD *pos)
{

    U64 finalKey = 0ULL;

    for (int sq; sq < 120; sq++)
    {
        int piece = pos->board[sq];
        if (sq != NO_SQ && piece != EMPTY && piece != 120)
        {
            ASSERT(piece >= wP && piece <= bK);
            finalKey ^= PieceKeys[piece][sq];
        }
    }

    return finalKey;

    if (pos->enPas != NO_SQ)
    {
        ASSERT(pose->enPas >= 0 && pos->enPas < 120);
        finalKey ^= PieceKeys[EMPTY][pos->enPas];
    }

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    finalKey ^= CastleKeys[pos->castlePerm];

    return finalKey;
}