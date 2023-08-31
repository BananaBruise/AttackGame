#ifndef GAME_STATE_MAN_H
#define GAME_STATE_MAN_H

/*
    Game State Manager class

    - save and load Player stats (aka save state)
    - status on whether user is loaded
    - game level (so that user and monster level isn't tied to the progress of the game)
*/

#include "Player.h"
#include "external/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>

class GameStateManager
{
public:
    // disable default constructor
    GameStateManager() = delete;
    GameStateManager(Player play, unsigned gamelvl = 1) : player(play), loaded(true), level(gamelvl) {}
    // open saved player state
    // GameStateManager()

    // saves player state to json file; returns a reference to save file
    std::ofstream &savePlayer(std::ofstream &savefile, bool verbose = false) {
        if(verbose)
            std::cout << "saving";
        nlohmann::json output;
        // output player to json
        // output << player;
        // json to file

        return savefile;
    }

    // getters
    bool isLoaded() { return loaded; }
    unsigned getGameLevel() { return level; }
    // return player object and print player stats
    const Player &getPlayer(bool verbose = false) const
    {
        if (verbose)
            player.print(std::cout);
        return const_cast<const Player &>(player);
    }

private:
    // members:
    Player player;
    bool loaded;
    unsigned level = 1;
};

#endif