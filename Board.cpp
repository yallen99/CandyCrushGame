#include "Board.h"
#include <iostream>
#include <random>

#include "GameManager.h"

bool Board::CheckForMatch(const string& marker)
{
    bool foundMatch = false;
    unsigned horMatchCount = 1;
    vector<unsigned> matchIndexes;
    matchIndexes.reserve(pow(rowSize, 2));

    /// horizontal check
    for (unsigned i = 1; i < slots.size(); i++)
    {
        if (slots[i]->IsEqual(*slots[i - 1])
            && i % rowSize != 0)
        {
            if (i == slots.size() - 1 && horMatchCount < MIN_MATCH - 1)
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
    for (unsigned j = 0, i = rowSize + j; i < slots.size(); i += rowSize)
    {
        // currLastRowIndex = given the current column, the index of i on the last row (ex: 42 to 48 for a 7x7 board)
        unsigned currLastRowIndex = static_cast<unsigned>(pow(rowSize, 2) - (rowSize - j));
        if (j >= rowSize)
        {
            break;
        }
        if (slots[i]->IsEqual(*slots[i - rowSize]))
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
        slots[matchIndex]->setCandy(ECandyType::NONE);
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

void Board::FillMatchedSlots(vector<unsigned> matchedIndexes)
{
    cout << endl;
    cout << " -_-_-_- FILLING MATCHES -_-_-_- " << endl;
    cout << endl;
    sort(matchedIndexes.begin(), matchedIndexes.end(),
        [] (const unsigned& a, const unsigned& b)
        {
            return a > b;
        });

    // cycle column by column, bottom to top (42 - 35 - 28 - 21 - 14 - 7 etc.)
    for (int column = 0, i = static_cast<int>(pow(rowSize, 2) - (rowSize - column)); i >= 0; i -= rowSize)
    {
        if (column > rowSize - 1 || i >= static_cast<int>(pow(rowSize, 2)))
        {
            break;
        }
        if (slots[i]->getCandy() == ECandyType::NONE)
        {
            if (i < rowSize)
            {
                slots[i]->setCandy(ECandyType::NONE);
                column++;
                i = static_cast<int>(pow(rowSize, 2) + column);
                continue;
            }
            int emptySlotsFound = 0;
            emptySlotsFound++;
            int anchorIndex = i;
            // count how many empty slots upwards we have
            while (anchorIndex > 0)
            {
                if (anchorIndex - rowSize < 0)
                {
                    break;
                }
                anchorIndex -= rowSize;
                    
                if (slots[anchorIndex]->getCandy() == ECandyType::NONE)
                {
                    emptySlotsFound++;
                }
                else
                {
                    break;
                }
            }
            // move down the candies sequentially
            int itr = 0;
            //cout << "Found " << emptySlotsFound << " empty slots on column " << column << endl;
            while (itr < emptySlotsFound)
            {
                int a = 0; // additional search index
                int currEmptySlotIndex = i - (itr * rowSize);
                int nextFullSlotIndex = i - rowSize * (emptySlotsFound + itr + a);
                if (nextFullSlotIndex < 0)
                {
                    break;
                }
                if (slots[nextFullSlotIndex]->getCandy() == ECandyType::NONE)
                {
                    int itr2 = nextFullSlotIndex;
                    while (slots[nextFullSlotIndex]->getCandy() == ECandyType::NONE && itr2 > 0)
                    {
                        a++;
                        itr2--;
                    }
                }
                slots[currEmptySlotIndex]->setCandy(slots[nextFullSlotIndex]->getCandy());
                slots[nextFullSlotIndex]->setCandy(ECandyType::NONE);
                //cout << "Moved down " << nextFullSlotIndex << " to " << currEmptySlotIndex << endl;
                //cout << "Additional search index: " << a << endl;
                itr++;
            }
        }
        if (i - rowSize < 0)
        {
            column++;
            i = static_cast<int>(pow(rowSize, 2) + column);
        }
    }
    cout << endl;

    // Fill the empty slots with random new candies
    for (int i = 0; i < static_cast<int>(pow(rowSize, 2)) - 1; i++)
    {
        if (slots[i]->getCandy() == ECandyType::NONE)
        {
            GenerateRandomCandy(*slots[i]);
        }
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

void Board::PickCell(const int coordinates)
{
    currentlySelectedCell = coordinates;
    
    /* const int row = (cellCoord / 10) - 1;
    const int column = (cellCoord % 10) - 1;
    slots[rowSize * row + column]->setVisualOutput("\033[1;37mo\033[1;37m");
    DrawFullBoard();*/
}

void Board::HighlightDirection(const EDirection& direction)
{
    if (currentlySelectedCell < 0)
    {
        return;
    }
    int prevRow = (currentlySelectedCell / 10) - 1;
    int prevColumn = (currentlySelectedCell % 10) - 1;
    int row = prevRow;
    int column = prevColumn;
    switch (direction)
    {
        case EDirection::UP:
            row -= 1;
            break;
        case EDirection::DOWN:
            row += 1;
            break;
        case EDirection::LEFT:
            column -= 1;
            break;
        case EDirection::RIGHT:
            column += 1;
            break;
    }
    if (column < 0 || row < 0 || column >= rowSize || row >= rowSize)
    {
        cout << "Invalid direction!" << endl;
        return;
    }

    // do the swap
    Slot& firstSlot = *slots[rowSize * prevRow + prevColumn];
    Slot& secondSlot = *slots[rowSize * row + column];
    const ECandyType firstType = slots[rowSize * prevRow + prevColumn]->getCandy();
    const ECandyType secondType = slots[rowSize * row + column]->getCandy();

    firstSlot.setCandy(secondType);
    secondSlot.setCandy(firstType);

    // check if the swap was a successful match
    if (!CheckForMatch("x"))
    {
        firstSlot.setCandy(firstType);
        secondSlot.setCandy(secondType);
        CandyCrushStrings::PrintGameEvent("NO MATCH FOR SELECTION!");
    }

    currentlySelectedCell = -1;
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