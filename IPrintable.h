#ifndef IPRINTABLE_H
#define IPRINTABLE_H

#include <iostream>

/*
interface for printable function
*/
class IPrintable {
    virtual inline std::ostream &print(std::ostream &is) const = 0;
};

#endif