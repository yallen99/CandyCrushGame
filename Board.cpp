#include "Board.h"
#include <iostream>
#include <random>

bool Board::CheckForMatch(const vector<Slot*>& inSlots, const string& marker)
{
    bool foundMatch = false;
    unsigned horMatchCount = 1;
    vector<unsigned> matchIndexes;
    matchIndexes.reserve(pow(rowSize, 2));

    /// horizontal check
    for (unsigned i = 1; i < inSlots.size(); i++)
    {
        if (inSlots[i]->getCandy() == inSlots[i - 1]->getCandy()
            && i % rowSize != 0)
        {
            if (i == inSlots.size() - 1 && horMatchCount < MIN_MATCH - 1)
            {
                continue;
            }
            horMatchCount++;
            matchIndexes.push_back(i - 1);
        }
        else
        {
            if (horMatchCount >= MIN_MATCH)
            {
                matchIndexes.push_back(i - 1);
                horMatchCount = 1;
            }
            else
            {
                if (horMatchCount > 1)
                {
                    while (horMatchCount > 1)
                    {
                        matchIndexes.pop_back();
                        horMatchCount--;
                    }
                }
            }
        }
        /// If we reached the end of a row, reset the counter
        if (i % rowSize == 0)
        {
            horMatchCount = 1;
        }
    }

    /// vertical check
    unsigned vertMatchCount = 1;
    for (unsigned j = 0, i = rowSize + j; i < inSlots.size(); i += rowSize)
    {
        // currLastRowIndex = given the current column, the index of i on the last row (ex: 42 to 48 for a 7x7 board)
        unsigned currLastRowIndex = static_cast<unsigned>(pow(rowSize, 2) - (rowSize - j));
        if (j >= rowSize)
        {
            break;
        }
        if (inSlots[i]->getCandy() == inSlots[i - rowSize]->getCandy())
        {
            vertMatchCount++;
            matchIndexes.emplace_back(i - rowSize);
            if (i == currLastRowIndex && vertMatchCount >= MIN_MATCH)
            {
                matchIndexes.emplace_back(i);
            }
            else if (i == currLastRowIndex && vertMatchCount < MIN_MATCH)
            {
                while (vertMatchCount > 1)
                {
                    matchIndexes.pop_back();
                    vertMatchCount--;
                }
            }
        }
        else
        {
            if (vertMatchCount >= MIN_MATCH)
            {
                matchIndexes.emplace_back(i - rowSize);
                vertMatchCount = 1;
            }
            else
            {
                if (vertMatchCount > 1)
                {
                    while (vertMatchCount > 1)
                    {
                        matchIndexes.pop_back();
                        vertMatchCount--;
                    }
                }
            }
        }
        if (i % currLastRowIndex == 0)
        {
            vertMatchCount = 1;
            j++;
            i = j;
        }
    }
   
    for (unsigned matchIndex : matchIndexes)
    {
        foundMatch = true;
        slots[matchIndex]->setVisualOutput("\033[1;37m" + marker + "\033[1;37m");
    }
    
    DrawFullBoard();
    if (foundMatch)
    {
        FillMatchedSlots(matchIndexes);
        DrawFullBoard();
    }

    return foundMatch;
}
Slot* Board::GenerateRandomCandy(Slot& currentCandy)
{
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> randomCandyType(1, static_cast<int>(ECandyType::COUNT) - 1);
    const ECandyType candyType = static_cast<ECandyType>(randomCandyType(gen));
    currentCandy.setCandy(candyType);
    currentCandy.setVisualOutput(candyTypeLiterals[candyType]);
    return &currentCandy;
}

void Board::FillMatchedSlots(const vector<unsigned>& matchedIndexes)
{
    cout << endl;
    cout << " -_-_-_- FILLING MATCHES -_-_-_- " << endl;
    cout << endl;
    // Replace matched indexes with a new random candy (for now, no gravity @todo)
    for (unsigned matchIndex : matchedIndexes)
    {
        GenerateRandomCandy(*slots[matchIndex]);
    }
}

void Board::GenerateBoardSlots(unsigned boardSize)
{
    for (unsigned i = 0; i < pow(boardSize, 2); i++)
    {
        Slot* slot = new Slot();
        GenerateRandomCandy(*slot);
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
    cout << " " << "\033[1;37m"<< i <<"\033[1;37m" << " ";
}
void Board::DrawCandy(const Slot& slotToDraw)
{
    cout << " " << slotToDraw.getVisualOutput() << " ";
}