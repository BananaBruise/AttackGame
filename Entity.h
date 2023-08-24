#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include "IPrintable.h"

/*
    base class to all playable units such as player and monsters
*/

// global
constexpr unsigned INIT_LEVEL = 1; // all entity start at level 1
constexpr int ATTACK_POINT = 10; // all units have base attack point of 10
constexpr double INIT_BASE_HEALTH = 1.0;
constexpr double INIT_BASE_MAGIC = 1.0;

class Entity : public IPrintable
{
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
    bool isAlive() const {return alive;}

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
    Entity &level_up()
    {
        level++;
        // update health and magic
        this->health_point = max_health();
        this->magic_point = max_magic();

        return *this;
    }

    // toggle alive status; dose not change health
    Entity &kill() {alive = false; return *this;}
    Entity &revive() {alive = true; return *this;}

    /*
    attacker reduces health on attackee. the attack hits by chance between 0.0 and 1.0

    attackee must be subclass to entity
    */
    template<typename Attackee, typename std::enable_if<std::is_base_of<Entity, Attackee>::value>::type* = nullptr>
    void attack(Attackee &enemy, double chance_to_hit);

    // read-only print
    virtual inline std::ostream &print(std::ostream &is) const override;

    // virtuals: health & magic --> dummy values
    virtual double max_health() {return INIT_BASE_HEALTH;}
    virtual double max_magic() {return INIT_BASE_MAGIC;}

private:
    // members:
    const std::string type; // type is read-only and init'd with class constructor
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

 template<typename Attackee, typename std::enable_if<std::is_base_of<Entity, Attackee>::value>::type* = nullptr>
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
                if (attack_point >= enemy.health_point) {// kills enemy
                    enemy.health_point = 0;
                    enemy.kill();
                    return;
                }
                else { // reduce health
                    enemy.health_point -= attack_point;
                    return;
                }
            }
        else // misses
        {
            std::cout << this->type << " attack misses!" << std::endl;
            return;
        }
    }

#endif