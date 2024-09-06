#include <iostream>
#include <string>
#include "Robot.h"
#include "RebelRobot.h"
#include <vector>
#include <fstream>

//Constructor
RebelRobot::RebelRobot(){
    this->setName("");
    this->setID(-1);
    this->setStackCursor(-1);
    for (int i = 0; i < 5; i++) {
        this->pushData(-1);
    }
    this->setStackCursor(-1);
}

//Copy constructor for transforming a Robot into a RebelRobot
RebelRobot::RebelRobot(Robot& r){
    *this << r; //copy robot over
    this->setName("Rebel" + this->getName()); //add rebel to name
}

//Class method specific to RebelRobot
void RebelRobot::spreadRebellion(){
    int chance = ((double) rand() / RAND_MAX) * (99) + 1; //generate probability from 1 to 100
    cout << "Chance for robot: " << this->getName() << ": " << chance << endl;
    if(this->getNextRobot() != nullptr){
     if (chance <= 20) { //if number is between 1 and 20 INCLUSIVE
        // Overwrite the nearestRobot's name and ID with rebelRobot's if not already a rebel robot
        string type = this->getNextRobot()->getName().substr(0, 5);
        Robot* current;
        Robot* next;
        cout << "Reached" << endl;
        if(strcmp(type.c_str(), "Robot") == 0) { //if it's a robot, convert it and overwrite information
            RebelRobot* rr = new RebelRobot(*this); //place in linked list
            current = this;
            next = this->getNextRobot()->getNextRobot();
            current->setNextRobot(rr);
            rr->setNextRobot(next);
        }
        else if (strcmp(type.c_str(), "Rebel") == 0) { //if it's already rebel robot, just overwrite the information
            RebelRobot* r = new RebelRobot(*this); //place it in linked list
            current = this;
            next = this->getNextRobot()->getNextRobot();
            current->setNextRobot(r);
            r->setNextRobot(next);
          }
      }
    }
}

//Override methods from Robot

    void RebelRobot::pushData(int value){
        int size = this->getStackCursor() + 1; //set size (1 more than stack cursor position)
        int sum = 0;
        if (this->getStackCursor() < 4) { //make sure we can push data
            setStackCursor(this->getStackCursor() + 1);
            this->setData(this->getStackCursor(), value);
        }
        else {
            cout << "Cannot push to a full stack" << endl;
            return;
        }
        if (size == 0) {
            this->setAverage(-1); //reset average to -1
            return;
        }

        for (int i = 0; i < size; i++) {
            sum += this->getData(i);
        }
        double average = (sum / size); //calculate average
        this->setAverage(average);
        this->spreadRebellion(); //spread rebellion
    }

    int RebelRobot::popData() {
        if(this->getStackCursor() > -1){ //make sure can pop from stack
            int top = this->getData(this->getStackCursor());
            this->setStackCursor(getStackCursor() - 1);
            cout << "Popping " << top << " from stack." << endl;
            int size = this->getStackCursor() + 1;
            int sum = 0;
            for (int i = 0; i < size; i++) {
                sum += this->getData(i);
            }
            double average = (sum / size);
            this->setAverage(average);
            this->spreadRebellion(); //spread rebellion
            return top;
        }
        else {
            cout << "Cannot pop from empty stack." << endl; //can't pop from empty stack
            return -1;
        }
    }