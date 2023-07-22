#include <stdio.h>
#include "defs.h"

bool CheckBoard(const S_BOARD *pos)
{

    int t_pceNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int t_bigPce[2] = {0, 0};
    int t_majPce[2] = {0, 0};
    int t_minPce[2] = {0, 0};
    int t_material[2] = {0, 0};

    U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

    t_pawns[WHITE] = pos->pawns[WHITE];
    t_pawns[BLACK] = pos->pawns[BLACK];
    t_pawns[BOTH] = pos->pawns[BOTH];

    for (int t_piece = wP; t_piece <= bK; t_piece++)
    {
        for (int t_piece_num = 0; t_piece_num < pos->pceNum[t_piece]; t_piece_num++)
        {
            int sq120 = pos->pList[t_piece][t_piece_num];
            ASSERT(pos->board[sq120] == t_piece);
        }
    }

    // Go through the board, check piece count and other counters;
    for (int sq64 = 0; sq64 < 64; sq64++)
    {
        int sq120 = Sq64ToSq120[sq64];
        int t_piece = pos->board[sq120];
        int color = PieceCol[t_piece];
        t_pceNum[t_piece] += 1;
        if (PieceBig[t_piece] == TRUE)
            t_bigPce[color]++;
        if (PieceMin[t_piece] == TRUE)
            t_minPce[color]++;
        if (PieceMaj[t_piece] == TRUE)
            t_majPce[color]++;

        t_material[color] += PieceVal[t_piece];
    }

    for (int t_piece = wP; t_piece <= bK; t_piece++)
    {
        ASSERT(t_pceNum[t_piece] == pos->pceNum[t_piece]);
    }

    // Check the Bitboard count
    int pCount = CountBits(t_pawns[WHITE]);
    ASSERT(pCount == pos->pceNum[wP]);
    pCount = CountBits(t_pawns[BLACK]);
    ASSERT(pCount == pos->pceNum[bP]);
    pCount = CountBits(t_pawns[BOTH]);
    ASSERT(pCount == (pos->pceNum[bP] + pos->pceNum[wP]));

    // check bitboard squares
    while (t_pawns[WHITE])
    {
        int sq64 = PopBit(&t_pawns[WHITE]);
        ASSERT(pos->board[Sq64ToSq120[sq64]] == wP);
    }

    while (t_pawns[BLACK])
    {
        int sq64 = PopBit(&t_pawns[BLACK]);
        ASSERT(pos->board[Sq64ToSq120[sq64]] == bP);
    }

    while (t_pawns[BOTH])
    {
        int sq64 = PopBit(&t_pawns[BOTH]);
        ASSERT((pos->board[Sq64ToSq120[sq64]] == bP) || (pos->board[Sq64ToSq120[sq64]] == wP));
    }

    ASSERT(t_material[WHITE] == pos->material[WHITE] && t_material[BLACK] == pos->material[BLACK]);
    ASSERT(t_minPce[WHITE] == pos->minPce[WHITE] && t_minPce[BLACK] == pos->minPce[BLACK]);
    ASSERT(t_majPce[WHITE] == pos->majPce[WHITE] && t_majPce[BLACK] == pos->majPce[BLACK]);
    ASSERT(t_bigPce[WHITE] == pos->bigPce[WHITE] && t_bigPce[BLACK] == pos->bigPce[BLACK]);

    ASSERT(pos->side == WHITE || pos->side == BLACK);
    ASSERT(GeneratePosKey(pos) == pos->posKey);

    ASSERT(pos->enPas == NO_SQ || (RanksBrd[pos->enPas] == RANK_6 && pos->side == WHITE) || (RanksBrd[pos->enPas] == RANK_3 && pos->side == BLACK));

    ASSERT(pos->board[pos->KingSq[WHITE]] == wK);
    ASSERT(pos->board[pos->KingSq[BLACK]] == bK);

    return true;
}

