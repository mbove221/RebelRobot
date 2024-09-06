#include <iostream>
#include <string>
#include "Room.h"
#include "RebelRobot.h"
#include <vector>
#include <fstream>

//Default Constructor
Room::Room() {
	name = ""; 
	firstRobot = nullptr;
	leftRoom = nullptr;
	rightRoom = nullptr;
}

//Acessor
string Room::getName(){
	return name;
}
Robot* Room::getFirstRobot() {
	return firstRobot;
}
Room* Room::getLeftRoom(){
	return leftRoom;
}

Room* Room::getRightRoom() {
	return rightRoom;
}


//Mutator
void Room::setName(string newName){
	name = newName; 
}

void Room::setFirstRobot(Robot* newFirstRobot){
	firstRobot = newFirstRobot;
}

void Room::setLeftRoom(Room* newLeftRoom){
	leftRoom = newLeftRoom;
}

void Room::setRightRoom(Room* newRightRoom){
	rightRoom = newRightRoom;
}


//iterate through linked list to find robot by name
Robot Room::searchRobotByName(string& name) {
	Robot* tmp = firstRobot;
	while (tmp != nullptr) {
		if (strcmp(tmp->getName().c_str(), name.c_str()) == 0) { //compare names
			cout << "Robot found!" << endl;
			tmp->displayRobot();
			return *tmp; //return robot if found
		}
		tmp = tmp->getNextRobot();
	}
	cout << "Robot not found!";
	return Robot(); //return empty robot if nothing found
}

//iterate through linked list to find robot by ID
Robot Room::searchRobotByID(int id) {
	Robot* tmp = firstRobot;
	while (tmp != nullptr) {
		if (tmp->getID() == id) { //compare IDs
			cout << "Robot found!" << endl;
			tmp->displayRobot();
			return *tmp; //return robot if found
		}
		tmp = tmp->getNextRobot();
	}
	cout << "Robot not found!";
	return Robot(); //return empty robot if nothing found

}

//swaps robot data at idx1 and idx of robots vector
void Room::swapRobotData(int idx1, int idx2) {
	//link list instead of vector
	Robot* r1 = firstRobot;
	Robot* r2 = firstRobot;
	for (int i = 0; i < idx1; i++) {
		r1 = r1->getNextRobot(); //get robot 1
	}
	for (int i = 0; i < idx2; i ++ ) {
		r2 = r2->getNextRobot(); //get robot 2
	}
	*r1 - *r2; //swap data at index 1 and index 2 in linkd list for robots 1 and 2
}

//adds data at robot index and room index
void Room::pushRobotData(int roomidx, int data) {
	Robot* ro = firstRobot;
	for (int i = 0; i < roomidx; i++) {
		ro = ro->getNextRobot();
	}
	ro->pushData(data);
	//Robot at position roomidx in the queue will push data onto its stack. Do nothing and report to screen if the stack is full or no Robot at position roomidx.
}

//copies robot at index 2 into robot at index1
void Room::copyRobot(int idx1, int idx2) {
	//link list instead of vector
	Robot* r1 = firstRobot;
	Robot* r2 = firstRobot;
	for (int i = 0; i < idx1; i++) {
		r1 = r1->getNextRobot(); //get robot 1
	}
	for (int i = 0; i < idx2; i++) {
		r2 = r2->getNextRobot(); //get robot 2
	}
	*r1 << *r2; //swap data at index 1 and index 2 in linkd list for robots 1 and 2
}

//print all robot names
void Room::printAllNames() {
	Robot* tmp = firstRobot;
	cout << "All robot names: " << endl;
	while (tmp != nullptr) {
		cout << tmp->getName() << endl; //printing all robot names
		tmp = tmp->getNextRobot();
	}
}

//Adds a Robot to the end of the queue
void Room::enqueueRobot(Robot* r) {
	if (firstRobot == nullptr) {
		firstRobot = r;
		firstRobot->setNextRobot(nullptr);
	}
	Robot* tmp = firstRobot;
	while (tmp->getNextRobot() != nullptr) { //iterate up to last robot
		tmp = tmp->getNextRobot();
	}
	tmp->setNextRobot(r); //set next robot to the input robot in parameters
	tmp->getNextRobot()->setNextRobot(nullptr);
}

//removes the first Robot from the queue
Robot* Room::dequeueRobot() {
	Robot* tmp = firstRobot;
	firstRobot = firstRobot->getNextRobot();

	return tmp;
}

