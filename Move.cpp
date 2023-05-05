/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */
 
#include <cmath>
#include <sstream>
#include <stdio.h>      
#include <stdlib.h>
#include "Move.h"

using namespace std;

Move::Move(string commandString) : Move() {
    if (commandString == "") {
            isPass = true;
        }
        // save/quit game
        else if (commandString == "s" || commandString == "S") {
            isSave = true;
        }
        else if (commandString == "q" || commandString == "Q") {
            isQuit = true;
        }
        // if service or pickup move, gets elevator ID
        else {
            elevatorId = commandString.at(1) - 48;
            // pickup
            if (commandString.at(2) == 'p') {
                isPickup = true;
            }
            // service
            else if (commandString.at(2) == 'f') {
                targetFloor = commandString.at(3) - 48;
            }
        }

}

bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
    if (isPass || isQuit || isSave) {
        return true;
    }
    // for pickup - if corresponding elevator is not servicing
    if (isPickup) {
        if (elevatorId >= 0 && elevatorId < NUM_ELEVATORS &&
            !elevators[elevatorId].isServicing()) {
            return true;
        }
    }
    else {
        if (elevatorId >= 0 && elevatorId < NUM_ELEVATORS &&
            !elevators[elevatorId].isServicing()) {
            if (targetFloor >= 0 && targetFloor < NUM_FLOORS &&
                targetFloor != elevators[elevatorId].getCurrentFloor()) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    
    return false;
}


void Move::setPeopleToPickup(const string& pickupList, const int currentFloor, const Floor& pickupFloor) {
    numPeopleToPickup = 0;
    totalSatisfaction = 0;

    for (int i = 0; i < pickupList.length(); i++) {
        peopleToPickup[i] = pickupList.at(i) - 48;
        totalSatisfaction += MAX_ANGER - pickupFloor.getPersonByIndex(pickupList.at(i) - 48).getAngerLevel();
        numPeopleToPickup++;
    }

    //setting target floor to the target floor of the person who has to travel the furthest
    int max = 0;
    int person;
    for (int j = 0; j < pickupList.length(); j++) {
        int personTargetFloor = pickupFloor.getPersonByIndex(pickupList.at(j) - 48).getTargetFloor();
        if (abs(personTargetFloor - currentFloor) > max) {
            max = abs(personTargetFloor - currentFloor);
            person = pickupList.at(j) - 48;
            targetFloor = pickupFloor.getPersonByIndex(person).getTargetFloor();
        }
    }
}


//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
    elevatorId = -1;
    targetFloor = -1;
    numPeopleToPickup = 0;
    totalSatisfaction = 0;
	isPass = false;
    isPickup = false;
    isSave = false;
    isQuit = false;
}

bool Move::isPickupMove() const {
    return isPickup;
}

bool Move::isPassMove() const {
	return isPass;
}

bool Move::isSaveMove() const {
	return isSave;
}

bool Move::isQuitMove() const {
	return isQuit;
}

int Move::getElevatorId() const {
    return elevatorId;
}

int Move::getTargetFloor() const {
    return targetFloor;
}

int Move::getNumPeopleToPickup() const {
    return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
    return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
    targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
    for (int i = 0; i < numPeopleToPickup; ++i) {
        newList[i] = peopleToPickup[i];
    }
}
