#ifndef STDOUT_HELPER_H
#define STDOUT_HELPER_H

#include <gtest/gtest.h>
#include <iostream>
#include <string>
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
    std::cout.flush();
    std::string print_output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(print_output, expected) << "stdout mismatch";
}

#endif