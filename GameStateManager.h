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
    // friendships:
    friend struct nlohmann::adl_serializer<GameStateManager>;

public:
    // disable default constructor
    GameStateManager() = delete;
    GameStateManager(Player &play, unsigned gamelvl = 1, bool isloaded = true) : player(play), loaded(isloaded), level(gamelvl) {}
    // constructor via save file
    GameStateManager(const std::string &);
    // copy constructor
    GameStateManager(const GameStateManager &) = default;

    // saves player state to json file; returns a reference to save file
    void saveGame(const std::string &savefile, bool verbose = false);

    // getters
    bool isLoaded() { return loaded; }
    unsigned getGameLevel() { return level; }

    // setter
    GameStateManager &game_level_up()
    {
        level++;
        return *this;
    }
    GameStateManager &unload()
    {
        loaded = false;
        return *this;
    }

    // return player object and print player stats
    Player &getPlayer(bool verbose = false)
    {
        if (verbose)
            player.print(std::cout);
        return player;
    }

    // print game states
    std::ostream &print_game_state(std::ostream &os)
    {
        os << std::boolalpha; // print bool in string instead of int representation
        player.print(os) << "\n";
        os << "loaded?: " << loaded
           << ", game level: " << level;
        return os;
    }

    // operator overloading
    bool operator==(const GameStateManager &gsm) const
    {
        if (this->player == gsm.player && this->level == gsm.level && this->loaded == gsm.loaded)
            return true;

        return false;
    }

private:
    // members:
    Player player;
    bool loaded;
    unsigned level = 1;
};

// by default, get<GameStateManager>() rely on default constructor
// reference:
// https://github.com/nlohmann/json#how-can-i-use-get-for-non-default-constructiblenon-copyable-types
// need to define struct when default constructor is disabled:
// https://json.nlohmann.me/api/adl_serializer/from_json/#examples
namespace nlohmann
{
    template <>
    struct adl_serializer<::GameStateManager>
    {
        // note: the return type is no longer 'void', and the method only takes
        // one argument
        static ::GameStateManager from_json(const nlohmann::json &j)
        {
            // uses this constructor
            //  GameStateManager(Player &play, unsigned gamelvl, bool isloaded)
            ::Player p(j.at("alive"), j.at("entity_level"), j.at("health_point"), j.at("magic_point"), j["potions"].at("Health Potion"), j["potions"].at("Magic Potion"), j.at("description"));

            return {
                p,
                j["gameStates"].at("level"),
                j["gameStates"].at("loaded")};
        }

        static void to_json(nlohmann::json &j, ::GameStateManager gsm)
        {
            // Player from_json (from global scope)
            ::to_json(j, gsm.player);
            // update fields unique to gsm
            j["gameStates"].update({{"level", gsm.level},
                                    {"loaded", gsm.loaded}});
        }
    };
}

// out-of-class members

// uses copy constructor bc from_json (result of parse) returns a GameStateManager object per adl_serializer
// this one uses from_json, which has to be defined after adl_serializer
GameStateManager::GameStateManager(const std::string &savefile) : GameStateManager(static_cast<const GameStateManager &>(nlohmann::json::parse(std::ifstream(savefile)))) {}

// this one ueses to_json, which has to be defined after adl_serializer
void GameStateManager::saveGame(const std::string &savefile, bool verbose)
{
    std::ofstream of(savefile);
    if (verbose)
        std::cout << "saving" << std::endl;
    // output player to json
    nlohmann::json output = *this;
    // json to file
    // https://github.com/nlohmann/json/issues/690
    of << output;

    if (verbose && !std::cout.fail())
        std::cout << "saved!" << std::endl;
}

#endif