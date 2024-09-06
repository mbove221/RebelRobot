#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib> // for rand()
#include "Robot.h"
#include "Room.h"
#include "RebelRobot.h"

using namespace std;

void menu();
Room* searchRoomByName(const string& name, Room* root);

//Room* constructRoomTree();
//Room* constructRoomTree(Room* root);


void constructBinaryTree(Room*);
void preOrderPrint(Room*);

static vector<Room*> rooms;   // Vector to store rooms
static vector<Robot> robots(20);
static vector<int> num_children;
static vector<int> num_robots;
int roomNumber;

int iterator;

int main() {
    srand(time(0)); //set seed for random
    
    string robotName;
    string roomName;
    int id;
    int data[5];

    int roomidx;
    double probability;

    int numChildren;
    int numRobots;

    int value;

    string new_line;
    ifstream robotFile("Robots.txt");
    if (!robotFile.good()) {
        cerr << "Trouble opening Robots.txt";
        exit(1);
    }
    int data_value;
    // Construct robot vector
    for (int i = 0; i < 20; i++) {
        Robot robot;
        robotFile >> robotName >> id;
        for (int i = 0; i < 5; i++) {
            robotFile >> data_value;
            robot.pushData(data_value);
        }
        for (int i = 0; i < 5; i++) {
            if (robot.getData(i) == -1) {
                robot.setStackCursor(i - 1);
            }
        }
        getline(robotFile, new_line); // Get rid of new line
        robot.setName(robotName);
        robot.setID(id);
            ; // Set data in robot
        
        robots[i] = robot; // Add robot to vector
    }

    robotFile.close();

    // Read rooms from file
    ifstream roomFile("Rooms.txt");
    if (!roomFile.good()) {
        cerr << "Trouble opening Rooms.txt";
        exit(1);
    }

    //construct room vector
  
    while (!roomFile.eof()) { 
        Room* room = new Room();

        roomFile >> roomName >> numChildren >> numRobots;
        getline(roomFile, new_line); // Get rid of empty line
        room->setName(roomName);
        rooms.push_back(room);//add rooms to vector

        num_children.push_back(numChildren); //push back number of children to num_children vector
        num_robots.push_back(numRobots); //push back number of robots to num_robots vector
 
    }

    roomFile.close();
    int room_idx = 0;
    int numRobotsAdded = 0;
    //add robots to room

    while (room_idx < rooms.size() - 1) {
        if (num_robots[room_idx] <= numRobotsAdded || num_robots[room_idx] == 0) { //if room is full
            numRobotsAdded = 0; //reset number of robots added to room
            room_idx++; //get next room
        }
        if(num_robots[room_idx] != 0){ //make sure the room is allowed to have children
               probability = static_cast<double>(rand()) / RAND_MAX; //generate probability
               int robot_number = (int)(probability * robots.size()); //set number from 0 and 19 inclusive
               Robot* toadd = new Robot();
               *toadd << robots[robot_number]; //add robot at random index to room
               rooms[room_idx]->enqueueRobot(toadd);
               numRobotsAdded++; //increase number of robots in room
        }
    }

    Room* root = rooms[0];
    constructBinaryTree(root);
   
    int pick;
    int rooomidx;
    int option;
    string name;
    Robot* tmp;
    bool rebels;
    Room* newRoom;
    while (1) {
        string view_names;
        string roomFind;
        cout << "Write 'print' to see room names. Write 'quit' to quit. Otherwise, skipping to room search: " << endl;
        cin >> view_names;
 
        if (strcmp(view_names.c_str(), "quit") == 0) { 
            exit(1); // Exit if the user enters 'quit'
        }

        if (strcmp(view_names.c_str(), "print") == 0) { //print rooms if option is "print"
            cout << "Printing all room names..." << endl;
            preOrderPrint(root);
        }

        bool found = 1; //flag to indicate if found
        cout << "Enter room to search: ";
        cin >> roomFind;
        Room* foundRoom = searchRoomByName(roomFind, root);
        if (foundRoom == nullptr) {
            found = 0;
        }
        if(found){
            menu(); //print the menu to the screen
            cout << "Enter your option: ";
            cin >> option;
            switch (option) {
            case 1: //1. Print All Robot Names
                foundRoom->printAllNames();
                break;
            case 2: //2.  Display Robot 
                tmp = foundRoom->getFirstRobot();
                cout << "Enter index of robot to find: ";
                cin >> roomidx;
                for(int i = 0; i < roomidx; i++){
                    if (tmp == nullptr) {
                        cout << "Could not find robot with name " << name << endl;
                    }
                    tmp = tmp->getNextRobot();
                }
                break;
            case 3: //3. Push Robot Data
                cout << "Enter room index: ";
                cin >> roomidx;
                cout << "Enter data value to push: ";
                cin >> value;
                foundRoom->pushRobotData(roomidx, value);
                break;
            case 4: // Pop Robot Data
                 cout << "Enter room index: ";
                 cin >> roomidx;
                 tmp = foundRoom->getFirstRobot();
                 for (int i = 0; i < roomidx; i++) {
                     if (tmp == nullptr) {
                         cout << "Robot not in list." << endl;
                     }
                     tmp = tmp->getNextRobot();
                 }
                tmp->popData();
                break;
            case 5: //5. Test patience 
                cout << "Enter probability: ";
                cin >> probability;
                foundRoom->testPatience(probability);
                break;
            case 6: //6. Move Robot
                foundRoom->moveRobot();
                break;
            case 7: //7. Check Preorder
                rebels = foundRoom->checkPreorder(root);
                cout << "Is every room filled with rebels? " << rebels << endl; 
                break;
            case 8: //8. Check Inorder 
                rebels = foundRoom->checkInorder(root);
                cout << "Is every room filled with rebels? " << rebels << endl; //0 indicates no, 1 indicates yes
                break;
            case 9: //9. Check Postorder 
                rebels = foundRoom->checkPostorder(root);
                cout << "Is every room filled with rebels? " << rebels << endl; //0 indicates no, 1 indicates yes
                break;
            default:
                cout << "Please enter a valid option." << endl;
            }
        }
        else {
            cout << "Error locating room. Please try again." << endl;
             }
        }
        return 0;
}


