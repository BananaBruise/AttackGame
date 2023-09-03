#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include "IPrintable.h"
#include "external/nlohmann/json.hpp"

/*
    base class to all playable units such as player and monsters
*/

// global
constexpr unsigned INIT_LEVEL = 1; // all entity start at level 1
constexpr int ATTACK_POINT = 10;   // all units have base attack point of 10
constexpr double INIT_BASE_HEALTH = 1.0;
constexpr double INIT_BASE_MAGIC = 1.0;

class Entity : public IPrintable
{
    // friendships
    friend void to_json(nlohmann::json &, const Entity &);
    friend void from_json(const nlohmann::json &, Entity &);

public:
    // default constructor (all optional param)
    // NOTE: order of initialization is the order of decleration (not init list) --> best practice to init to order of declaration
    Entity() : type("none"), health_point(INIT_BASE_HEALTH), magic_point(INIT_BASE_MAGIC), description("no description") {}
    // constructor only by type
    Entity(std::string t, double hp = INIT_BASE_HEALTH, double mp = INIT_BASE_MAGIC, std::string desp = "no description") : type(t), health_point(hp), magic_point(mp), description(desp) {}

    // getter
    double get_hp() const { return health_point; }
    double get_mp() const { return magic_point; }
    int get_level() const { return level; }
    std::string get_desp() const { return description; }
    bool isAlive() const { return alive; }

    // setter
    Entity &update_hp(double hp)
    {
        health_point = hp;
        return *this;
    }

    Entity &update_mp(double mp)
    {
        magic_point = mp;
        return *this;
    }

    Entity &update_desp(const std::string &desp)
    {
        description = desp;
        return *this;
    }

    /*
        type must be subclass of entity
    */
    Entity &level_up(unsigned cnt = 1, bool update_health = true, bool revive = true)
    {
        level += cnt;
        // update health and magic
        if (update_health)
        {
            this->health_point = max_health();
            this->magic_point = max_magic();
        }
        // revive if dead
        if (!alive && revive)
            this->alive = true;

        return *this;
    }

    // toggle alive status; dose not change health
    Entity &kill()
    {
        alive = false;
        return *this;
    }
    Entity &revive()
    {
        alive = true;
        return *this;
    }

    /*
    attacker reduces health on attackee. the attack hits by chance between 0.0 and 1.0

    attackee must be subclass to entity
    */
    template <typename Attackee, typename std::enable_if<std::is_base_of<Entity, Attackee>::value>::type * = nullptr>
    void attack(Attackee &enemy, double chance_to_hit);

    // read-only print
    virtual inline std::ostream &print(std::ostream &is) const override;

    // virtuals: health & magic --> dummy values
    virtual double max_health() { return INIT_BASE_HEALTH; }
    virtual double max_magic() { return INIT_BASE_MAGIC; }

    // operator overloading
    bool operator==(const Entity &e) const
    {
        if (this->type == e.type && this->health_point == e.health_point && this->magic_point == e.magic_point && this->description == e.description && this->level == e.level && this->alive == e.alive)
            return true;

        return false;
    }

private:
    const std::string type; // type is read-only and init'd with class constructor
    // members:
    double health_point;
    double magic_point;
    std::string description;
    unsigned level = INIT_LEVEL;
    bool alive = true;
};

// out-of-class definition
std::ostream &Entity::print(std::ostream &is) const
{
    // set is flags
    is << std::fixed;           // force demical representation (https://cplusplus.com/reference/ios/fixed/)
    is << std::setprecision(1); // force demical places (https://cplusplus.com/reference/iomanip/setprecision/)
    // output
    is << "type: " << this->type
       << ", level: " << this->level
       << ", hp: " << this->health_point
       << ", mp: " << this->magic_point
       << ", description: " << this->description;

    return is;
}

template <typename Attackee, typename std::enable_if<std::is_base_of<Entity, Attackee>::value>::type * = nullptr>
void Entity::attack(Attackee &enemy, double chance_to_hit)
{
    // if chance is not between 0.0 and 1.0 --> no attack
    if (chance_to_hit < 0.0 || chance_to_hit > 1.0)
    {
        std::cout << "hit chance must be between [0.0,1.0)" << std::endl;
        return;
    }

    // get random number between 0.0-1.0
    // using old C-style -> apparently not the best idea: https://codingnest.com/generating-random-numbers-using-c-standard-library-the-problems/
    double roll_hit;
    // seed based on time
    std::srand(std::time(nullptr));
    // generate chance [0.0, 1.0)
    // https://stackoverflow.com/a/26853142
    // https://c-faq.com/lib/randrange.html
    // why? bc rand() gives [0, RAND_MAX], and RAND_MAX + 1.0 returns a double and will always be greater than rand()
    roll_hit = rand() / (RAND_MAX + 1.0);

    // change to hit
    if (chance_to_hit > roll_hit) // hit
    {
        int attack_point = this->level * ATTACK_POINT;
        if (attack_point >= enemy.health_point)
        { // kills enemy
            enemy.health_point = 0;
            enemy.kill();
            std::cout << this->type << " kills " << enemy.type << "!" << std::endl;
            return;
        }
        else
        { // reduce health
            enemy.health_point -= attack_point;
            std::cout << this->type << " hits " << enemy.type << " for " << attack_point << " health!" << std::endl;
            return;
        }
    }
    else // misses
    {
        std::cout << this->type << " attack misses!" << std::endl;
        return;
    }
}

// non-member definition
// declare inline to avoid "multiple definition error":
// https://github.com/nlohmann/json/issues/542
inline void to_json(nlohmann::json &j, const Entity &e)
{
    j = {
        {"type", e.type},
        {"health_point", e.health_point},
        {"magic_point", e.magic_point},
        {"description", e.description},
        {"entity_level", e.level},
        {"alive", e.alive}};
}

inline void from_json(const nlohmann::json &j, Entity &e)
{
    // omit "type" bc it's constant and should already exist after construction
    j.at("health_point").get_to(e.health_point);
    j.at("magic_point").get_to(e.magic_point);
    j.at("description").get_to(e.description);
    j.at("entity_level").get_to(e.level);
    j.at("alive").get_to(e.alive);
}

#endif