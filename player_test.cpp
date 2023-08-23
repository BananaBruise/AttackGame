#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "Player.h"
#include "Monster.h"
#include "tester_stdout_helper.h"

using namespace std;
using namespace testing::internal;

// default constructor & getters
TEST(PlayerTest, AssertDefaultConstructor)
{
    Player p;
    // using getter
    EXPECT_EQ(p.get_hp(), 100.0) << "hp mismatch";
    EXPECT_EQ(p.get_mp(), 100.0) << "mp mismatch";
    EXPECT_EQ(p.get_desp(), "player") << "description mismatch";
    EXPECT_EQ(p.get_level(), 1) << "level mismatch";
    EXPECT_EQ(p.get_hp_pot_cnt(), 0) << "hp pot mismatch";
    EXPECT_EQ(p.get_mp_pot_cnt(), 0) << "mp pot mismatch";

    // using print
    expect_stdout<Player>(cout, "type: player, level: 1, hp: 100.0, mp: 100.0, description: player\nHealth Potion: 0\nMagic Potion: 0\n", p);
}

// setters: add potions
TEST(PlayerTest, AssertPotionSetters)
{
    Player p;

    // add hp pot
    p.add_hp_pot(); // 1
    EXPECT_EQ(p.get_hp_pot_cnt(), 1) << "hp pot cnt mismatch";
    p.add_hp_pot(); // 2

    // add magic pot
    p.add_mp_pot(); // 1
    EXPECT_EQ(p.get_mp_pot_cnt(), 1) << "mp pot cnt mismatch";
    p.add_mp_pot(); // 2
    p.add_mp_pot(); // 3

    // test with print
    expect_stdout<Player>(cout, "type: player, level: 1, hp: 100.0, mp: 100.0, description: player\nHealth Potion: 2\nMagic Potion: 3\n", p);
}

// setters: restore health with potion
TEST(PlayerTest, AssertHealthPotSetters)
{
    Player p; // default constructor initiate max health of INIT_HEALTH
              // starts with no potion

    // player starts with INIT_HEALTH
    EXPECT_EQ(p.get_hp(), INIT_PLAYER_HEALTH);

    // half health & restore 20
    p.add_hp_pot(); // give a potion
    p.update_hp(INIT_PLAYER_HEALTH / 2);
    p.restore_health(20.0);
    EXPECT_EQ(p.get_hp(), INIT_PLAYER_HEALTH / 2 + 20.0) << "health mismatch";
    EXPECT_EQ(p.get_hp_pot_cnt(), 0) << "health pot mismatch";

    // restore to full
    p.add_hp_pot(); // give a potion
    p.update_hp(INIT_PLAYER_HEALTH / 2);
    p.restore_health();
    EXPECT_EQ(p.get_hp(), INIT_PLAYER_HEALTH) << "health mismatch";
    EXPECT_EQ(p.get_hp_pot_cnt(), 0) << "health pot mismatch";

    // over-restore health result in full health
    p.add_hp_pot();
    p.update_hp(INIT_PLAYER_HEALTH / 2);
    p.restore_health(INIT_PLAYER_HEALTH * 2); // over-restore health
    EXPECT_EQ(p.get_hp(), INIT_PLAYER_HEALTH) << "health mismatch";
    EXPECT_EQ(p.get_hp_pot_cnt(), 0) << "health pot mismatch";

    // no restore when no health potion
    p.update_hp(INIT_PLAYER_HEALTH / 2);
    // check "no potion" message
    testing::internal::CaptureStdout();
    p.restore_health();
    std::cout.flush();
    std::string print_output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(print_output, "no health potion!\n") << "\"no potion\" message mismatch";
    // check hp and potion count after "no restore"
    EXPECT_EQ(p.get_hp(), INIT_PLAYER_HEALTH / 2) << "health mismatch";
    EXPECT_EQ(p.get_hp_pot_cnt(), 0) << "health pot mismatch";
}

