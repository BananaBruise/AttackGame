#include <gtest/gtest.h>
#include <fstream>
#include "external/nlohmann/json.hpp"
#include "Player.h"
#include "GameStateManager.h"

using namespace std;
using json = nlohmann::json;

// globals
constexpr char TEST_SAVE [] = "./saves/test_save.json";




// test player object-based constructor
TEST(GameStateManTest, AssertPlayerConstructor)
{
    Player p;
    GameStateManager gsm(p);

    EXPECT_TRUE(gsm.isLoaded()) << "player not loaded";
    EXPECT_EQ(gsm.getGameLevel(), 1) << "game level not 1";
    EXPECT_EQ(gsm.getPlayer(), p) << "incorrect player reference did not returned";
}

// test saving file and reading (using filename constructor)
TEST(GameStateManTest, AssertPlayerSaveANDRead)
{
    Player p;
    GameStateManager gsm1(p);

    // update&save gsm1
    gsm1.game_level_up();
    gsm1.unload();
    gsm1.saveGame(TEST_SAVE);

    // load save into new object
    GameStateManager gsm2 (TEST_SAVE);

    EXPECT_EQ(gsm1,gsm2) << "save & load game state mismatach";
}

// test setters
TEST(GameStateManTest, AssertSetters)
{
    Player p;
    GameStateManager gsm(p);

    // setters
    gsm.game_level_up();
    gsm.unload();

    // tests
    EXPECT_FALSE(gsm.isLoaded()) << "player should not loaded";
    EXPECT_EQ(gsm.getGameLevel(), 2) << "game level not 2";
    EXPECT_EQ(gsm.getPlayer(), p) << "incorrect player reference did not returned";
}

// test json conversion
TEST(GameStateManTest, AssertJsonConversion)
{
    Player p;
    GameStateManager gsm(p);
    json j;

    // update members for testing
    gsm.game_level_up();
    gsm.unload();

    // convert GameStateManager to json (to_json)
    j = gsm;
    // convert json to GameStateManager (from_json)
    auto gsm2 = j.get<::GameStateManager>();

    // compare
    EXPECT_EQ(gsm,gsm2);
}