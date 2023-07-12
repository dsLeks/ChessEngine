#include <iostream>
#include <cstdlib>

typedef uint64_t U64;

// ASCII characters representing the pieces
const char *pieces[] = {
    "\u2659", "\u2658", "\u2657", "\u2656", "\u265B", "\u2654", // White pieces
    "\u265F", "\u265E", "\u265D", "\u265C", "\u265B", "\u265A"  // Black pieces
};

class Board
{
public:
    U64 whitePawns;
    U64 whiteKnights;
    U64 whiteBishops;
    U64 whiteRooks;
    U64 whiteQueens;
    U64 whiteKing;

    U64 blackPawns;
    U64 blackKnights;
    U64 blackBishops;
    U64 blackRooks;
    U64 blackQueens;
    U64 blackKing;
    Board()
    {
        whitePawns = 0xff00;
        whiteKnights = 0x42;
        whiteBishops = 0x24;
        whiteRooks = 0x81;
        whiteQueens = 0x10;
        whiteKing = 0x8;

        blackPawns = 0xff000000000000;
        blackKnights = 0x4200000000000000;
        blackBishops = 0x2400000000000000;
        blackRooks = 0x8100000000000000;
        blackQueens = 0x1000000000000000;
        blackKing = 0x800000000000000;
    }

    void PrintBoard()
    {
        std::cout << "  a b c d e f g h" << std::endl;
        std::cout << "  ----------------" << std::endl;

        // Iterate over each square of the board
        for (int rank = 7; rank >= 0; --rank)
        {
            std::cout << rank + 1 << "|";
            for (int file = 0; file < 8; ++file)
            {
                // Calculate the bit corresponding to the current square
                U64 square = 1ULL << (rank * 8 + file);

                // Check if the square contains a white piece
                if (whitePawns & square)
                    std::cout << pieces[0] << " ";
                else if (whiteKnights & square)
                    std::cout << pieces[1] << " ";
                else if (whiteBishops & square)
                    std::cout << pieces[2] << " ";
                else if (whiteRooks & square)
                    std::cout << pieces[3] << " ";
                else if (whiteQueens & square)
                    std::cout << pieces[4] << " ";
                else if (whiteKing & square)
                    std::cout << pieces[5] << " ";
                // Check if the square contains a black piece
                else if (blackPawns & square)
                    std::cout << pieces[6] << " ";
                else if (blackKnights & square)
                    std::cout << pieces[7] << " ";
                else if (blackBishops & square)
                    std::cout << pieces[8] << " ";
                else if (blackRooks & square)
                    std::cout << pieces[9] << " ";
                else if (blackQueens & square)
                    std::cout << pieces[10] << " ";
                else if (blackKing & square)
                    std::cout << pieces[11] << " ";
                else
                    std::cout << ". ";
            }
            std::cout << "|" << std::endl;
        }

        std::cout << "  ----------------" << std::endl;
    }
};