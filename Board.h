#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

using namespace std;

#define EASY_BOARD 5
#define MEDIUM_BOARD 7
#define HARD_BOARD 9

#define MIN_MATCH 3

enum class ECandyType: uint8_t
{
    NONE,
    A,
    B,
    C,
    D,
    COUNT
};

static map<ECandyType, string> candyTypeLiterals =
{
    {ECandyType::NONE, ""},
    {ECandyType::A, "\033[32mo\033[0m"},
    {ECandyType::B, "\x1B[31mo\x1B[31m"},
    {ECandyType::C, "\x1B[36mo\x1B[36m"},
    {ECandyType::D, "\033[35mo\033[35m"},
};

class Slot
{
public:
   
    const ECandyType& getCandy() const { return candy; }
    void setCandy(const ECandyType& newType) { candy = newType; }

    const int getIndex() const { return index; }
    void setIndex(const int newIndex) { index = newIndex; }

    const string& getVisualOutput() const { return visualOutput; }
    void setVisualOutput(const string& newVisualOutput) { visualOutput = newVisualOutput; }

private:
    ECandyType candy = ECandyType::NONE;
    string visualOutput = candyTypeLiterals[candy];
    int index = -1;
};

/// The board would look something like this, with indexes above and to the left for indication:
/// X would be initially a candy at random from the ECandyType 
///     1   2   3   4   5   6   N
/// 1   x   x   x   x   x   x   x
/// 2   x   x   x   x   x   x   x
/// 3   x   x   x   x   x   x   x
/// 4   x   x   x   x   x   x   x
/// 5   x   x   x   x   x   x   x
/// 6   x   x   x   x   x   x   x
/// N   x   x   x   x   x   x   x
class Board
{
public:
    Board(const uint8_t size)
    {
        rowSize = size;
        slots.reserve(pow(size, 2));
        GenerateBoardSlots(size);
        DrawFullBoard();
    }

    ~Board()
    {
        for (const Slot* slot : slots)
        {
            delete slot;
        }
    }

    const uint8_t getSize() const { return rowSize; }
    const vector<Slot*>& getSlots() const { return slots; }
    bool CheckForMatch(const vector<Slot*>& inSlots, const string& marker);
    void DrawFullBoard();

private:
    uint8_t rowSize = 0;
    vector<Slot*> slots;
    void GenerateBoardSlots(unsigned boardSize);
    
    void FillMatchedSlots(const vector<unsigned>& matchedIndexes);

    static Slot* GenerateRandomCandy(Slot& currentCandy);
    static void DrawBoardCorner();
    static void DrawIndex(const unsigned i);
    static void DrawCandy(const Slot& slotToDraw);
};





