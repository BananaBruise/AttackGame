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
    EXPECT_EQ(a.get_hp(), 0.0) << "hp mismatch";
    EXPECT_EQ(a.get_mp(), 0.0) << "mp mismatch";
    EXPECT_EQ(a.get_desp(), "no description") << "description mismatch";

    // using print
    expect_stdout<Entity>(cout, "type: none, hp: 0.0, mp: 0.0, description: no description", a);
}

// constructor by type
TEST(EntityTest, AssertTypeConstructor)
{
    Entity a("test type");

     // using print
    expect_stdout<Entity>(cout, "type: test type, hp: 0.0, mp: 0.0, description: no description", a);
}

// test setters
TEST(EntityTest, AssertSetters)
{
    Entity a;

    // sets
    a.update_hp(10.0);
    a.update_mp(20.0);
    a.update_desp("description updated");

    // test with print
    expect_stdout<Entity>(cout,  "type: none, hp: 10.0, mp: 20.0, description: description updated", a);
}