# pragma once
#include <iostream>
#include <map>
#include <string>

using namespace std;

namespace GameCommands
{
    enum class EInfoBlock: uint8_t
    {
        HELP,
        END,
        RESTART,
        SHUFFLE
    };

    static map<EInfoBlock, string> extendedInfo =
    {
        {EInfoBlock::HELP, "help"},
        {EInfoBlock::END, "Type \"end\" to end the game"},
        {EInfoBlock::RESTART, "Type \"restart\" to end the game"},
        {EInfoBlock::SHUFFLE, "Type \"shuffle\" to shuffle the current board"},
    };

    static map<EInfoBlock, string> commands =
    {
        {EInfoBlock::HELP, "help"},
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
    cout << "Type \"help\" to see the list of available commands:" << endl;
    string input;
    do
    {
        cin >> input;
        if (input == commands[EInfoBlock::HELP])
        {
            PrintHelpBlock();
        }
        if (input == commands[EInfoBlock::END])
        {
            break;
        }
        
    } while (true);
    return 0;
}
