#pragma once
#include <cstdint>
#include <iostream>
#include <map>
#include <string>

class Board;
using namespace std;

enum class EInfoBlock: uint8_t
{
    help,
    start,
    end,
    restart,
};

enum class EGameState: uint8_t
{
    main_menu,
    checking_matches,
    awaiting_input_cell,
    awaiting_input_direction,
    game_end
};

namespace CandyCrushStrings
{
    static map<EInfoBlock, string> extendedInfo =
    {
        {EInfoBlock::help, "help"},
        {EInfoBlock::start, "Type \"start\" to start the game"},
        {EInfoBlock::end, "Type \"end\" to end the game"},
        {EInfoBlock::restart, "Type \"restart\" to end the game"},
    };

    static map<EInfoBlock, string> commands =
    {
        {EInfoBlock::help, "help"},
        {EInfoBlock::start, "start"},
        {EInfoBlock::end, "end"},
        {EInfoBlock::restart, "restart"},
    };

    static void PrintHelpBlock()
    {
        for (const auto& helpText : extendedInfo)
        {
            if (helpText.first != EInfoBlock::help)
            {
                cout << helpText.second << endl;
            }
        }
    }
    static void PrintGameEvent(const string& eventName)
    {
        cout << endl;
        cout << " -_-_-_- " + eventName + " -_-_-_- " << endl;
        cout << endl;
    }

    static void PrintGameStart()
    {
        cout << "Type \"help\" to see the list of available commands." << endl;
        cout << "Type \"start\" to Start!" << endl;
    }

    inline string GetSanitisedInput(const string& input)
    {
        string sanitisedString;
        for (auto chr : input)
        {
            chr = tolower(chr); 
            sanitisedString.push_back(chr);
        }
        return sanitisedString;
    }
}

class GameManager
{
public:

    const EGameState& GetGameState() const { return currentState; }
    void SetGameState(const EGameState& state) { currentState = state; }

    void ProcessMenuCommands(const string& input);
    void ProcessMatchCheck();
    void ProcessPlayerCellChoice(const string& input);
    void ProcessPlayerDirectionChoice(const string& input);

    bool ProcessEndGame(const string& input);
private:
    EGameState currentState = EGameState::main_menu;
    unique_ptr<Board> board = nullptr;
};

