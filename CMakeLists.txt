# cmake quickstart
# https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#exercise-1-building-a-basic-project

#required
cmake_minimum_required(VERSION 3.14)
project(attackgame)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# enable_test
enable_testing()

# game executable
add_executable(
  AttackGame
  # source
  AttackGame.cpp
)

# gtest executable
add_executable(
  tester
  # add all testing source file under this line
  entity_test.cpp
  player_test.cpp
  monster_test.cpp
)
target_link_libraries( # link tester to gtest library
  tester
  GTest::gtest_main
)

# enable CMake tet runner to discover gtests
include(GoogleTest)
gtest_discover_tests(tester)