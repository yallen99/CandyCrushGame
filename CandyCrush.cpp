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
        SHUFFLE
    };

    static map<EInfoBlock, string> extendedInfo =
    {
        {EInfoBlock::HELP, "help"},
        {EInfoBlock::START, "Type \"start\" to start the game"},
        {EInfoBlock::END, "Type \"end\" to end the game"},
        {EInfoBlock::RESTART, "Type \"restart\" to end the game"},
        {EInfoBlock::SHUFFLE, "Type \"shuffle\" to shuffle the current board"},
    };

    static map<EInfoBlock, string> commands =
    {
        {EInfoBlock::HELP, "help"},
        {EInfoBlock::START, "start"},
        {EInfoBlock::END, "end"},
        {EInfoBlock::RESTART, "restart"},
        {EInfoBlock::SHUFFLE, "shuffle"},
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
int main()
{
    cout << "Type \"help\" to see the list of available commands." << endl;
    cout << "Type \"start\" to Start!" << endl;
    string input;
    do
    {
        cin >> input;
        if (input == commands[EInfoBlock::HELP])
        {
            PrintHelpBlock();
        }
        if (input == commands[EInfoBlock::START])
        {
            Board newBoard(EASY_BOARD);
        }
        if (input == commands[EInfoBlock::END])
        {
            break;
        }
        
    } while (true);
    return 0;
}
