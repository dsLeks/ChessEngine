#include "defs.h"

int main()
{
    AllInit();

    U64 playBitBoard = 0ULL;

    playBitBoard |= (1ULL << Sq120ToSq64[D2]);
    playBitBoard |= (1ULL << Sq120ToSq64[D3]);
    playBitBoard |= (1ULL << Sq120ToSq64[D4]);

    PrintBoard(playBitBoard);

    int count = CountBits(playBitBoard);
    std::cout << "Count: " << count << std::endl
              << std::endl;

    int index = PopBit(&playBitBoard);
    std::cout << "Index: " << index << std::endl;

    PrintBoard(playBitBoard);

    return 0;
}