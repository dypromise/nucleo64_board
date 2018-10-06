/********************************
 * Name:    IOInterface.hpp
 * Author:  Bryant Gonzaga
 * Date Modified:   2 Feb. 2017
 ********************************/

#ifndef IOINTERFACE_HPP_
#define IOINTERFACE_HPP_


#include <cstdio>

class IOInterface {
protected:
    FILE *_file;
public:
    IOInterface() : _file(nullptr) {}

    // replacement for fopen()
    virtual int open(char *filename, const char *mode) = 0;

    // replacement for fgetc()
    virtual int getchar() = 0;

    // replacement for fread()
    virtual size_t read(void *ptr, size_t size, size_t count) = 0;

    // replacement for fwrite()
    virtual size_t write(const void *ptr, size_t size, size_t count) = 0;
};

#endif
