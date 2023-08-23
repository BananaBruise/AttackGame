#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
#include "IPrintable.h"

/*
    base class to all playable units such as player and monsters
*/
class Entity : public IPrintable
{
public:
    // default constructor (all optional param)
    // NOTE: order of initialization is the order of decleration (not init list) --> best practice to init to order of declaration
    Entity(double hp = 0.0, double mp = 0.0, std::string desp = "no description") : type("none"), health_point(hp), magic_point(mp), description(desp) {} 
    // constructor only by type
    Entity(std::string t, double hp = 0.0, double mp = 0.0, std::string desp = "no description") : type(t), health_point(hp), magic_point(mp), description(desp) {}

    // getter
    double get_hp() const { return health_point; }
    double get_mp() const { return magic_point; }
    std::string get_desp() const { return description; }

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

    // read-only print
    virtual inline std::ostream &print(std::ostream &is) const override;

private:
    // members:
    const std::string type; // type is read-only and init'd with class constructor
    double health_point;
    double magic_point;
    std::string description;
};

// out-of-class definition
std::ostream &Entity::print(std::ostream &is) const
{
    // set is flags
    is << std::fixed; // force demical representation (https://cplusplus.com/reference/ios/fixed/)
    is << std::setprecision(1); // force demical places (https://cplusplus.com/reference/iomanip/setprecision/) 
    // output
    is  << "type: " << this->type
        << ", hp: " << this->health_point
        << ", mp: " << this->magic_point
        << ", description: " << this->description;

    return is;
}

#endif