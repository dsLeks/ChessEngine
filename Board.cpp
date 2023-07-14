#include <stdio.h>
#include "defs.h"

// Add file to Makefile

void ResetBoard(S_BOARD *pos)
{
    for (int i = 0; i < BRD_SQ_NUM; i++)
    {
        pos->board[i] = 120; // Set them to offboard
    }

    for (int i = 0; i < 64; i++)
    {
        pos->board[Sq64ToSq120[i]] = EMPTY;
    }

    for (int i = 0; i < 3; i++)
    {
        pos->bigPce[i] = 0;
        pos->majPce[i] = 0;
        pos->minPce[i] = 0;
        pos->pawns[i] = 0ULL;
    }

    for (int i = 0; i < 13; i++)
    {
        pos->pceNum[i] = 0;
    }

    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMove = 0;

    pos->ply = 0;
    pos->hisPly = 0;

    pos->castlePerm = 0;

    pos->posKey = 0ULL;
}