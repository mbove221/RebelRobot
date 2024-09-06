#ifndef ROBOT_H
#define ROBOT_H
#include <iostream>
#include <string>
#include "Room.h"
using namespace std;

class Room;

class Robot {
private:
	string name;
	int ID;
	int data[5];
	double average;
	int stackCursor;
	Robot* nextRobot;
	Room* currentRoom;
	
public:
	//Constructor
	Robot();
	//Accessors
	string getName()const;
	int getID() const;
	int peekData(); //return top value of stack
	double getAverage() const;
	Robot* getNextRobot();
	Room* getCurrentRoom();
	int getStackCursor();
	int getData(int index);

	//Mutators
	void setName(string newName);
	void setID(int newID);
	void setData(int index, int value);
	void setAverage(double avg);

	virtual void pushData(int value);
	virtual int popData();
	void setNextRobot(Robot* newNextRobot);
	void setCurrentRoom(Room* newCurrentRoom);
	void setStackCursor(int newStackCursor);

	//Class methods
	void displayRobot();
	

	//Operator overloading
	void operator <<(const Robot& r2);
	void operator -(Robot& r2);

	//helper function
	double randomDouble(double min, double max);

};

#endif