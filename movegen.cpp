#include "defs.h"
#include "stdio.h"

#define MOVE(f, t, ca, pro, fl) ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (fl))

void AddQuietMove(const S_BOARD *pos, int move, S_MOVELIST *moveList)
{
    moveList->moves[moveList->count].move = move;
    moveList->moves[moveList->count].score = 0;

    moveList->count++;
}

void AddCaptureMove(const S_BOARD *pos, int move, S_MOVELIST *moveList)
{
    moveList->moves[moveList->count].move = move;
    moveList->moves[moveList->count].score = 0;

    moveList->count++;
}

void AddWhitePawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *moveList)
{

    ASSERT(PieceValidEmpty(cap));
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(CheckBoard(pos));

    if (RanksBrd[from] == RANK_7)
    {
        AddCaptureMove(pos, MOVE(from, to, cap, wQ, 0), moveList);
        AddCaptureMove(pos, MOVE(from, to, cap, wR, 0), moveList);
        AddCaptureMove(pos, MOVE(from, to, cap, wB, 0), moveList);
        AddCaptureMove(pos, MOVE(from, to, cap, wN, 0), moveList);
    }
    else
    {
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), moveList);
    }
}

void AddBlackPawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *moveList)
{

    ASSERT(PieceValidEmpty(cap));
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(CheckBoard(pos));

    if (RanksBrd[from] == RANK_2)
    {
        AddCaptureMove(pos, MOVE(from, to, cap, bQ, 0), moveList);
        AddCaptureMove(pos, MOVE(from, to, cap, bR, 0), moveList);
        AddCaptureMove(pos, MOVE(from, to, cap, bB, 0), moveList);
        AddCaptureMove(pos, MOVE(from, to, cap, bN, 0), moveList);
    }
    else
    {
        AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), moveList);
    }
}

void AddWhitePawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *moveList)
{

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(CheckBoard(pos));

    if (RanksBrd[from] == RANK_7)
    {
        AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), moveList);
        AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), moveList);
        AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), moveList);
        AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), moveList);
    }
    else
    {
        AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), moveList);
    }
}

void AddBlackPawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *moveList)
{

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(CheckBoard(pos));

    if (RanksBrd[from] == RANK_2)
    {
        AddQuietMove(pos, MOVE(from, to, EMPTY, bQ, 0), moveList);
        AddQuietMove(pos, MOVE(from, to, EMPTY, bR, 0), moveList);
        AddQuietMove(pos, MOVE(from, to, EMPTY, bB, 0), moveList);
        AddQuietMove(pos, MOVE(from, to, EMPTY, bN, 0), moveList);
    }
    else
    {
        AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), moveList);
    }
}

void AddEnPassantMove(const S_BOARD *pos, int move, S_MOVELIST *moveList)
{
    moveList->moves[moveList->count].move = move;
    moveList->moves[moveList->count].score = 0;

    moveList->count++;
}

void GenerateAllPawnMoves(const S_BOARD *pos, S_MOVELIST *moveList)
{
    ASSERT(CheckBoard(pos));
    moveList->count = 0;

    if (pos->side == WHITE)
    {
        for (int pceNum = 0; pceNum < pos->pceNum[wP]; pceNum++)
        {
            int sq = pos->pList[wP][pceNum];
            ASSERT(SqOnBoard(sq));

            if (pos->board[sq + 10] == EMPTY)
            {
                AddWhitePawnMove(pos, sq, sq + 10, moveList);
                if (RanksBrd[sq] == RANK_2 && pos->board[sq + 20] == EMPTY)
                {
                    AddQuietMove(pos, MOVE(sq, sq + 20, EMPTY, EMPTY, MFLAGPS), moveList);
                }
            }

            // Capture to left (+9) or right (+11)
            if (((sq + 9) != 120) && PieceCol[pos->board[sq + 9]] == BLACK)
            {
                AddWhitePawnCapMove(pos, sq, sq + 9, pos->board[sq + 9], moveList);
            }

            if (((sq + 11) != 120) && PieceCol[pos->board[sq + 11]] == BLACK)
            {
                AddWhitePawnCapMove(pos, sq, sq + 11, pos->board[sq + 11], moveList);
            }

            if (sq + 9 == pos->enPas)
            {
                AddCaptureMove(pos, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), moveList);
            }

            if (sq + 11 == pos->enPas)
            {
                AddCaptureMove(pos, MOVE(sq, sq + 11, EMPTY, EMPTY, MFLAGEP), moveList);
            }
        }
    }
    else
    {
        for (int pceNum = 0; pceNum < pos->pceNum[bP]; pceNum++)
        {
            int sq = pos->pList[bP][pceNum];
            ASSERT(SqOnBoard(sq));

            if (pos->board[sq - 10] == EMPTY)
            {
                AddBlackPawnMove(pos, sq, sq - 10, moveList);
                if (RanksBrd[sq] == RANK_7 && pos->board[sq - 20] == EMPTY)
                {
                    AddQuietMove(pos, MOVE(sq, sq - 20, EMPTY, EMPTY, MFLAGPS), moveList);
                }
            }

            // Capture to left (-9) or right (-11)
            if (((sq - 9) != 120) && PieceCol[pos->board[sq - 9]] == WHITE)
            {
                AddBlackPawnCapMove(pos, sq, sq - 9, pos->board[sq - 9], moveList);
            }

            if (((sq - 11) != 120) && PieceCol[pos->board[sq - 11]] == WHITE)
            {
                AddBlackPawnCapMove(pos, sq, sq - 11, pos->board[sq - 11], moveList);
            }

            if (sq - 9 == pos->enPas)
            {
                AddCaptureMove(pos, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAGEP), moveList);
            }

            if (sq - 11 == pos->enPas)
            {
                AddCaptureMove(pos, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAGEP), moveList);
            }
        }
    }
}

