/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Floor.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Katherine Gurgenian
 * gurgenia
 *
 * Final Project - Elevators
 */


#include "Floor.h"

using namespace std;

int Floor::tick(int currentTime) {
    int numExploded = 0;
    int peopleToRemove[MAX_PEOPLE_PER_FLOOR];
    int indexPlaced = 0;
    
    //runs through people array
    for (int i = 0; i < numPeople; i++) {
        if (people[i].tick(currentTime)) {
            numExploded++;
            //indexPlaced used to keep track of people exploded
            peopleToRemove[indexPlaced] = i;
            indexPlaced++;
        }
    }
    
    removePeople(peopleToRemove, numExploded);

    return numExploded;
}

void Floor::addPerson(Person newPerson, int request) {
    if (numPeople < MAX_PEOPLE_PER_FLOOR) {
        people[numPeople] = newPerson;
        numPeople++;
        if(request > 0) {
            hasUpRequest = true;
        }
        else {
            hasDownRequest = true;
        }
    }
    return;
}

void Floor::removePeople(int indicesToRemove[MAX_PEOPLE_PER_FLOOR], int numPeopleToRemove) {
    Person peopleTemporary[MAX_PEOPLE_PER_FLOOR];
    int sum;
    int index = 0;
    int holderNumPeople = 0;
    
    //runs through indices of people array
    for (int i = 0; i < numPeople; i++) {
        sum = 0;
        for (int j = 0; j < numPeopleToRemove; j++) {
                    if (i != indicesToRemove[j]) {
                        sum++;
                    }
        }
        
        //this means that a value in the indicesToRemove did not match any of the indices of the people array
        if (sum == numPeopleToRemove) {
            //index makes sure all people kept are shifted to beginning of array with no gaps
            peopleTemporary[index] = people[i];
            index++;
            holderNumPeople++;
        }
    }
    
    numPeople = holderNumPeople;
    
    //replaces contents of original people array
    for (int i = 0; i < MAX_PEOPLE_PER_FLOOR; i++) {
        people[i] = peopleTemporary[i];
    }
    
    resetRequests();
    
    return;
}

void Floor::resetRequests() {
    hasUpRequest = false;
    hasDownRequest = false;
    
    for(int i = 0; i < numPeople; i++) {
        if (people[i].getTargetFloor() > people[i].getCurrentFloor()) {
            hasUpRequest = true;
        }
        else if (people[i].getTargetFloor() < people[i].getCurrentFloor()) {
            hasDownRequest = true;
        }
    }
    
    return;
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
    hasDownRequest = false;
    hasUpRequest = false;
    numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream& outs) const {
	string up = "U";
	outs << (hasUpRequest ? up : " ") << " ";
	for (int i = 0; i < numPeople; ++i){
		outs << people[i].getAngerLevel();
		outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
	}
	outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream& outs) const {
	string down = "D";
	outs << (hasDownRequest ? down : " ") << " ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "o   ";
	}
	outs << endl;
}

void Floor::printFloorPickupMenu(ostream& outs) const {
	cout << endl;
	outs << "Select People to Load by Index" << endl;
	outs << "All people must be going in same direction!";
	/*  O   O
	// -|- -|-
	//  |   |
	// / \ / \  */
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " O   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "-|-  ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " |   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "/ \\  ";
	}
	outs << endl << "INDEX:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << i << "   ";
	}
	outs << endl << "ANGER:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getAngerLevel() << "   ";
	}
	outs << endl << "TARGET FLOOR: ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getTargetFloor() << "   ";
	}
}

void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
	return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
	return hasDownRequest;
}

int Floor::getNumPeople() const {
    return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
    return people[index];
}
