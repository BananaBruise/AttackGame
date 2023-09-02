#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "Entity.h"
#include "tester_stdout_helper.h"
#include "external/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// default constructor & getters
TEST(EntityTest, AssertDefaultConstructor)
{
    Entity a;
    // using getter
    EXPECT_EQ(a.get_hp(), 1.0) << "hp mismatch";
    EXPECT_EQ(a.get_mp(), 1.0) << "mp mismatch";
    EXPECT_EQ(a.get_desp(), "no description") << "description mismatch";
    EXPECT_EQ(a.get_level(), 1) << "level mismatch";
    // using print
    expect_stdout<Entity>(cout, "type: none, level: 1, hp: 1.0, mp: 1.0, description: no description", a);
}

// constructor by type
TEST(EntityTest, AssertTypeConstructor)
{
    Entity a("test type");

     // using print
    expect_stdout<Entity>(cout, "type: test type, level: 1, hp: 1.0, mp: 1.0, description: no description", a);
}

// test setters
TEST(EntityTest, AssertSetters)
{
    Entity a;

    // sets
    a.update_hp(10.0);
    a.update_mp(20.0);
    a.update_desp("description updated");
    a.level_up();

    // test with print
    expect_stdout<Entity>(cout,  "type: none, level: 2, hp: 1.0, mp: 1.0, description: description updated", a);
}

// json conversion ability
TEST(EntityTest, AssertJsonConversion)
{
    Entity a;
    json j;

    // update members for testing
    a.level_up(); // level 2
    a.update_hp(2);
    a.update_mp(3);
    a.update_desp("json test");
    a.kill(); // alive = false

    // convert Entity to json (to_json)
    j = a;
    // convert json to Entity (from_json)
    auto a2 = j.get<Entity>();

    // compare
    EXPECT_EQ(a,a2);

}