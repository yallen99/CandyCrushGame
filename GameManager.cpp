#include "GameManager.h"

#include "Board.h"

using namespace CandyCrushStrings;

void GameManager::ProcessMenuCommands(const string& input)
{
    const string& sanitizedInput = GetSanitisedInput(input);
    if (sanitizedInput == commands[EInfoBlock::help])
    {
        PrintHelpBlock();
    }
    else if (sanitizedInput == commands[EInfoBlock::start])
    {
        board = make_unique<Board>(MEDIUM_BOARD);
        currentState = EGameState::checking_matches;
        ProcessMatchCheck();
    }
}

void GameManager::ProcessMatchCheck()
{
    if (currentState == EGameState::checking_matches)
    {
        PrintGameEvent("CHECKING MATCH");

        while (board->CheckForMatch(board->GetSlots(), "x"))
        {
            PrintGameEvent("NEW BOARD");
        }

        currentState = EGameState::awaiting_input_cell;
        PrintGameEvent("AWAITING CHOICE (row - column)");
    }
}

void GameManager::ProcessPlayerCellChoice(const string& input)
{
    const string& sanitizedInput = GetSanitisedInput(input);
    if (sanitizedInput == commands[EInfoBlock::help])
    {
        PrintHelpBlock();
        return;
    }
    int inputInt = atoi(sanitizedInput.c_str());
    if (inputInt > 0 && inputInt < 100)
    {
        const int cellCoords = stoi(sanitizedInput);
        board->PickCell(cellCoords);
        PrintGameEvent("AWAITING CHOICE (up - down - left - right)");
        currentState = EGameState::awaiting_input_direction;
    }
    else
    {
        PrintGameEvent("** Invalid row - column input! **");
        PrintGameEvent("AWAITING CHOICE (row - column)");
    }
}

void GameManager::ProcessPlayerDirectionChoice(const string& input)
{
    const string& sanitizedInput = GetSanitisedInput(input);
    if (sanitizedInput == commands[EInfoBlock::help])
    {
        PrintHelpBlock();
        return;
    }
    if (sanitizedInput == directionStrings[EDirection::UP])
    {
        board->HighlightDirection(EDirection::UP);
    }
    else if (sanitizedInput == directionStrings[EDirection::DOWN])
    {
        board->HighlightDirection(EDirection::DOWN);
    }
    else if (sanitizedInput == directionStrings[EDirection::LEFT])
    {
        board->HighlightDirection(EDirection::LEFT);
    }
    else if (sanitizedInput == directionStrings[EDirection::RIGHT])
    {
        board->HighlightDirection(EDirection::RIGHT);
    }
    else
    {
        PrintGameEvent("** Invalid direction input! **");
        PrintGameEvent("AWAITING CHOICE (up - down - left - right)");
    }
    currentState = EGameState::checking_matches;
    ProcessMatchCheck();
}

bool GameManager::ProcessEndGame(const string& input)
{
    const string& sanitizedInput = GetSanitisedInput(input);
    return sanitizedInput == commands[EInfoBlock::end];
    currentState = EGameState::game_end;
}