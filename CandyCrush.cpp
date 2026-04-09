#include <iostream>
#include <map>
#include <string>

#include "Board.h"
#include "GameManager.h"

using namespace std;
using namespace CandyCrushStrings;

int main()
{
    PrintGameStart();
    
    string input;
    unique_ptr<Board> board = nullptr;
    const unique_ptr<GameManager> gameManager = make_unique<GameManager>();
    do
    {
        cin >> input;
        gameManager->ProcessEndGame(input);
        switch (gameManager->GetGameState())
        {
            case EGameState::main_menu:
                gameManager->ProcessMenuCommands(input);
                break;
            case EGameState::checking_matches:
                gameManager->ProcessMatchCheck();
                break;
            case EGameState::awaiting_input_cell:
                gameManager->ProcessPlayerCellChoice(input);
                break;
            case EGameState::awaiting_input_direction:
                gameManager->ProcessPlayerDirectionChoice(input);
            break;
            case EGameState::game_end:
                break;
        }
    } while (gameManager->GetGameState() != EGameState::game_end);
    return 0;
}
/// TODO
/// gravityyyyyyyyy
/// score system - give each candy a score value and x by how many candies matched
/// Figure out the colors for power shell