void GenerateAllSlidingPiecesMoves(const S_BOARD *pos, S_MOVELIST *moveList)
{
    /* For Slide Pieces */
    /* wB, wR, wQ  -- bB, bR, bQ */
    if (pos->side == WHITE)
    {
        // For white Bishop
        for (int pceNum = 0; pceNum < pos->pceNum[wB]; pceNum++)
        {
            int sq = pos->pList[wB][pceNum];
            ASSERT(SqOnBoard(sq));

            std::cout << "Move For white Bishop on: " << PrSq(sq) << std::endl;

            for (int dir = 0; dir < 4; dir++)
            {
                int t_sq = sq + bishopMoves[dir];

                while (t_sq != 120)
                {
                    if (pos->board[t_sq] != EMPTY)
                    {
                        if (PieceCol[pos->board[t_sq]] == BLACK)
                        {
                            std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                        }

                        break;
                    }
                    else
                    {
                        std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
                    }

                    t_sq += bishopMoves[dir];
                }
            }

            std::cout << std::endl
                      << std::endl;
        }

        // For white Rook
        for (int pceNum = 0; pceNum < pos->pceNum[wR]; pceNum++)
        {
            int sq = pos->pList[wR][pceNum];
            ASSERT(SqOnBoard(sq));

            std::cout << "Move For white Rook on: " << PrSq(sq) << std::endl;

            for (int dir = 0; dir < 4; dir++)
            {
                int t_sq = sq + rookMoves[dir];

                while (t_sq != 120)
                {
                    if (pos->board[t_sq] != EMPTY)
                    {
                        if (PieceCol[pos->board[t_sq]] == BLACK)
                        {
                            std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                        }
                        break;
                    }
                    else
                    {
                        std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
                    }

                    t_sq += rookMoves[dir];
                }
            }

            std::cout << std::endl
                      << std::endl;
        }

        // For white Queen
        for (int pceNum = 0; pceNum < pos->pceNum[wQ]; pceNum++)
        {
            int sq = pos->pList[wQ][pceNum];
            ASSERT(SqOnBoard(sq));

            std::cout << "Move For white Queen on: " << PrSq(sq) << std::endl;

            for (int dir = 0; dir < 8; dir++)
            {
                int t_sq = sq + queenMoves[dir];

                while (t_sq != 120)
                {
                    if (pos->board[t_sq] != EMPTY)
                    {
                        if (PieceCol[pos->board[t_sq]] == BLACK)
                        {
                            std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                        }

                        break;
                    }
                    else
                    {
                        std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
                    }

                    t_sq += queenMoves[dir];
                }
            }

            std::cout << std::endl
                      << std::endl;
        }
    }
    else
    {
        // For Black Bishop
        for (int pceNum = 0; pceNum < pos->pceNum[bB]; pceNum++)
        {
            int sq = pos->pList[bB][pceNum];
            ASSERT(SqOnBoard(sq));

            std::cout << "Move For black bishop on: " << PrSq(sq) << std::endl;

            for (int dir = 0; dir < 4; dir++)
            {
                int t_sq = sq + bishopMoves[dir];

                while (t_sq != 120)
                {
                    if (pos->board[t_sq] != EMPTY)
                    {
                        if (PieceCol[pos->board[t_sq]] == WHITE)
                        {
                            std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                        }

                        break;
                    }
                    else
                    {
                        std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
                    }

                    t_sq += bishopMoves[dir];
                }
            }

            std::cout << std::endl
                      << std::endl;
        }

        // For black Rook
        for (int pceNum = 0; pceNum < pos->pceNum[bR]; pceNum++)
        {
            int sq = pos->pList[bR][pceNum];
            ASSERT(SqOnBoard(sq));

            std::cout << "Move For black rook on: " << PrSq(sq) << std::endl;

            for (int dir = 0; dir < 4; dir++)
            {
                int t_sq = sq + rookMoves[dir];

                while (t_sq != 120)
                {
                    if (pos->board[t_sq] != EMPTY)
                    {
                        if (PieceCol[pos->board[t_sq]] == WHITE)
                        {
                            std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                        }

                        break;
                    }
                    else
                    {
                        std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
                    }

                    t_sq += rookMoves[dir];
                }
            }

            std::cout << std::endl
                      << std::endl;
        }

        // For black Queen
        for (int pceNum = 0; pceNum < pos->pceNum[bQ]; pceNum++)
        {
            int sq = pos->pList[bQ][pceNum];
            ASSERT(SqOnBoard(sq));

            std::cout << "Move For black queen on: " << PrSq(sq) << std::endl;

            for (int dir = 0; dir < 8; dir++)
            {
                int t_sq = sq + queenMoves[dir];

                while (t_sq != 120)
                {
                    if (pos->board[t_sq] != EMPTY)
                    {
                        if (PieceCol[pos->board[t_sq]] == WHITE)
                        {
                            std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                        }

                        break;
                    }
                    else
                    {
                        std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
                    }

                    t_sq += queenMoves[dir];
                }
            }

            std::cout << std::endl
                      << std::endl;
        }
    }
}

