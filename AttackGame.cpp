#include <iostream>
#include <string>
#include <filesystem>
#include "Player.h"
#include "Monster.h"
#include "GameStateManager.h"

using namespace std;

constexpr char SAVE_FILE_PATH[] = "./saves/save.json";

ostream &print_menu(ostream &os = cout)
{
    os << "press [S] to start or continue game\n"
         << "press [D] to show game stats\n"
         << "press [Q] to save player stats and quit game\n"
         << "press [R] to reset your progress"
         << endl;

    return os;
}

ostream &print_game_menu(ostream &os = cout)
{
    os << "press [A] to attack enemy\n"
    << "press [H] to consume health potion to full health\n"
    << "press [B] to leave game session" 
    << endl;

    return os;
}

GameStateManager *init_gsm(const string &savefile, bool reset = false)
{
    if (filesystem::exists(savefile) && !reset) // load save file if file exists and is not a reset
    {
        GameStateManager *gsm = new GameStateManager(savefile);
        return gsm;
    }
    else // file doesn't exist or reset flag true; start new game
    {
        Player p;
        GameStateManager *gsm = new GameStateManager(p);
        return gsm;
    }
}

/*
    resets game state and optionally prints a message

    Note: *&gsm is a "reference to pointer" so our pointer is preserved
*/
ostream &reset(GameStateManager *&gsm, const string &reset_message = "", ostream &os = cout, const string &savefile_path = SAVE_FILE_PATH)
{
    os << reset_message;
    // reset gsm by de-allocate old object and assign a new one
    delete gsm;
    gsm = init_gsm(SAVE_FILE_PATH, true);

    return os;
}

int main()
{
    char input;

    // welcome screen and get first input
    cout << "Welcome to Attacker Game!" << endl;
    print_menu();
    cin >> input;
    cout << endl;

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

        // R: reset game
        case 'R':
        case 'r':
            reset(gsm) << endl;
            break;

        // S: start game
        case 'S':
        case 's':
            cout << "let the battle begin!" << endl;
            Player &p = gsm->getPlayer();
            Monster m;
            while (p.isAlive() && m.isAlive() && input != 'B' && input != 'b')
            {
                // print player and enemy stats
                p.print(cout); 
                m.print(cout) << endl;
                print_game_menu();
                cin >> input;
                cout << endl;

                switch (input)
                {
                // player attacks enemy, then enemy attacks player
                case 'A':
                case 'a':
                    // player attacks enemy
                    p.attack<Monster>(m);
                    if (!m.isAlive()) // kills monster, return to home screen; player is reset
                    {
                        // TODO: level game, monster, player up; instead of resetting everytime
                        reset(gsm, "you defeated the enemy!") << endl;
                        
                        // player gain potions
                        cout << "you got a health potion!" << endl;
                        p.add_hp_pot();
                        continue;
                    }
                    // monster attacks player
                    m.attack<Player>(p);
                    if (!p.isAlive()) // kills player
                    {
                        reset(gsm, "you've been defeated!") << endl;
                        continue;
                    }

                    cout << endl;
                    break;

                // H: refill health
                case 'H':
                case 'h':
                    p.restore_health();
                    break;

                // B: leave game session
                case 'B':
                case 'b':
                    cout << "you escaped!" << endl;
                    continue;
                }
            }
            break;
        }
        print_menu();
        cin >> input;
    }

    // quitting; save game state
    gsm->saveGame(SAVE_FILE_PATH, true);
    cout << "GoodBye!" << endl;

    // clean up resources
    if (gsm != nullptr)
        delete gsm;

    return 0;
}