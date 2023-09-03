#include <iostream>
#include <string>
#include <filesystem>
#include "Player.h"
#include "Monster.h"
#include "GameStateManager.h"

using namespace std;

constexpr char SAVE_FILE_PATH[] = "./saves/save.json";
constexpr unsigned GAME_OVER_LEVEL = 10; // max level of game; after which user beats the game and game ends

ostream &print_about(ostream &os = cout)
{
    os << "User must defeat "
       << GAME_OVER_LEVEL
       << " monsters, each in increasing power and abilities.\n"
       << "\n"
       << "A game made by Han Tu."
       << endl;

    return os;
}

ostream &print_menu(ostream &os = cout)
{
    os << "press [S] to start or continue game\n"
       << "press [D] to show game stats\n"
       << "press [Q] to save player stats and quit game\n"
       << "press [R] to reset your progress\n"
       << "press [A] to learn about the game"
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
        if (GameStateManager *gsm = new GameStateManager(savefile))
            return gsm;
    }
    else // file doesn't exist or reset flag true; start new game
    {
        Player p;
        if (GameStateManager *gsm = new GameStateManager(p))
            return gsm;
    }

    return nullptr; // failure
}

/*
    resets game state and optionally prints a message

    Note: *&gsm is a "reference to pointer" so our pointer is preserved
*/
ostream &reset(GameStateManager *&gsm, const string &reset_message = "", ostream &os = cout, const string &savefile_path = SAVE_FILE_PATH)
{
    // reset gsm by de-allocate old object and assign a new one
    delete gsm;
    if (gsm = init_gsm(SAVE_FILE_PATH, true)) // reset success (ptr returned)
        os << reset_message;

    return os;
}

int main()
{
    char input;

    // welcome screen and get first input
    system("clear");
    cout << "Welcome to Attacker Game!" << endl;
    print_menu();
    cin >> input;
    system("clear");

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
            reset(gsm, "game reset.") << endl;
            break;

        // A: about game
        case 'A':
        case 'a':
            print_about(cout) << endl;
            break;

        // S: start game
        case 'S':
        case 's':
            cout << "let the battle begin!" << endl;
            Player &p = gsm->getPlayer();
            Monster m(gsm->getGameLevel()); // initiate monster from game level
            while (m.isAlive() && input != 'B' && input != 'b')
            {
                if (p.isAlive()) // player alive
                {
                    // print player and enemy stats
                    p.print(cout);
                    m.print(cout) << endl;
                    print_game_menu();
                    cin >> input;
                    system("clear");

                    switch (input)
                    {
                    // player attacks enemy, then enemy attacks player
                    case 'A':
                    case 'a':
                        // player attacks enemy
                        p.attack<Monster>(m);
                        if (!m.isAlive()) // user kills monster
                        {
                            // user beats game when game level is 10 and user is still alive (with monster dead)
                            if (gsm->getGameLevel() == GAME_OVER_LEVEL)
                            {
                                reset(gsm, "Congrats! You've beaten the game!") << endl;
                                continue;
                            }

                            cout << "you leveled up!" << endl;
                            // game and player level up
                            gsm->game_level_up();
                            p.level_up();

                            // monster levels up
                            m.level_up();

                            // player gain potions
                            cout << "you got a health potion!" << endl;
                            p.add_hp_pot();

                            continue;
                        }
                        // monster attacks player
                        m.attack<Player>(p); // we'll decide the fate of player (i.e. p.Alive()) on next iteration
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
                else // player dead
                {
                    reset(gsm, "you've been defeated!\nreturning to main menu") << endl;
                    break; // exit loop
                }
            }
            break;
        }
        print_menu();
        cin >> input;
        system("clear");
    }

    // quitting; save game state
    gsm->saveGame(SAVE_FILE_PATH, true);
    cout << "GoodBye!" << endl;

    // clean up resources
    if (gsm != nullptr)
        delete gsm;

    return 0;
}