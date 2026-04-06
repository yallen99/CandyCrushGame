#include "Board.h"
#include <iostream>
#include <random>

void Board::CheckForMatch(const vector<Slot*>& inSlots, const string& marker)
{
    unsigned matchCount = 1;
    vector<unsigned> matchIndexes;
    matchIndexes.reserve(pow(rowSize, 2));

    /// horizontal check
    for (unsigned i = 1; i < inSlots.size(); i++)
    {
        if (inSlots[i]->getCandy() == inSlots[i - 1]->getCandy()
            && i % rowSize != 0)
        {
            if (i == inSlots.size() - 1 && matchCount < MIN_MATCH - 1)
            {
                continue;
            }
            matchCount++;
            matchIndexes.push_back(i - 1);
        }
        else
        {
            if (matchCount >= MIN_MATCH)
            {
                matchIndexes.push_back(i - 1);
                matchCount = 1;
            }
            else
            {
                if (matchCount > 1)
                {
                    while (matchCount > 1)
                    {
                        matchIndexes.pop_back();
                        matchCount--;
                    }
                }
            }
        }
        /// If we reached the end of a row, reset the counter
        if (i % rowSize == 0)
        {
            matchCount = 1;
        }
    }
   
    for (unsigned matchIndex : matchIndexes)
    {
        slots[matchIndex]->setVisualOutput("\033[1;37m" + marker + "\033[1;37m");
    }

    ///////////////////////////////////////////////////////////////
    unsigned vertMatchCount = 1;
    vector<unsigned> vertMatchIndexes;
    vertMatchIndexes.reserve(inSlots.size());
 
    /// vertical check
    for (unsigned j = 0, i = rowSize + j; i < inSlots.size(); i += rowSize)
    {
        if (j >= rowSize)
        {
            break;
        }
        if (inSlots[i]->getCandy() == inSlots[i - rowSize]->getCandy())
        {
            vertMatchCount++;
            vertMatchIndexes.emplace_back(i - rowSize);
            if (i == static_cast<unsigned>(pow(rowSize, 2) - (rowSize - j)) && vertMatchCount >= MIN_MATCH)
            {
                vertMatchIndexes.emplace_back(i);
            }
            else if (i == static_cast<unsigned>(pow(rowSize, 2) - (rowSize - j)) && vertMatchCount < MIN_MATCH)
            {
                while (vertMatchCount > 1)
                {
                    vertMatchIndexes.pop_back();
                    vertMatchCount--;
                }
            }
        }
        else
        {
            if (vertMatchCount >= MIN_MATCH)
            {
                vertMatchIndexes.emplace_back(i - rowSize);
                vertMatchCount = 1;
            }
            else
            {
                if (vertMatchCount > 1)
                {
                    while (vertMatchCount > 1)
                    {
                        vertMatchIndexes.pop_back();
                        vertMatchCount--;
                    }
                }
            }
        }
        if (i % static_cast<int>(pow(rowSize, 2) - (rowSize - j)) == 0)
        {
            vertMatchCount = 1;
            j++;
            i = j;
        }
    }
   
    for (unsigned matchIndex : vertMatchIndexes)
    {
        slots[matchIndex]->setVisualOutput("\033[1;37m" + marker + "\033[1;37m");
    }
}

void Board::GenerateBoardSlots(unsigned boardSize)
{
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> randomCandyType(1, static_cast<int>(ECandyType::COUNT) - 1);

    for (unsigned i = 0; i < pow(boardSize, 2); i++)
    {
        Slot* slot = new Slot();
        slot->setCandy(static_cast<ECandyType>(randomCandyType(gen)));
        slot->setIndex(static_cast<int>(i));
        slot->setVisualOutput(candyTypeLiterals[slot->getCandy()]);
        slots.push_back(slot);
    }
}

void Board::DrawBoardCorner()
{
    cout << "   ";
}

void Board::DrawFullBoard()
{
    DrawBoardCorner();
    for (unsigned i = 1; i <= rowSize; i++)
    {
        DrawIndex(i);
    }
    cout << endl;
    for (unsigned i = 0; i < pow(rowSize, 2); i++)
    {
        if (i % rowSize == 0)
        {
            if (i / rowSize > 0)
            {
                cout << endl;
            }
            DrawIndex(floor(i / rowSize) + 1);
        }
        DrawCandy(*slots[i]);
    }
    cout << "\033[1;37m \033[1;37m" << endl;
}

void Board::DrawIndex(const unsigned i)
{
    /// draw the indexes in white
    cout << " " << "\033[1;37m"<< i <<"\033[1;37m" << " ";
}
void Board::DrawCandy(const Slot& slotToDraw)
{
    cout << " " << slotToDraw.getVisualOutput() << " ";
}