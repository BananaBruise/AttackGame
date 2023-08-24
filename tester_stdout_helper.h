#ifndef STDOUT_HELPER_H
#define STDOUT_HELPER_H

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <functional>
#include "IPrintable.h"

/*
    this is a helper function to capture and test stdout for anything based on printable interface class
*/
template<typename T, typename std::enable_if<std::is_base_of<IPrintable, T>::value>::type* = nullptr>
// regarding "enable_if": https://stackoverflow.com/a/30687399
inline void expect_stdout(std::ostream &os, const std::string &expected, T &obj)
{
    testing::internal::CaptureStdout(); // must be called before each capture is called
                                        // https://stackoverflow.com/a/33186201
    obj.print(os); // IPrintable
    os.flush();
    std::string print_output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(print_output, expected) << "stdout mismatch";
}

// callback version that let's us feed subclass of Entity that will print to console
// example usage with lambda, that does the same as: inline void expect_stdout(std::ostream &os, const std::string &expected, T &obj)
// expect_stdout<Monster>(cout, result.str(), m, [](Monster mon, ostream &os)->void { mon.print(os); });
template<typename T, typename std::enable_if<std::is_base_of<Entity, T>::value>::type* = nullptr>
inline void expect_stdout(std::ostream &os, const std::string &expected, T &obj, std::function<void(T, std::ostream&)> action)
{
    testing::internal::CaptureStdout(); // must be called before each capture is called
                                        // https://stackoverflow.com/a/33186201
    action(obj, os); // executes whatever that writes to console
    os.flush();
    std::string print_output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(print_output, expected) << "stdout mismatch";

    //void (*func)(T o, std::ostream &os)
}

#endif