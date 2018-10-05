/*
 * mazeIO.hpp
 *
 *  Created on: Jan 17, 2017
 *      Author: vagrant
 */

#ifndef MAZEIO_HPP_
#define MAZEIO_HPP_

#include "config_maze.h"
#include "wall.h"
#include "cell.h"
#include "direction.h"
#include "position.h"
#include "position_controller.h"
#include "IO_interface.h"

#include <vector>
#include <cstdio>
#include <cstddef>

// Forward declaration
class Maze;

/*******************************************************************************
 * Class Declaration
 ******************************************************************************/
class MazeIO {
private:
    Maze *maze;
    int maxRowSize;
    int maxColSize;
    PositionController mousePosition;
    std::vector<Position> destinations;
    char buffer[CONFIG_MAZE_IO_BUFFER_SIZE];
    // IOInterface objects for IO
    IOInterface *fileIO;
    IOInterface *printIO;

    // printing helper
    void writeBufferFromMaze(bool isShowMouse);

    void writeIOFromBuffer(IOInterface *io);

    void printCell(int row, int col, bool isShowMouse, char *buf);

public:
    // Constructor
    MazeIO(Maze *mazePtr, IOInterface *fileIO, IOInterface *printIO);

    // Mouse Position
    Position getMousePosition();

    void setMousePosition(Position pos);

    void setMousePosition(PositionController posCon);

    // Mouse Destinations
    void setDestinations(const std::vector<Position> &des);

    bool positionIsDestination(Position pos);

    // Actual IO
    void printMaze();

    void loadMazeFromString(char *str);

    void loadMazeFromFile(char *fileName);

    void saveMazeToFile(char *fileName);
};

/*******************************************************************************
 * Inline function definition
 ******************************************************************************/


#endif /* MAZEIO_HPP_ */