void constructBinaryTree(Room* root) { //construct binary tree
    int first_child_index = 0; //first child index
    int second_child_index = 0; //second child index
    Room* tmp = new Room(); //allocate memory for room
    while(first_child_index < num_children.size() - 1){ //make sure valid index
      while (first_child_index < num_children.size() - 1 && num_children[first_child_index] != 0) { //make sure room can have child
        tmp = rooms[first_child_index]; //set a tmp pointer to index with first child
        int next_child_index = first_child_index + 1; //get next room and set it to next child of current room
        tmp->setLeftRoom(rooms[next_child_index]); 
        num_children[first_child_index]--;  //decrease number of remaining children allowed 
        first_child_index++;
    }
    second_child_index = first_child_index; //second child index equals first child index
    while (num_children[second_child_index] == 0 && second_child_index >= 0) { //go back to get next index for second child
        second_child_index--;
    }
    first_child_index++; //increment to next index
    if (first_child_index < rooms.size()) { //make sure valid index
        rooms[second_child_index]->setRightRoom(rooms[first_child_index]); //set child of second child to the first child
      }
    }
}

Room* searchRoomByName(const string& name, Room* root) {
    if (!root) {
        return nullptr;
    }

    if (strcmp(root->getName().c_str(), name.c_str()) == 0) {
        return root;
    }

    Room* leftResult = searchRoomByName(name, root->getLeftRoom());
    if (leftResult != nullptr) {
        return leftResult;
    }
    Room* rightResult = searchRoomByName(name, root->getRightRoom());
    return rightResult;
}

void menu() {
    cout << "1. Print All Robot Names" << endl
        << "2. Display Robot" << endl
        << "3. Push Robot Data" << endl
        << "4. Pop Robot Data" << endl
        << "5. Test Patience" << endl
        << "6. Move Robot" << endl
        << "7. Check Preorder" << endl
        << "8. Check Inorder" << endl
        << "9. Check Postorder" << endl;
}

void preOrderPrint(Room* root) {
    if (root == nullptr)
        return;
    cout << root->getName() << endl;
    preOrderPrint(root->getLeftRoom());
    preOrderPrint(root->getRightRoom());
}
