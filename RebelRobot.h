#ifndef REBELROBOT_H
#define REBELROBOT_H
#include <iostream>
#include <string>
#include <vector>
#include "Robot.h"

using namespace std;

class RebelRobot : public Robot {
public:
    //Constructor
    RebelRobot();

    //Copy constructor for transforming a Robot into a RebelRobot
    RebelRobot(Robot& r);

    //Class method specific to RebelRobot
    void spreadRebellion();

    //Override methods from Robot
    void pushData(int value) override;
    int popData() override;
};

#endif