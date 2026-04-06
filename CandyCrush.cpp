#include <iostream>
#include <map>
#include <string>

#include "Board.h"

using namespace std;

namespace GameCommands
{
    enum class EInfoBlock: uint8_t
    {
        HELP,
        START,
        END,
        RESTART,
    };

    static map<EInfoBlock, string> extendedInfo =
    {
        {EInfoBlock::HELP, "help"},
        {EInfoBlock::START, "Type \"start\" to start the game"},
        {EInfoBlock::END, "Type \"end\" to end the game"},
        {EInfoBlock::RESTART, "Type \"restart\" to end the game"},
    };

    static map<EInfoBlock, string> commands =
    {
        {EInfoBlock::HELP, "help"},
        {EInfoBlock::START, "start"},
        {EInfoBlock::END, "end"},
        {EInfoBlock::RESTART, "restart"},
    };

    static void PrintHelpBlock()
    {
        for (const auto& helpText : extendedInfo)
        {
            if (helpText.first != EInfoBlock::HELP)
            {
                cout << helpText.second << endl;
            }
        }
    }
}

using namespace GameCommands;

void CheckMatch(Board& board)
{
    cout << endl;
    cout << " -_-_-_- CHECKING MATCH -_-_-_- " << endl;
    cout << endl;
    
    while (board.CheckForMatch(board.getSlots(), "x"))
    {
        cout << endl;
        cout << " -_-_-_- NEW BOARD -_-_-_- " << endl;
        cout << endl;
    }
    
    cout << endl;
    cout << " ************ Your turn! ************ " << endl;
    cout << endl;
    cout << " -_-_-_- input row - column -_-_-_- " << endl;
    cout << endl;
}

int main()
{
    cout << "Type \"help\" to see the list of available commands." << endl;
    cout << "Type \"start\" to Start!" << endl;
    string input;
    unique_ptr<Board> board = nullptr;
    do
    {
        int boardSize = HARD_BOARD;
        cin >> input;
        if (input == commands[EInfoBlock::HELP])
        {
            PrintHelpBlock();
        }
        if (input == commands[EInfoBlock::START])
        {
            board = make_unique<Board>(boardSize);
            CheckMatch(*board);
        }
        if (input > "0" && input.length() == 2)
        {
            const int cellCoords = stoi(input);
            cout << endl;
            board->HighlightCell(cellCoords);
        }
        // Direction checks
        if (input == directionStrings[EDirection::UP])
        {
            board->HighlightDirection(EDirection::UP);
        }
        if (input == directionStrings[EDirection::DOWN])
        {
            board->HighlightDirection(EDirection::DOWN);
        }
        if (input == directionStrings[EDirection::LEFT])
        {
            board->HighlightDirection(EDirection::LEFT);
        }
        if (input == directionStrings[EDirection::RIGHT])
        {
            board->HighlightDirection(EDirection::RIGHT);
        }
        if (input == commands[EInfoBlock::END])
        {
            break;
        }
        
    } while (true);
    return 0;
}
