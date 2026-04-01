#include "Board.h"
#include <iostream>
#include <random>

void Board::GenerateBoard(unsigned boardSize)
{
    DrawBoardCorner();
    for (int i = 1; i <= boardSize; i++)
    {
        DrawIndex(i);
    }
    cout << endl;
    
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> randomCandyType(1, static_cast<int>(ECandyType::COUNT) - 1);

    for (int i = 0; i < pow(boardSize, 2); i++)
    {
        if (i % boardSize == 0)
        {
            if (i / boardSize > 0)
            {
                cout << endl;
            }
            DrawIndex((i / 5) + 1);
        }
        Slot* slot = new Slot();
        slot->setCandy(static_cast<ECandyType>(randomCandyType(gen)));
        slot->setIndex(i);
        cout << " " << candyTypeLiterals[slot->getCandy()] << " ";
    }
}

void Board::DrawBoardCorner()
{
    cout << "   ";
}

void Board::DrawIndex(const unsigned i)
{
    cout << " " << i << " ";
}