// setters: restore magic with potion
TEST(PlayerTest, AssertMagicPotSetters)
{
    Player p; // default constructor initiate max magic of INIT_MAGIC
              // starts with no potion

    // player starts with INIT_MAGIC
    EXPECT_EQ(p.get_mp(), INIT_PLAYER_MAGIC);

    // half magic & restore 20
    p.add_mp_pot(); // give a potion
    p.update_mp(INIT_PLAYER_MAGIC / 2);
    p.restore_magic(20.0);
    EXPECT_EQ(p.get_mp(), INIT_PLAYER_MAGIC / 2 + 20.0) << "magic mismatch";
    EXPECT_EQ(p.get_mp_pot_cnt(), 0) << "magic pot mismatch";

    // restore to full
    p.add_mp_pot(); // give a potion
    p.update_mp(INIT_PLAYER_MAGIC / 2);
    p.restore_magic();
    EXPECT_EQ(p.get_mp(), INIT_PLAYER_HEALTH) << "magic mismatch";
    EXPECT_EQ(p.get_mp_pot_cnt(), 0) << "magic pot mismatch";

    // over-restore magic result in full magic
    p.add_mp_pot();
    p.update_mp(INIT_PLAYER_MAGIC / 2);
    p.restore_magic(INIT_PLAYER_MAGIC * 2); // over-restore magic
    EXPECT_EQ(p.get_mp(), INIT_PLAYER_MAGIC) << "magic mismatch";
    EXPECT_EQ(p.get_mp_pot_cnt(), 0) << "magic pot mismatch";

    // no restore when no magic potion
    p.update_mp(INIT_PLAYER_MAGIC / 2);
    // check "no potion" message
    testing::internal::CaptureStdout();
    p.restore_magic();
    std::cout.flush();
    std::string print_output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(print_output, "no magic potion!\n") << "\"no potion\" message mismatch";
    // check mp and potion count after "no restore"
    EXPECT_EQ(p.get_mp(), INIT_PLAYER_MAGIC / 2) << "magic mismatch";
    EXPECT_EQ(p.get_mp_pot_cnt(), 0) << "magic pot mismatch";
}

TEST(PlayerTest, AssertLevelUp)
{
    Player p;
    p.level_up();
    EXPECT_EQ(p.get_level(), 2) << "level mismatch";
    EXPECT_EQ(p.get_hp(), 200.0) << "health mismatch";
    EXPECT_EQ(p.get_mp(), 200.0) << "magic mismatch";
}

TEST(PlayerTest, AssertAttackOnMonster)
{
    Player p;
    Monster m;

    // capture bad attack input (console)
    {
        CaptureStdout();
        p.Entity::attack<Monster>(m, 2.0); // bad call
        cout.flush();
        string print_output = GetCapturedStdout();
        EXPECT_EQ(print_output, "hit change must be between [0.0,1.0)\n") << "stdout mismatch";
    }

    // on hit (0.999999 chance; technically this might miss)
    p.Entity::attack<Monster>(m, 0.999999);
    // monster still alive and have health left
    EXPECT_NE(m.get_hp(), 0) << "Monster should have health left";
    EXPECT_TRUE(m.isAlive()) << "Monster should be alive";

    // on hit and kill (0.999999; technially this might miss)
    m.update_hp(0.0); // assuming attacks have >1
    p.Entity::attack<Monster>(m, 0.999999);
    EXPECT_EQ(m.get_hp(), 0) << "Monster should have no health left";
    EXPECT_FALSE(m.isAlive()) << "Monster should be dead";

    // on miss (0.0 chance hit; technically this might hit)
    m.update_hp(INIT_MONSTER_HEALTH); // reset monster health to level 1
    m.revive(); // set to alive
    // check "no miss" message
    {
        CaptureStdout();
        p.Entity::attack<Monster>(m, 0.0);
        cout.flush();
        string print_output = GetCapturedStdout();
        EXPECT_EQ(print_output, "player attack misses!\n") << "\"miss\" message mismatch";
        EXPECT_NE(m.get_hp(), 0) << "Monster should have health left";
        EXPECT_TRUE(m.isAlive()) << "Monster should be alive";
    }
}