void UpdateListsMaterial(S_BOARD *pos)
{

    for (int index = 0; index < BRD_SQ_NUM; index++)
    {
        int sq = index;
        int piece = pos->board[sq];
        if (piece != 120 && piece != EMPTY)
        {
            int color = PieceCol[piece];
            if (PieceBig[piece] == TRUE)
                pos->bigPce[color]++;
            if (PieceMin[piece] == TRUE)
                pos->minPce[color]++;
            if (PieceMaj[piece] == TRUE)
                pos->majPce[color]++;

            pos->material[color] += PieceVal[piece];
            pos->pList[piece][pos->pceNum[piece]] = sq;
            pos->pceNum[piece]++;

            if (piece == wK)
                pos->KingSq[WHITE] = sq;
            if (piece == bK)
                pos->KingSq[BLACK] = sq;

            if (piece == wP)
            {
                SETBIT(pos->pawns[WHITE], Sq120ToSq64[sq]);
                SETBIT(pos->pawns[BOTH], Sq120ToSq64[sq]);
            }
            else if (piece == bP)
            {
                SETBIT(pos->pawns[BLACK], Sq120ToSq64[sq]);
                SETBIT(pos->pawns[BOTH], Sq120ToSq64[sq]);
            }
        }
    }
}

int ParseFen(const char *fen, S_BOARD *pos)
{
    ASSERT(fen != NULL);
    ASSERT(pos != NULL);

    ResetBoard(pos);

    int rank = RANK_8;
    int file = FILE_A;
    int piece = 0;
    int count = 0; // To count the number of empty squares
    int i = 0;
    int sq64 = 0;
    int sq120 = 0;

    while ((rank >= RANK_1) && *fen)
    {
        count = 1;
        switch (*fen)
        {
        case 'p':
            piece = bP;
            break;
        case 'r':
            piece = bR;
            break;
        case 'n':
            piece = bN;
            break;
        case 'b':
            piece = bB;
            break;
        case 'k':
            piece = bK;
            break;
        case 'q':
            piece = bQ;
            break;
        case 'P':
            piece = wP;
            break;
        case 'R':
            piece = wR;
            break;
        case 'N':
            piece = wN;
            break;
        case 'B':
            piece = wB;
            break;
        case 'K':
            piece = wK;
            break;
        case 'Q':
            piece = wQ;
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            piece = EMPTY;
            count = *fen - '0'; // Converts the char to an int
            break;

        case '/':
        case ' ':
            rank--;
            file = FILE_A;
            fen++;
            continue;

        default:
            std::cout << "FEN error" << std::endl;
            return -1;
        }

        for (i = 0; i < count; i++)
        {
            sq64 = rank * 8 + file;
            sq120 = Sq64ToSq120[sq64];
            if (piece != EMPTY)
            {
                pos->board[sq120] = piece;
            }
            file++;
        }
        fen++;
    }

    ASSERT(*fen == 'w' || *fen == 'b');
    pos->side = (*fen == 'w') ? WHITE : BLACK;

    fen += 2;

    while (*fen != ' ')
    {
        switch (*fen)
        {
        case 'K':
            pos->castlePerm |= WKCA;
            break;
        case 'Q':
            pos->castlePerm |= WQCA;
            break;
        case 'k':
            pos->castlePerm |= BKCA;
            break;
        case 'q':
            pos->castlePerm |= BQCA;
            break;
        }
        fen++;
    }

    fen++;

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    if (*fen != '-')
    {
        file = fen[0] - 'a';
        rank = fen[1] - '1'; // Subtracting '1' from the char gives us the int value of the char
        ASSERT(file >= FILE_A && file <= FILE_H);
        ASSERT(rank >= RANK_1 && rank <= RANK_8);
        pos->enPas = FR2SQ(file, rank);
    }

    pos->posKey = GeneratePosKey(pos);

    UpdateListsMaterial(pos);

    return 0;
}

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
        pos->material[i] = 0;
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

void PrintBoard(const S_BOARD *pos)
{

    for (int rank = RANK_8; rank >= RANK_1; rank--)
    {
        std::cout << rank + 1 << "  ";
        for (int file = FILE_A; file <= FILE_H; file++)
        {
            int sq120 = FR2SQ(file, rank);
            int piece = pos->board[sq120];
            std::cout << PceChar[piece] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl
              << "  ";
    for (int file = FILE_A; file <= FILE_H; file++)
    {
        std::cout << " " << (char)('a' + file);
    }

    std::cout << std::endl
              << std::endl;
    std::cout << "side : " << SideChar[pos->side] << std::endl;
    std::cout << "enPas : " << pos->enPas << std::endl;
    std::cout << "castle : " << pos->castlePerm && WKCA ? 'K' : '-' << pos->castlePerm && WQCA ? 'Q'
                                                            : '-' << pos->castlePerm && BKCA   ? 'k'
                                                            : '-' << pos->castlePerm && BQCA   ? 'q'
                                                                                               : '-';
    std::cout << std::endl;

    std::cout << "PosKey : " << pos->posKey << std::endl;
}