//after rebel robot is created
void Room::testPatience(double probability) {
	int size = 0; //use to store amount of robots in room
	Robot* next; //used to get next robot
	Robot* tmp;
	Robot* change_robot = firstRobot; //set tmp back to first robot in linked list
	Robot* prev = firstRobot;
	while(change_robot!=nullptr){//for each robot in the room
		double chance = ((double)rand() / RAND_MAX); //compute random chance between 0 and 1
		string type = change_robot->getName().substr(0, 5);
		if (strcmp(type.c_str(), "Rebel") == 0) { //if rebel in it's name, call spread rebellion in rebel robot object
			RebelRobot* rebel = dynamic_cast<RebelRobot*>(change_robot);
			rebel->spreadRebellion(); //spread rebellion 
		}
		if (chance <= probability) { //if chance is less than or equal to probability
			
			if (strcmp(type.c_str(), "Rebel") != 0){
				if (change_robot == firstRobot) {
					next = change_robot->getNextRobot();
					RebelRobot rr = RebelRobot(*change_robot);//create new rebel robot with identical parameters
					tmp = new RebelRobot(rr); //set tmp to point to rr
					firstRobot = tmp;
					firstRobot->setNextRobot(next);
				}
				else{
					next = change_robot->getNextRobot(); 
					RebelRobot rr = RebelRobot(*change_robot); //create new rebel robot with identical parameters
					tmp = new RebelRobot(rr); //set tmp to point to rr
					prev->setNextRobot(tmp);
					tmp->setNextRobot(next);
				}
			}
		}
		prev = change_robot; //set previous robot to current robot
		change_robot = change_robot->getNextRobot(); //get next robot
	}
}

//after test patience
void Room::moveRobot() {
	int chance = ((double)rand() / RAND_MAX) * (99) + 1; //generate probability from 1 to 100
	if (chance <= 50) { //if chance is between 1 and 50
		if (this->getLeftRoom() != nullptr) { //move left room if open
			Robot* robo = this->dequeueRobot(); //remove from current room
			this->getLeftRoom()->enqueueRobot(robo); //add to left room
			testPatience(0.15);
			return;
		}
		else if (this->getRightRoom() != nullptr) { //otherwise move to right room if open
			Robot* robo = this->dequeueRobot(); //remove from current room
			this->getRightRoom()->enqueueRobot(robo); //add to left room
			testPatience(0.15);
			return;
		}
	}
	else { //if chance is greater than 50
			if (this->getRightRoom() != nullptr) { //move to right room if open
				Robot* robo = this->dequeueRobot(); //remove from current room
				this->getRightRoom()->enqueueRobot(robo); //add to left room
				testPatience(0.15);
				return;
		}
			else if (this->getLeftRoom() != nullptr) { //otherwise move to left room if 
				Robot* robo = this->dequeueRobot(); //remove from current room
				this->getLeftRoom()->enqueueRobot(robo); //add to left room
				testPatience(0.15);
				return;
			}
	}
	//if both rooms were empty
	testPatience(0.3); //call test patience 0.3
}

bool Room::checkPreorder(Room * root) {
	if (root == nullptr) {
		return true; //base case
	}
	bool isAllRebels = true;
	Robot* tmp = root->getFirstRobot();
	while (tmp != nullptr) {
		string type = tmp->getName().substr(0, 5);
		if (strcmp(type.c_str(), "Rebel") != 0) { //if no rebel in it's name, that means normal robot
			isAllRebels = false;
		}
		tmp = tmp->getNextRobot();
	}
	cout << root->getName() << endl;
	bool leftRebels = checkPreorder(root->getLeftRoom()); //boolean if all are rebels in left subtree
	bool rightRebels = checkPreorder(root->getRightRoom()); //boolean if all are rebels in right subtree
	return (isAllRebels && leftRebels && rightRebels); //return true if every robot is rebel
}

bool Room::checkInorder(Room * root) { //inorder traversal
	if (root == nullptr) {
		return true; //base case
	}
	bool isAllRebels = true;
	Robot* tmp = root->getFirstRobot();
	while (tmp != nullptr) {
		string type = tmp->getName().substr(0, 5);
		if (strcmp(type.c_str(), "Rebel") != 0) { //if no rebel in it's name, that means normal robot
			isAllRebels = false;
		}
		tmp = tmp->getNextRobot();
	}
	bool leftRebels = checkInorder(root->getLeftRoom()); //boolean if all are rebels in left subtree
	cout << root->getName() << ' '<< endl;
	bool rightRebels = checkInorder(root->getRightRoom()); //boolean if all are rebels in right subtree
	return (isAllRebels && leftRebels && rightRebels); //return true if every robot is rebel
}

bool Room::checkPostorder(Room * root) { //postorder traversal
	if (root == nullptr) {	
		return true; //base case
	}
	bool isAllRebels = true;
	Robot* tmp = root->getFirstRobot();
	while (tmp != nullptr) {
		string type = tmp->getName().substr(0, 5);
		if (strcmp(type.c_str(), "Rebel") != 0) { //if no rebel in it's name, that means normal robot
			isAllRebels = false;
			break;
		}
		tmp = tmp->getNextRobot();
	}
	bool leftRebels = checkPostorder(root->getLeftRoom()); //boolean if all are rebels in left subtree
	bool rightRebels = checkPostorder(root->getRightRoom());//boolean if all are rebels in right subtree
	cout << root->getName() << endl;
	return (isAllRebels && leftRebels && rightRebels); //return true if every robot is rebel
}

