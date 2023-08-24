#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include "Player.h"
#include "Monster.h"
#include "tester_stdout_helper.h"

using namespace std;
using namespace testing::internal;

// default constructor and getters
TEST(MonsterTest, AssertDefaultConstrutor)
{
    Monster m;

    // using getter
    EXPECT_EQ(m.get_hp(), INIT_MONSTER_HEALTH) << "hp mismatch";
    EXPECT_EQ(m.get_mp(), INIT_MONSTER_MAGIC) << "mp mismatch";
    EXPECT_EQ(m.get_desp(), "monster") << "description mismatch";
    EXPECT_EQ(m.get_level(), 1) << "level mismatch";

    // using print
    ostringstream result;
    result << fixed << setprecision(1); // see entity::print()
    result << "type: " << MONSTER_TYPE
           << ", level: " << m.get_level()
           << ", hp: " << INIT_MONSTER_HEALTH
           << ", mp: " << INIT_MONSTER_MAGIC
           << ", description: " << m.get_desp();
    expect_stdout<Monster>(cout, result.str(), m);
}

TEST(MonsterTest, AssertLevelUp)
{
    Monster m;
    m.level_up();
    EXPECT_EQ(m.get_level(), 2) << "level mismatch";
    EXPECT_EQ(m.get_hp(), pow(m.get_level(), 1.12) * INIT_MONSTER_HEALTH) << "health mismatch";
    EXPECT_EQ(m.get_mp(), pow(m.get_level(), 1.12) * INIT_MONSTER_MAGIC) << "magic mismatch";
}

TEST(MonsterTest, AssertAttackOnPlayer)
{
    Monster m;
    Player p;

    // capture bad attack input (console)
    // not tested here bc same logic as player class attack; see TEST(PlayTest, AssertAttackOnMonster)

    // on hit (1.0 chance; this always hits bc rand() is [0.0, 1.0))
    m.Entity::attack<Player>(p, 1.0);
    // player still alive and have health left
    EXPECT_NE(p.get_hp(), 0) << "Player should have health left";
    EXPECT_TRUE(p.isAlive()) << "Player should be alive";

    // on hit and kill (1.0 chance; this always hits bc rand() is [0.0,1.0))
    p.update_hp(1.0);                                     // assuming attacks have >1
    EXPECT_TRUE(p.isAlive()) << "Player should be alive"; // should be "alive" before being killed
    m.Entity::attack<Player>(p, 1.0);
    EXPECT_EQ(p.get_hp(), 0) << "Player should have no health left";
    EXPECT_FALSE(p.isAlive()) << "Player should be dead";

    // on miss (0.0 chance hit; this always misses)
    p.update_hp(INIT_PLAYER_HEALTH); // reset player health to level 1
    p.revive();                      // set to alive
    // check "no miss" message
    string expected = "monster attack misses!\n";
    expect_stdout<Monster>(cout, expected, m, [&](Monster mon, ostream &os)
                           { mon.Entity::attack<Player>(p, 0.0); });
}