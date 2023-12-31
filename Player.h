#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <iostream>
#include <string>
#include "Entity.h"
#include "IPrintable.h"
#include "external/nlohmann/json.hpp"

/*
    player class

    In addition to Entity, Player has:
    - own health scalling
    - inventory of potions (recuperate hp and mp)
    - abilities that consume mp
*/

// globals
constexpr double INIT_PLAYER_HEALTH = 100.0; // initial health
constexpr double INIT_PLAYER_MAGIC = 100.0;  // initial magic
constexpr double PLAYER_HIT_CHANCE = 0.8;    // 80% chance to hit
constexpr char PLAYER_TYPE[] = "player";

class Player : public Entity
{
public:
    // friendship
    friend class GameStateManager;
    friend void to_json(nlohmann::json &, const Player &);
    friend void from_json(const nlohmann::json &, Player &);

    // default constructor
    Player() : Entity(PLAYER_TYPE, INIT_PLAYER_HEALTH, INIT_PLAYER_MAGIC, "player") {}
    // special constructor for GameStateManager 
    Player(bool alive, unsigned level, double hp, double mp, int hpot, int mpot, std::string desc) : Entity(PLAYER_TYPE, hp, mp, desc)
    {
        // initialize alive
        if (alive)
            revive();
        else
            kill();
        // initialize level
        level_up(level - INIT_LEVEL, false);
        // initialize pots
        this->potions["Health Potion"] = hpot;
        this->potions["Magic Potion"] = mpot;
    }

    // getters
    unsigned int get_hp_pot_cnt() const { return potions.find("Health Potion")->second; } // [] operator is a non-const member function; use find instead
                                                                                          // https://stackoverflow.com/a/10699752
    unsigned int get_mp_pot_cnt() const { return potions.find("Magic Potion")->second; }

    // add hp pot
    Player &add_hp_pot()
    {
        potions["Health Potion"]++;
        return *this;
    }
    // add mp pot
    Player &add_mp_pot()
    {
        potions["Magic Potion"]++;
        return *this;
    }

    // restore health
    Player &restore_health(double hp)
    {
        // does not update hp if no potion
        if (potions["Health Potion"] == 0)
        {
            std::cout << "no health potion!" << std::endl;
            return *this;
        }
        // restore full health if input is greater than max
        if (hp >= max_health())
            update_hp(max_health()); // checks for potion count
        else                         // otherwise, restore the amount given
            update_hp(get_hp() + hp);

        potions["Health Potion"]--; // removes potion
        return *this;
    }
    // restore full health
    Player &restore_health()
    {
        return restore_health(max_health());
    }

    // retore magic point
    Player &restore_magic(double mp)
    {
        // does not update mp if no potion
        if (potions["Magic Potion"] == 0)
        {
            std::cout << "no magic potion!" << std::endl;
            return *this;
        }
        // restore full magic if input is greater than max
        if (mp >= max_magic())
            update_mp(max_magic()); // checks for potion count
        else                        // otherwise, restore the amount given
            update_mp(get_mp() + mp);

        potions["Magic Potion"]--; // removes potion
        return *this;
    }
    // restore max magic
    Player &restore_magic()
    {
        return restore_magic(max_magic());
    }

    // read-only print
    inline std::ostream &print(std::ostream &is) const override;

    // overload entity::attack with default player hit chance
    template <typename Attackee, typename std::enable_if<std::is_base_of<Entity, Attackee>::value>::type * = nullptr>
    void attack(Attackee &enemy) { Entity::attack(enemy, PLAYER_HIT_CHANCE); }

    // operator overloading
    bool operator==(const Player &p) const
    {
        if (Entity::operator==(p) && this->potions == p.potions)
            return true;

        return false;
    }

private:
    // members
    std::map<std::string, unsigned int> potions = {// potion name, potion count
                                                   {"Health Potion", 0},
                                                   {"Magic Potion", 0}};
    // print list of potions
    std::ostream &print_potions(std::ostream &os) const
    {
        for (const auto &kv : potions) // https://stackoverflow.com/a/6963910
            os << kv.first << ": " << kv.second << '\n';

        return os;
    }

    // max health
    double max_health() override { return get_level() * INIT_PLAYER_HEALTH; }
    // max magic
    double max_magic() override { return get_level() * INIT_PLAYER_MAGIC; }
};

// out-of-class definition
std::ostream &Player::print(std::ostream &os) const
{
    Entity::print(os) << "\n";
    print_potions(os);

    return os;
}

// non-member function
inline void to_json(nlohmann::json &j, const Player &p)
{
    // inherits Entity to_json
    to_json(j, static_cast<const Entity &>(p));
    // update fields unique to Player
    // usage: https://github.com/nlohmann/json/issues/2199
    j.update({{"potions", p.potions}});
}

inline void from_json(const nlohmann::json &j, Player &p)
{
    // inherits Entity from_json
    from_json(j, static_cast<Entity &>(p)); // cannot cast to Entity bc of "temp" conversion: https://stackoverflow.com/questions/18565167/non-const-lvalue-references
    // update fields unique to Player
    j.at("potions").get_to(p.potions);
}

#endif