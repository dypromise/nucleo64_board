
#ifndef GREAT_H_
#define GREAT_H_

#include <finder_Interface.h>
#include <mover_interface.h>

#define LED3_PORT GPIOA
#define LED3_PIN GPIO_PIN_6
#define LED4_PORT GPIOA
#define LED4_PIN GPIO_PIN_5
#define LED5_PORT GPIOB
#define LED5_PIN GPIO_PIN_13

class RealMouse : public FinderInterface, public MoverInterface {
public:
    // Constructor
    RealMouse() = default;

    /* Overriding FinderInterface methods */
    Wall examineWall(int row, int col, Direction wallDir,
                     PositionController &mousePos) override;


    /* Overriding MoverInterface methods */
    void moveTo(int row, int col, Direction destDir,
                PositionController &mousePos) override;


    void rotateTo(Direction destDir, PositionController &mousePos) override;
};

#endif
