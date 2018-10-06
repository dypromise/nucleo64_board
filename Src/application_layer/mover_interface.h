/*
 * MovingInterface.hpp
 *
 *  Created on: Mar 5, 2017
 *      Author: vagrant
 */

#ifndef MOVERINTERFACE_HPP_
#define MOVERINTERFACE_HPP_

#include "direction.hpp"
#include "position_controller.h"

class MoverInterface {
protected:
public:
    virtual void moveTo(int row, int col, Direction destDir,
                        PositionController &mousePos) = 0;

    void moveTo(Position pos, Direction destDir, PositionController &mousePos) {
        return moveTo(pos.row, pos.col, destDir, mousePos);
    }

    virtual void rotateTo(Direction destDir, PositionController &mousePos) = 0;
};

#endif /* MOVERINTERFACE_HPP_ */
