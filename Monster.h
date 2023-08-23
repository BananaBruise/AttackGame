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
constexpr unsigned INIT_MONSTER_HEALTH = 80; // initial health
constexpr unsigned INIT_MONSTER_MAGIC = 80; // initlal magic
constexpr double MONSTER_HIT_CHANCE = 0.5; // 50% chance to hit

class Monster: public Entity {
    public:
    Monster() : Entity("monster", INIT_MONSTER_HEALTH, INIT_MONSTER_MAGIC, "monster") {}

    // overload entity::attack with default monster hit chance
    template<typename Attackee, typename std::enable_if<std::is_base_of<Entity, Attackee>::value>::type* = nullptr>
    void attack(Attackee &enemy) { Entity::attack(enemy, MONSTER_HIT_CHANCE);}

    private:
    // max health
    double max_health() override { return get_level() == 1? INIT_MONSTER_HEALTH : std::pow(get_level(), 1.2) * 100.0;}
    // max magic
    double max_magic() override { return get_level() == 1? INIT_MONSTER_MAGIC : std::pow(get_level(), 1.2) * 100.0;}
};

#endif

//TODO: test level up, constructor, getters, max_health, max_magic