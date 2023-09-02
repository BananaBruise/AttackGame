#include <iostream>
#include <string>
#include <filesystem>
#include "Player.h"
#include "Monster.h"
#include "GameStateManager.h"

using namespace std;

constexpr char SAVE_FILE_PATH[] = "./saves/save.json";

void print_menu()
{
    cout << "press [S] to start or continue game\n"
         << "press [D] to show game stats\n"
         << "press [Q] anytime to quit game"
         << endl;
}

GameStateManager *init_gsm(const string &savefile)
{
    if (filesystem::exists(savefile)) // load save file
    {
        GameStateManager *gsm = new GameStateManager(savefile);
        return gsm;
    }
    else // file doesn't exist; start new game
    {
        Player p;
        GameStateManager *gsm = new GameStateManager(p);
        return gsm;
    }
}

int main()
{
    char input;

    // welcome screen and get first input
    cout << "Welcome to Attacker Game!" << endl;
    print_menu();
    cin >> input;

    // initialize game states
    GameStateManager *gsm = init_gsm(SAVE_FILE_PATH);

    while (input != 'Q' && input != 'q')
    {
        // parse input
        switch (input)
        {
            // D: game stats
            case 'D':
            case 'd':
                gsm->print_game_state(cout) << endl;
                break;

            // S: start game
            case 'S':
            case 's':
                cout << "start game!" << endl;
                gsm->getPlayer().add_hp_pot().add_mp_pot(); // TODO test save file
                break;
        }
        print_menu();
        cin >> input;
    }

    // quitting; save game state
    gsm->saveGame(SAVE_FILE_PATH, true);
    cout << "GoodBye!" << endl;

    // clean up resources
    if(gsm!= nullptr)
        delete gsm;


    return 0;
}