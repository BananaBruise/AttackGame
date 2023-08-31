#include <gtest/gtest.h>
#include "GameStateManager.h"
#include "Player.h"

using namespace std;

// test player object-based constructor
TEST(GameStateManTest, AssertPlayerConstructor)
{
    Player p;
    GameStateManager gsm(p);

    EXPECT_TRUE(gsm.isLoaded()) << "player not loaded";
    EXPECT_EQ(gsm.getGameLevel(), 1) << "game level not 1";
    EXPECT_EQ(gsm.getPlayer(), p) << "incorrect player reference did not returned";
}

// test json file based constructor
TEST(GameStateManTest, AssertJsonConstructor)
{

}

// test saving file
TEST(GameStateManTest, AssertPlayerSave)
{
    
}