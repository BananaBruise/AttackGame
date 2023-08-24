#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
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
    EXPECT_EQ(p.get_hp(), INIT_PLAYER_HEALTH) << "hp mismatch";
    EXPECT_EQ(p.get_mp(), INIT_PLAYER_MAGIC) << "mp mismatch";
    EXPECT_EQ(p.get_desp(), "player") << "description mismatch";
    EXPECT_EQ(p.get_level(), 1) << "level mismatch";
    EXPECT_EQ(p.get_hp_pot_cnt(), 0) << "hp pot mismatch";
    EXPECT_EQ(p.get_mp_pot_cnt(), 0) << "mp pot mismatch";

    // using print
    ostringstream result;
    result << fixed << setprecision(1); // see entity::print()
    result << "type: " << PLAYER_TYPE
           << ", level: " << p.get_level()
           << ", hp: " << INIT_PLAYER_HEALTH
           << ", mp: " << INIT_PLAYER_MAGIC
           << ", description: " << p.get_desp()
           << "\nHealth Potion: " << p.get_hp_pot_cnt()
           << "\nMagic Potion: " << p.get_mp_pot_cnt()
           << "\n";
    expect_stdout<Player>(cout, result.str(), p);
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
    ostringstream result;
    result << fixed << setprecision(1); // see entity::print()
    result << "type: " << PLAYER_TYPE
           << ", level: " << p.get_level()
           << ", hp: " << INIT_PLAYER_HEALTH
           << ", mp: " << INIT_PLAYER_MAGIC
           << ", description: " << p.get_desp()
           << "\nHealth Potion: " << p.get_hp_pot_cnt()
           << "\nMagic Potion: " << p.get_mp_pot_cnt()
           << "\n";
    expect_stdout<Player>(cout, result.str(), p);
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
    string expected = "no health potion!\n";
    expect_stdout<Player>(cout, expected, p, [](Player play, ostream &os)
                          { play.restore_health(); });
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
    string expected = "no magic potion!\n";
    expect_stdout<Player>(cout, expected, p, [](Player play, ostream &os)
                          { play.restore_magic(); });
    // check mp and potion count after "no restore"
    EXPECT_EQ(p.get_mp(), INIT_PLAYER_MAGIC / 2) << "magic mismatch";
    EXPECT_EQ(p.get_mp_pot_cnt(), 0) << "magic pot mismatch";
}

TEST(PlayerTest, AssertLevelUp)
{
    Player p;
    p.level_up();
    EXPECT_EQ(p.get_level(), 2) << "level mismatch";
    EXPECT_EQ(p.get_hp(), p.get_level() * INIT_PLAYER_HEALTH) << "health mismatch";
    EXPECT_EQ(p.get_mp(), p.get_level() * INIT_PLAYER_MAGIC) << "magic mismatch";
}

TEST(PlayerTest, AssertAttackOnMonster)
{
    Player p;
    Monster m;

    // capture bad attack input (console)
    {
        string expected = "hit chance must be between [0.0,1.0)\n";
        expect_stdout<Player>(cout, expected, p, [&m](Player play, ostream &os)
                              { play.Entity::attack<Monster>(m, 2.0); });
    }
    // on hit (1.0 chance; this always hits bc rand() is [0.0, 1.0))
    p.Entity::attack<Monster>(m, 1.0);
    // monster still alive and have health left
    EXPECT_NE(m.get_hp(), 0) << "Monster should have health left";
    EXPECT_TRUE(m.isAlive()) << "Monster should be alive";

    // on hit and kill (1.0 chance; this always hits bc rand() is [0.0, 1.0))
    m.update_hp(1.0);                                      // assuming attacks have >1
    EXPECT_TRUE(m.isAlive()) << "Monster should be alive"; // should be "alive" before being killed
    p.Entity::attack<Monster>(m, 1.0);
    EXPECT_EQ(m.get_hp(), 0) << "Monster should have no health left";
    EXPECT_FALSE(m.isAlive()) << "Monster should be dead";

    // on miss (0.0 chance hit; this always misses)
    m.update_hp(INIT_MONSTER_HEALTH); // reset monster health to level 1
    m.revive();                       // set to alive
    // check "no miss" message
    {
        string expected = "player attack misses!\n";
        expect_stdout<Player>(cout, expected, p, [&m](Player play, ostream &os)
                              { play.Entity::attack<Monster>(m, 0.0); });
    }
}