#ifndef ROOM_H
#define ROOM_H
#include <iostream>
#include <string>
#include <vector>
#include "Robot.h"
using namespace std;

class Robot;

class Room {
private:
    string name;
    Robot* firstRobot;
    Room* leftRoom;
    Room* rightRoom;
    int numChildren;
    int numRobots;
	
public:
    Room();
    //Acessor
    string getName();
    Robot* getFirstRobot();
    Room* getLeftRoom();
    Room* getRightRoom();


    //Mutator
    void setName(string newName);
    void setFirstRobot(Robot* newFirstRobot);
    void setLeftRoom(Room* newLeftRoom);
	void setRightRoom(Room* newRightRoom);


    //class methods
    Robot searchRobotByName(string& name);
    Robot searchRobotByID(int id);
	void printAllNames();


    void swapRobotData(int indx1, int indx2);
    void copyRobot(int idx1, int idx2);
    void pushRobotData(int roomidx, int data);
	void enqueueRobot(Robot* r);
	Robot* dequeueRobot();


	void testPatience(double probability);
	void moveRobot();

	//Binary Tree Traversal methods
	bool checkPreorder(Room* root);
	bool checkInorder(Room* root);
    bool checkPostorder(Room* root);

    //Bonus
};


#endif