void GenerateAllNonSlidingPiecesMoves(const S_BOARD *pos, S_MOVELIST *moveList)
{
    /* For Slide Pieces */
    /* wN, wK -- bN, bK */
    if (pos->side == WHITE)
    {
        // For white Knight
        for (int pceNum = 0; pceNum < pos->pceNum[wN]; pceNum++)
        {
            int sq = pos->pList[wN][pceNum];
            ASSERT(SqOnBoard(sq));

            for (int dir = 0; dir < 8; dir++)
            {
                int t_sq = sq + knightMoves[dir];

                if (t_sq == 120)
                    continue;

                if (pos->board[t_sq] != EMPTY)
                {
                    if (PieceCol[pos->board[t_sq]] == BLACK)
                    {
                        std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                    }
                    continue;
                }
                std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
            }
        }

        // For white King
        for (int pceNum = 0; pceNum < pos->pceNum[wK]; pceNum++)
        {
            int sq = pos->pList[wK][pceNum];
            ASSERT(SqOnBoard(sq));

            for (int dir = 0; dir < 8; dir++)
            {
                int t_sq = sq + kingMoves[dir];

                if (t_sq == 120)
                    continue;

                if (pos->board[t_sq] != EMPTY)
                {
                    if (PieceCol[pos->board[t_sq]] == BLACK)
                    {
                        std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                    }
                    continue;
                }
                std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
            }
        }
    }
    else
    {
        // For Black Knight
        for (int pceNum = 0; pceNum < pos->pceNum[bN]; pceNum++)
        {
            int sq = pos->pList[bN][pceNum];
            ASSERT(SqOnBoard(sq));

            for (int dir = 0; dir < 8; dir++)
            {
                int t_sq = sq + knightMoves[dir];

                if (t_sq == 120)
                    continue;

                if (pos->board[t_sq] != EMPTY)
                {
                    if (PieceCol[pos->board[t_sq]] == WHITE)
                    {
                        std::cout << "\t\t Capture on: " << PrSq(t_sq) << std::endl;
                    }
                    continue;
                }
                std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
            }
        }

        // For Black King
        for (int pceNum = 0; pceNum < pos->pceNum[bK]; pceNum++)
        {
            int sq = pos->pList[bK][pceNum];
            ASSERT(SqOnBoard(sq));

            for (int dir = 0; dir < 8; dir++)
            {
                int t_sq = sq + kingMoves[dir];

                if (t_sq == 120)
                    continue;

                if (pos->board[t_sq] != EMPTY)
                {
                    if (PieceCol[pos->board[t_sq]] == WHITE)
                    {
                        std::cout << "\t\t Caputre on: " << PrSq(t_sq) << std::endl;
                    }
                    continue;
                }
                std::cout << "\t\t Normal Move: " << PrSq(t_sq) << std::endl;
            }
        }
    }
}

void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *moveList)
{
    // GenerateAllPawnMoves(pos, moveList);
    GenerateAllSlidingPiecesMoves(pos, moveList);
    // GenerateAllNonSlidingPiecesMoves(pos, moveList);
}