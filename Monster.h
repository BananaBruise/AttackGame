#ifndef MONSTER_H
#define MONSTER_H

#include <cmath>
#include "Entity.h"

/*
    Monster class

    In addition to Entity, Player has:
    - own health scaling (no health restore though)
    - currently does not use magic

*/

// globals
constexpr double INIT_MONSTER_HEALTH = 80.0; // initial health
constexpr double INIT_MONSTER_MAGIC = 80.0; // initlal magic
constexpr double MONSTER_HIT_CHANCE = 0.5; // 50% chance to hit
constexpr char MONSTER_TYPE [] = "monster";

class Monster: public Entity {
    public:
    Monster() : Entity(MONSTER_TYPE, INIT_MONSTER_HEALTH, INIT_MONSTER_MAGIC, "monster") {}
    // cast Monster at specific level
    Monster(unsigned level) : Monster() {this->level_up(level-INIT_LEVEL);} // number of level to add is (level-INIT_LEVEL)

    // overload entity::attack with default monster hit chance
    template<typename Attackee, typename std::enable_if<std::is_base_of<Entity, Attackee>::value>::type* = nullptr>
    void attack(Attackee &enemy) { Entity::attack(enemy, MONSTER_HIT_CHANCE);}

    private:
    // max health
    double max_health() override { return std::pow(get_level(), 1.12) * INIT_MONSTER_HEALTH;}
    // max magic
    double max_magic() override { return std::pow(get_level(), 1.12) * INIT_MONSTER_MAGIC;}
};

#endif