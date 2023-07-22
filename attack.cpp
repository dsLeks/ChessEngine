#include <iostream>
#include "defs.h"

// Direction Arrays:
const int KnDir[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int RkDir[4] = {-1, -10, 1, 10};
const int BiDir[4] = {-9, -11, 11, 9};
const int KiDir[8] = {-1, -10, 1, 10, -9, -11, 11, 9};

int SqAttacked(const int sq, const int side, const S_BOARD *pos) // side = side that is attacking!
{

    ASSERT(SqOnBoard(sq));
    ASSERT(SideValid(side));
    ASSERT(CheckBoard(pos));
    // pawns
    if (side == WHITE)
    {
        if (pos->board[sq - 11] == wP || pos->board[sq - 9] == wP)
        {
            return TRUE;
        }
    }
    else
    {
        if (pos->board[sq + 11] == bP || pos->board[sq + 9] == bP)
        {
            return TRUE;
        }
    }

    // knights
    for (int index = 0; index < 8; ++index)
    {
        int pce = pos->board[sq + KnDir[index]];

        if (pce != 120 && IsKn(pce) && PieceCol[pce] == side)
        {
            return TRUE;
        }
    }

    // rooks, queens
    for (int index = 0; index < 4; ++index)
    {
        int dir = RkDir[index];
        int t_sq = sq + dir;

        int pce = pos->board[t_sq];

        while (pce != 120)
        {
            if (pce != EMPTY)
            {
                if (IsRQ(pce) && PieceCol[pce] == side)
                {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;

            pce = pos->board[t_sq];
        }
    }

    // bishops, queens
    for (int index = 0; index < 4; ++index)
    {
        int dir = BiDir[index];
        int t_sq = sq + dir;

        int pce = pos->board[t_sq];

        while (pce != 120)
        {
            if (pce != EMPTY)
            {
                if (IsBQ(pce) && PieceCol[pce] == side)
                {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;

            pce = pos->board[t_sq];
        }
    }

    // kings
    for (int index = 0; index < 8; ++index)
    {
        int pce = pos->board[sq + KiDir[index]];

        if (pce != 120 && IsKi(pce) && PieceCol[pce] == side)
        {
            return TRUE;
        }
    }

    return FALSE;
}
