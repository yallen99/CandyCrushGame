#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

using namespace std;

#define EASY_BOARD 5
#define MEDIUM_BOARD 7
#define HARD_BOARD 13

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
    {ECandyType::A, "A"},
    {ECandyType::B, "B"},
    {ECandyType::C, "C"},
    {ECandyType::D, "D"},
};

class Slot
{
public:
    const ECandyType& getCandy() const { return candy; }
    void setCandy(const ECandyType& newType) { candy = newType; }

    const int getIndex() const { return index; }
    void setIndex(const int newIndex) { index = newIndex; }

private:
    ECandyType candy = ECandyType::NONE;
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
        slots.reserve(pow(size, 2));
        GenerateBoard(size);
    }
    
private:
    vector<Slot> slots;
    void GenerateBoard(unsigned boardSize);

    static void DrawBoardCorner();
    static void DrawIndex(const unsigned i);
    static void DrawCandy(const unsigned i);
};
