#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "Entity.h"
#include "tester_stdout_helper.h"

using namespace std;
using namespace testing::internal;

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