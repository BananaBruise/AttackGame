#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "Entity.h"

using namespace std;

// forward declaration


TEST(EntityTest, AssertDefaultConstructor)
{
    Entity a;
    // using getter
    EXPECT_EQ(a.get_hp(), 0.0) << "hp mismatch";
    EXPECT_EQ(a.get_mp(), 0.0) << "mp mismatch";
    EXPECT_EQ(a.get_desp(), "no description") << "description mismatch";
    
    // using print
    testing::internal::CaptureStdout(); // must be called before each capture is called
                                        // https://stackoverflow.com/a/33186201
    a.print(cout);
    cout.flush();
    string print_output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(print_output, "hp: 0.0, mp: 0.0, description: no description") << "print mismatch";
}

// TODO: test setters