#include <iostream>
#include <string>
#include "Robot.h"
#include <random>
using namespace std;

template<typename Type>
void display(Type val) {
	cout << val << endl;
}

Robot::Robot() {
	name = "";
	ID = -1;
	for (int i = 0; i < 5; i++) {
		data[i] = -1; //initialize all values of data to -1
	}
	average = -1;
	stackCursor = -1;
}

	//Accessors
string Robot::getName()const {
	return name;
}
int Robot::getID() const {
	return ID;
}
int Robot::peekData() {
	return data[stackCursor];
}
double Robot:: getAverage() const {
	return average;
}

int Robot::getStackCursor() {
	return stackCursor;
}

Robot* Robot::getNextRobot() {
	return nextRobot;
}

Room* Robot::getCurrentRoom() {
	return currentRoom;
}

int Robot::getData(int index){
	return data[index];
}

	//mutators
void Robot::setName(string newName) {
	name = newName;
}
void Robot::setID(int newID) {
	ID = newID;
}

void Robot::setNextRobot(Robot* newNextRobot) {
	nextRobot = newNextRobot;
}

void Robot::setCurrentRoom(Room* newCurrentRoom) {
	currentRoom = newCurrentRoom;
}

void Robot::setStackCursor(int newStackCursor) {
	stackCursor = newStackCursor;
}

void Robot::setData(int index, int value) {
	int sum = 0;
	data[index] = value;
	if (this->getStackCursor() == -1) {
		return;
	}
	for (int i = 0; i < this->getStackCursor(); i++) {
		sum += data[i]; //get sum of all data
	}
	average = (1.0*sum / this->getStackCursor() +1); //update average
}

void Robot::setAverage(double avg) {
	average = avg;
}

//set data at a given index
void Robot::pushData(int value) {
	int size = stackCursor + 1; //set size to one more than stack cursor
	int sum = 0;
	if (stackCursor < 4) { //make sure stack cursor is valid
		stackCursor++;
		data[stackCursor] = value;
	}
	else { 
		cout << "Cannot push to a full stack" << endl; //if stack cursor >= 4, we can't push any more values
		return;
	}
	if (size == 0) { //reset average
		average = -1;
		return;
	}

	for (int i = 0; i < size; i++) { //otherwise, calculate average if has data
		sum += data[i];
	}
	average = (1.0*sum / 1.0*size);
}

int Robot::popData() { //pop data from stack
	if (stackCursor > -1) {
		int top = data[stackCursor];
		stackCursor--;
		cout << "Popping " << top << " from stack." << endl;
		int size = stackCursor + 1;
		int sum = 0;
		for (int i = 0; i < size; i++) { //same calculations as pushing data
			sum += this->getData(i);
		}
		average = (1.0*sum / 1.0*size);
		return top;
	}
	else {
		cout << "Cannot pop from empty stack." << endl;
		return -1;
	}
}

//display attributes of robot
void Robot::displayRobot(){
	int sum = 0;
	cout << "Name: ";
	display(name);
	cout << endl;
	cout << " ID: ";
	display(ID); 
	cout << endl;
	cout << " Data: " << endl;
	for (int i = 0; i <= stackCursor; i++) {
		display(data[i]);
		cout << " ";
	}
	cout << endl;
	cout << "Average: ";
	for (int i = 0; i <= stackCursor; i++) {
		sum += getData(i);
	}
	average = ((1.0 * sum) / (1.0 * (stackCursor + 1))); //recalculate average

	display(average); //print average
	cout << endl; 
}

	//Operator overloading
void Robot::operator <<(const Robot& r2) { //copies r2 into (this) robot we call operation overload on
	this->setID(r2.ID);
	int size = r2.stackCursor + 1;
	for (int i = 0; i < size; i++) {
		data[i] = r2.data[i];
	}
	this->setAverage(r2.getAverage());
	this->setStackCursor(r2.stackCursor);
	this->setName(r2.name);
}

void Robot::operator -(Robot& r2) { //swaps r2 with (this) robot we call operation on
	Robot r3; //create another robot object, r3
	r3.setName(this->getName()); //copy name, ID, data
	r3.setID(this->getID());
	int size = stackCursor + 1;
	
	for (int i = 0; i < size; i++) {
		r3.data[i] = data[i];
	}; //set r3 object equal to original robot object
							//which we'll call r1
	r3.stackCursor = stackCursor;
	this->setName(r2.getName());
	this->setID(r2.getID());
	size = r2.stackCursor + 1;
	for (int i = 0; i <= size; i++) {
		this->data[i] = r2.data[i];
	}
	this->stackCursor = r2.stackCursor;
	//copy all data from robot 3 into r2
	r2 << r3;
	cout << "r2 name: " << r2.getName() << endl;
	}

double Robot::randomDouble(double min, double max) { //generate random double
	return ((double)rand() / RAND_MAX) * (max - min) + min;
}