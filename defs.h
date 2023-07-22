#include <iostream>
#include <stdlib.h>

#define DEBUG ;

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                         \
    if (!(n))                             \
    {                                     \
        printf("%s - Failed", #n);        \
        printf("On %s ", __DATE__);       \
        printf("At %s ", __TIME__);       \
        printf("In File %s ", __FILE__);  \
        printf("At Line %d\n", __LINE__); \
        exit(1);                          \
    }
#endif

typedef uint64_t U64;
#define BRD_SQ_NUM 120 // 120 squares on the board

#define MAXGAMEMOVES 2048    // Maximum number of moves (halfmoves) in a game. A game typically does not go over 1000 half moves.
#define MAXPOSITIONMOVES 256 // Maximum number of positions that can be expected for a given position.

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" // Starting position of the boards

// Piece definitions
enum
{
    EMPTY,
    wP,
    wN,
    wB,
    wR,
    wQ,
    wK,
    bP,
    bN,
    bB,
    bR,
    bQ,
    bK,
};

// File and rank definitions
enum
{
    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,
    FILE_NONE
};
enum
{
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_NONE
};

enum
{
    WHITE,
    BLACK,
    BOTH
};

// Board square definitions
enum
{
    A1 = 21,
    B1,
    C1,
    D1,
    E1,
    F1,
    G1,
    H1,
    A2 = 31,
    B2,
    C2,
    D2,
    E2,
    F2,
    G2,
    H2,
    A3 = 41,
    B3,
    C3,
    D3,
    E3,
    F3,
    G3,
    H3,
    A4 = 51,
    B4,
    C4,
    D4,
    E4,
    F4,
    G4,
    H4,
    A5 = 61,
    B5,
    C5,
    D5,
    E5,
    F5,
    G5,
    H5,
    A6 = 71,
    B6,
    C6,
    D6,
    E6,
    F6,
    G6,
    H6,
    A7 = 81,
    B7,
    C7,
    D7,
    E7,
    F7,
    G7,
    H7,
    A8 = 91,
    B8,
    C8,
    D8,
    E8,
    F8,
    G8,
    H8,
    NO_SQ
};

enum
{
    FALSE,
    TRUE
};

// Castling permissions.
enum
{
    WKCA = 1,
    WQCA = 2,
    BKCA = 4,
    BQCA = 8,
};

typedef struct
{
    int move;
    int score;
} S_MOVE;

typedef struct
{
    S_MOVE moves[MAXPOSITIONMOVES];
    int count;
} S_MOVELIST;

typedef struct
{
    int move;
    int castlePerm;
    int enPas;
    int fiftyMove;
    U64 posKey; // Unique number representing the position on the board. Used to check for 3 fold repetition.
} S_UNDO;

// Board structure declaration
typedef class S_BOARD
{
public:
    int board[BRD_SQ_NUM]; // The board is stored as an array of 120 integers. Each integer represents a square on the board. The value of the integer represents the piece on that square. Ex: board[A1] = wR means there is a white rook on A1.
    U64 pawns[3];          // 3 bitboards for white pawns, black pawns, and all pawns (both black and white).

    int KingSq[2]; // The square of the white king and the square of the black king.

    int side;
    int enPas;
    int fiftyMove;
    int ply;    // Number of half moves. Ex: if white moves and black moves, ply = 2. If only white has moved, ply = 1.
    int hisPly; // Number of half moves in the history. Ex: if white moves and black moves, hisPly = 1. If only white has moved, hisPly = 0.

    int castlePerm; // Castling permissions. Castleing permissions are stored as a 4 bit number. Ex: 0000 means no castling, 0001 means white can castle kingside, 0010 means white can castle queenside, 0011 means white can castle both sides, 0100 means black can castle kingside, 1000 means black can castle queenside, 1100 means black can castle both sides.

    U64 posKey; // A Unique key generated for each position.

    // These are used for position evaluation.
    int pceNum[13];  // The number of pieces on the baord. Ex: pceNum[wP] = 8 means there are 8 white pawns on the board.
    int bigPce[2];   // Anything that is not a pawn. Ex: bigPce[WHITE] = 3 means there are 3 white pieces on the board that are not pawns.
    int majPce[2];   // Major pieces are rooks and queens. Ex: majPce[BLACK] = 2 means there are 2 black major pieces on the board.
    int minPce[2];   // Minor pieces are bishops and knights. Ex: minPce[WHITE] = 2 means there are 2 white minor pieces on the board.
    int material[2]; // Holds Material score for black and white.

    S_UNDO history[MAXGAMEMOVES]; // Array of undo structures. Each undo structure stores the move, castlePerm, enPas, fiftyMove, and posKey for a given move.

    int pList[13][10]; // List of pieces. Ex: pList[wP][0] = 21 means the first white pawn is on square 21. This is initliazed to no square at first.
} S_BOARD;

// Note: My Offboard is 120.

/* -- GAME MOVE -- */
/*
0000 0000 0000 0000 0000 0111 1111 -> From 0x7F
0000 0000 0000 0011 1111 1000 0000 -> To >> 7, 0x7F
0000 0000 0011 1100 0000 0000 0000 -> Captured >> 14, 0xF
0000 0000 0100 0000 0000 0000 0000 -> EP 0x40000
0000 0000 1000 0000 0000 0000 0000 -> Pawn Start 0x80000
0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece >> 20, 0xF
0001 0000 0000 0000 0000 0000 0000 -> Castle 0x1000000
*/

#define FROMSQ(m) ((m)&0x7F)            // Returns the from square of a move.
#define TOSQ(m) (((m) >> 7) & 0x7F)     // Returns the to square of a move.
#define CAPTURED(m) (((m) >> 14) & 0xF) // Returns the captured piece of a move.
#define PROMOTED(m) (((m) >> 20) & 0xF) // Returns the promoted piece of a move.
#define MFLAGEP 0x40000                 // En Passant flag.
#define MFLAGPS 0x80000                 // Pawn Start flag.
#define MFLAGCA 0x1000000               // Castle flag.

#define MFLAGCAP 0x7C000   // This flag will tell us if the move was a capturing move. Did it capture something in the move?
#define MFLAGPROM 0xF00000 // Was the move a promotion?

/*Macros*/
/*-------*/
#define FR2SQ(f, r) ((21 + (f)) + ((r)*10))       // Given a File (f) and Rank (r) it returns the corresponding square on the 120 square board.
#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)]); // Clearss a bit on a bitboard.
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)]);   // Sets a bit on a bitboard.

