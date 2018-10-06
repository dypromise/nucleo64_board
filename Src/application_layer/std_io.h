/********************************
 * Name:    StdIO.hpp
 * Author:  Bryant Gonzaga
 * Date Modified:   2 Feb. 2017
 ********************************/

#ifndef STDIO_HPP_
#define STDIO_HPP_

#include "IO_interface.h"

class StdIO : public IOInterface {
    bool allowFile = false;
public:
    // Constructor
    explicit StdIO(bool allowFile);

    // Overridden methods
    int open(char *filename, const char *mode) override;

    int getchar() override;

    size_t read(void *ptr, size_t size, size_t count) override;

    size_t write(const void *ptr, size_t size, size_t count) override;
};

#endif /* STDIO_HPP_ */
