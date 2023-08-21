# AttackGame

## What is this?
User must defeat 10 monsters, each in increasing power and abilities.

## Purpose
Learn C++:
- apply OOP concepts
- unit testing ([googletest](https://github.com/google/googletest/tree/main))
- basic automated GitHub testing (using Action)

## build and run
### pre-req (as tested):
  - Ubuntu 22.04.3 LTS (on WSL)
  - [cmake](https://cmake.org/)
  - [make](https://man7.org/linux/man-pages/man1/make.1.html)

### build
1. Generate build: In `root` of project, run `cmake . -B build`, which sources from current dir (`.`) and output build to `build` directory
2. Build: `cmake --build build`

### run
#### run unit test ([reference](http://google.github.io/googletest/quickstart-cmake.html#create-and-run-a-binary))
1. `./build/<test_name>`. Test file appends "normal" file name with `_test`. For example, class defined in `Entity.h` has a tester called `entity_test`