#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])

/* GLOBALS */
/*----------*/
// Macros for converting between the 120 square board and the 64 square board. These will be used to quickly get the square number conversions from the 120 board to the 64 board and vice versa.
extern int Sq120ToSq64[BRD_SQ_NUM]; // Converts a 120 square to a 64 square.
extern int Sq64ToSq120[64];         // Converts a 64 square to a 120 square.
extern U64 SetMask[64];             // Masks for setting a bit on a bitboard.
extern U64 ClearMask[64];           // Masks for clearing a bit on a bitboard.
extern U64 PieceKeys[13][120];      // Piece keys will hash a random number for each piece on each square. Ex: PieceKeys[wP][A1] will hash a random number for a white pawn on A1.
extern U64 SideKey;                 // Side to move key will hash a random number if it is white to move. If it is black to move it will hash a different random number.
extern U64 CastleKeys[16];

// Array of characters for printing the board.
extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

// Arrays for piece information.
extern bool PieceBig[13];
extern bool PieceMaj[13];
extern bool PieceMin[13];
extern int PieceVal[13];
extern int PieceCol[13];
extern bool PieceSlides[13];
extern int knightMoves[8];
extern int kingMoves[8];
extern int bishopMoves[4];
extern int rookMoves[4];
extern int queenMoves[8];

// To get the type of piece. i.e. is it a knight, king, rook/queen, or bishop/queen?
extern bool PieceKnight[13];
extern bool PieceKing[13];
extern bool PieceRookQueen[13];
extern bool PieceBishopQueen[13];

extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

extern void AllInit();
extern void InitSq120To64();
extern void PrintBitBoard(U64 board);
extern int PopBit(U64 *bb);
extern int CountBits(U64 b);
extern void ResetBoard(S_BOARD *pos);
extern U64 GeneratePosKey(const S_BOARD *pos);
extern int ParseFen(const char *fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);
extern void UpdateListsMaterial(S_BOARD *pos);
extern void InitFilesRankBrd();
extern bool CheckBoard(const S_BOARD *pos);
extern int SqAttacked(const int sq, const int side, const S_BOARD *pos);
extern char *PrSq(const int sq);
extern char *PrMove(const int move);
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid(const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);
extern void PrintMoveList(const S_MOVELIST *moveList);
extern void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *moveList);