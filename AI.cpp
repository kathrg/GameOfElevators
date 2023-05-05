;/*
 * Copyright 2023 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Katherine Gurgenian
 * gurgenia
 *
 * Final Project - Elevators
 */

#include "AI.h"
#include "Elevator.h"
#include <cassert>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core

string getAIMoveString(const BuildingState& buildingState) {
    string response = "";
    _Elevator elevator0 = buildingState.elevators[0];
    _Elevator elevator1 = buildingState.elevators[1];
    _Elevator elevator2 = buildingState.elevators[2];
    double averageAnger[NUM_FLOORS];
    int pplPerFloor[NUM_FLOORS];
    
    //get numPeople per floor and put into array
    for (int i = 0; i < NUM_FLOORS; i++) {
        _Floor floor = buildingState.floors[i];
        pplPerFloor[i] = floor.numPeople;
    }
    
    // for the first 5 ticks, move two of the elevators
    if (buildingState.turn < 6) {
        if (buildingState.turn == 0) {
            response = "e1f6";
            return response;
        }
        else if (buildingState.turn == 2) {
            response = "e2f3";
            return response;
        }
        // pass move
        else {
            return response;
        }
    }
    
    // run the bulk of the game
    else if (buildingState.turn > 5 && buildingState.turn < 95) {
        
        //find max amount of people
        int max = 0;
        for (int j = 0; j < NUM_FLOORS; j++) {
            if (pplPerFloor[j] > max) {
                max = pplPerFloor[j];
            }
        }
        
        // find the second max number of people
        int secondMax = 0;
        for (int j = 0; j < NUM_FLOORS; j++) {
            if (pplPerFloor[j] != max && pplPerFloor[j] > secondMax) {
                secondMax = pplPerFloor[j];
            }
        }
        
        //average angerLevel of every floor in the building
        for (int floorNum = 0; floorNum < NUM_FLOORS; floorNum++) {
            double sumAnger = 0.0;
            
            for (int person = 0; person < pplPerFloor[floorNum]; person++) {
                sumAnger += buildingState.floors[floorNum].people[person].angerLevel;
            }
            averageAnger[floorNum] = sumAnger / pplPerFloor[floorNum];
        }
        
        //creating an array of the floors with max people
        int floorsMaxPpl[NUM_FLOORS];
        int index = 0;
        int arrayLength = 0;
        for (int i = 0; i < NUM_FLOORS; i++) {
            if (pplPerFloor[i] == max) {
                floorsMaxPpl[index] = i;
                index++;
                arrayLength++;
            }
        }
        
        // if all elevators are idle AND there is no one on any of the floors, pass move
        bool checkIdleEmpty = true;
            for (int floor = 0; floor < NUM_FLOORS; floor++) {
                if (pplPerFloor[floor] != 0) {
                    checkIdleEmpty = false;
                }
            }
            if (checkIdleEmpty == true) {
                response = "";
                return response;
            }

        //if there's an elevator on a floor w max people it automatically picks up
        bool makePickupMove = false;
        char elevatorToUse = 0;
        int multipleElevators = 0;
        int tempElevator = 0;
        double tempAnger = 0.0;
        int j = 0;
        for (int i = 0; i < arrayLength; i++) {
            if (max != 0) {
                for (j = 0; j < NUM_ELEVATORS; j++) {
                    if (buildingState.elevators[j].currentFloor == floorsMaxPpl[i]
                        && !buildingState.elevators[j].isServicing) {
                        
                        makePickupMove = true;
                        elevatorToUse = (j + 48);
                    }
                }
                // multiple elevators
                if (multipleElevators > 0) {
                    
                    // pickup move for elevator with higher average
                    if (averageAnger[buildingState.elevators[j].currentFloor] > tempAnger) {
                        response = response.append("e");
                        response = response.append(1, elevatorToUse);
                        response = response.append("p");
                        return response;
                    }
                    else {
                        char tempElevatorChar = tempElevator + 48;
                        response = response.append("e");
                        response = response.append(1, tempElevatorChar);
                        response = response.append("p");
                        return response;
                    }
                }
                // pickup move
                else if (makePickupMove && buildingState.elevators[j].currentFloor != 0) {
                    int i = 0;
                    response = response.append("e");
                    response = response.append(1, elevatorToUse);
                    response = response.append("p");
                    tempAnger = averageAnger[i];
                    tempElevator = j;
                    multipleElevators++;
                    return response;
                }
            
            }
        }
        
        // make a move
        if (!makePickupMove) {

            //determine which floor to go to based on minimum distance and max anger
            int minDist = NUM_FLOORS;
            int elevatorToUse = 0;
            int floorToGoTo = 0;
            for (int i = 0; i < arrayLength; i++) {
                for (int j = 0; j < NUM_ELEVATORS; j++) {
                    if (!buildingState.elevators[j].isServicing) {
                        int distOfElevator = fabs(buildingState.elevators[j].currentFloor - floorsMaxPpl[i]);
                        if (distOfElevator < minDist) {
                            //make sure that there isn't already an elevator heading to the target floor
                            if (floorsMaxPpl[i] != elevator0.targetFloor && floorsMaxPpl[i] != elevator1.targetFloor && floorsMaxPpl[i] != elevator2.targetFloor) {
                                minDist = distOfElevator;
                                elevatorToUse = j;
                                floorToGoTo = floorsMaxPpl[i];
                            }
                            // if another elevator is already headed to a max floor, send that elevator to the nearest floor
                            // with second highest max
                            else {
                                // checks first in direction starting from current floor to 9
                                for (int k = buildingState.elevators[j].currentFloor; k < NUM_FLOORS; k++) {
                                    if (pplPerFloor[k] == secondMax) {
                                        floorToGoTo = k;
                                        elevatorToUse = j;
                                        k = NUM_FLOORS;
                                    }
                                }
                                // then checks from floor 0 to current floor
                                for (int k = 0; k < buildingState.elevators[j].currentFloor; k++) {
                                    if (pplPerFloor[k] == secondMax) {
                                        floorToGoTo = k;
                                        elevatorToUse = j;
                                    }
                                }
                                //if more another floor has minDist, compare average anger levels and use whichever floor is higher
                                if (distOfElevator == minDist) {
                                    if (averageAnger[floorsMaxPpl[i]] > averageAnger[floorToGoTo]) {
                                        if (floorsMaxPpl[i] != elevator0.targetFloor && floorsMaxPpl[i] != elevator1.targetFloor
                                            && floorsMaxPpl[i] != elevator2.targetFloor) {
                                            minDist = distOfElevator;
                                            elevatorToUse = j;
                                            floorToGoTo = floorsMaxPpl[i];
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            char elevatorIdChar = (elevatorToUse + 48); //use ascII table
            char floorIdChar = (floorToGoTo + 48);
            
            // if all servicing, pass move
            if (elevator0.isServicing &&
                elevator1.isServicing &&
                elevator2.isServicing) {
                        
                response = "";
                return response;
            }
            // if the floorToGoTo is current floor, pickup INSTEAD of making a move
            else if (floorToGoTo == buildingState.elevators[elevatorToUse].currentFloor) {
                response = response.append("e");
                response = response.append(1, elevatorIdChar);
                response = response.append("p");
                return response;
            }
            //  return move
            else if (!elevator0.isServicing ||
                !elevator1.isServicing ||
                !elevator2.isServicing) {
                        
                response = response.append("e");
                response = response.append(1, elevatorIdChar);
                response = response.append("f");
                response = response.append(1, floorIdChar);
                return response;
            }
        else {
            return response;
        }
    }
}
            //if move >= 95, every elevator just picks up the people on the floor its at
        // if all elevators are idle AND there is no one on any of the floors, pass move
        bool checkIdleEmpty = true;
            for (int floor = 0; floor < NUM_FLOORS; floor++) {
                if (pplPerFloor[floor] != 0) {
                checkIdleEmpty = false;
                }
            }
            if (checkIdleEmpty == true) {
                response = "";
                return response;
            }
    
            if (buildingState.turn >= 95) {
                for (int i = 0; i < NUM_ELEVATORS; i++) {
                    char iChar = i + 48;
                    char floorToGoTo  = '\0';
                    //pick up move if elevator's current floor has > 0 people
                    if (!buildingState.elevators[i].isServicing &&
                        buildingState.floors[buildingState.elevators[i].currentFloor].numPeople > 0) {
                        response = response.append("e");
                        response = response.append(1, iChar);
                        response = response.append("p");
                        return response;
                    }
                    //go to nearest floor with people if current floor has no one
                    else if (!buildingState.elevators[i].isServicing) {
                        // if idle elevator is on floor 9, find closest untargeted floor with non-zero people and move
                        if (buildingState.elevators[i].currentFloor == 9) {
                            for (int j = 1; j < NUM_FLOORS; j++) { // floors
                                if (buildingState.floors[buildingState.elevators[i].currentFloor - j].numPeople != 0) {
                                    floorToGoTo = (10 - j) + 48;
                                }
                            }
                            response = response.append("e");
                            response = response.append(1, iChar);
                            response = response.append("f");
                            response = response.append(1, floorToGoTo);
                            return response;
                            
                        }
                        // if idle elevator is on floor 0
                        else if (buildingState.elevators[i].currentFloor == 0) {
                            for (int j = 1; j < NUM_FLOORS; j++) { // floors
                                // finds first non-zero # of people on a floor that is not currently targeted
                                if (buildingState.floors[buildingState.elevators[i].currentFloor + j].numPeople != 0) {
                                    floorToGoTo = j + 48;
                                }
                            }
                            response = response.append("e");
                            response = response.append(1, iChar);
                            response = response.append("f");
                            response = response.append(1, floorToGoTo);
                            return response;
                        }
                        // if the elevator is on floors 1-8 inclusive
                        else {
                            int numPeopleAbove = buildingState.floors[buildingState.elevators[i].currentFloor + 1].numPeople;
                            int numPeopleBelow = buildingState.floors[buildingState.elevators[i].currentFloor - 1].numPeople;
                            char floorToGoTo = '\0';
                            
                            // if the number of people on floor above are non-zero + previously untargeted, send elevator
                            // if that floor is already being targeted, find the closest elevator
                                if (numPeopleAbove > numPeopleBelow && numPeopleBelow > 0) {
                                
                                floorToGoTo = (buildingState.elevators[i].currentFloor + 1) + 48;
                                }
                                // if more num people below + untargeted floor,
                                else if (numPeopleAbove <= numPeopleBelow && numPeopleBelow > 0) {
                                
                                    floorToGoTo = (buildingState.elevators[i].currentFloor - 1) + 48;
                                }
                                else {
                                    response = "";
                                    return response;
                                }
                            }
                            response = response.append("e");
                            response = response.append(1, iChar);
                            response = response.append("f");
                            response = response.append(1, floorToGoTo);
                            return response;
                        }
                    }
                }
            return response;
        }



string getAIPickupList(const Move& move, const BuildingState& buildingState,
                       const Floor& floorToPickup) {
    int i = move.getElevatorId();
    _Elevator currentElevator = buildingState.elevators[i];
    string peopleToPickUp = "";
    _Floor floor = buildingState.floors[currentElevator.currentFloor];
    int sumUpRequests = 0;
    string upRequests = "";
    int sumDownRequests = 0;
    string downRequests = "";
    
    //used for avg anger lvls
    int sumMostAngryUp = 0;
    int totalAngryUp = 0;
    int averageAngerUp = 0;
    int sumMostAngryDown = 0;
    int totalAngryDown = 0;
    int averageAngerDown = 0;
    
    //used for # people with anger lvls
    int angerAboveSevenUp = 0;
    int angerAboveFiveUp = 0;
    int angerAboveTwoUp = 0;
    int angerLowUp = 0;
    int angerAboveSevenDown = 0;
    int angerAboveFiveDown = 0;
    int angerAboveTwoDown = 0;
    int angerLowDown = 0;
    

    if (currentElevator.currentFloor <= 3) {
        for (int i = 0; i < floor.numPeople; ++i) {
            //checks for up request
            if (floorToPickup.getPersonByIndex(i).getCurrentFloor() < floorToPickup.getPersonByIndex(i).getTargetFloor()) {
                ++sumUpRequests;
                upRequests += char('0' + i);
            }
            else {
                ++sumDownRequests;
                downRequests += char('0' + i);
            }
        }
        
        if (sumUpRequests > sumDownRequests) {
            peopleToPickUp = upRequests;
        }
        //what if downRequests = upRequests??
        else {
            peopleToPickUp = downRequests;
        }
    }
    else if (currentElevator.currentFloor >= 6) {
        for (int i = 0; i < floor.numPeople; ++i) {
            //checks for down request
            if (floorToPickup.getPersonByIndex(i).getCurrentFloor() > floorToPickup.getPersonByIndex(i).getTargetFloor()) {
                ++sumDownRequests;
                downRequests += char('0' + i);
            }
            else {
                ++sumUpRequests;
                upRequests += char('0' + i);
            }
        }
        
        if (sumUpRequests > sumDownRequests) {
            peopleToPickUp = upRequests;
        }
        //what if downRequests = upRequests??
        else {
            peopleToPickUp = downRequests;
        }
    }
    //for floors 4 and 5
    else {
        //get num of up requests and stats
        for (int i = 0; i < floor.numPeople; ++i) {
            if (floorToPickup.getPersonByIndex(i).getCurrentFloor() < floorToPickup.getPersonByIndex(i).getTargetFloor()) {
                ++sumUpRequests;
                upRequests += char('0' + i);
                
                //get values to calc avg anger level up
                sumMostAngryUp += floor.people[i].angerLevel;
                ++totalAngryUp;
                
                //get values to calc highest num of people w highest anger lvls
                if (floor.people[i].angerLevel > 7) {
                    ++angerAboveSevenUp;
                }
                else if (floor.people[i].angerLevel > 5) {
                    ++angerAboveFiveUp;
                }
                else if (floor.people[i].angerLevel > 2) {
                    ++angerAboveTwoUp;
                }
                else {
                    ++angerLowUp;
                }
            }
        }
        
        //get num of down requests
        for (int i = 0; i < floor.numPeople; ++i) {
            if (floorToPickup.getPersonByIndex(i).getCurrentFloor() > floorToPickup.getPersonByIndex(i).getTargetFloor()) {
                ++sumDownRequests;
                downRequests += char('0' + i);
                
                //get values to calc avg anger level down
                sumMostAngryDown += floor.people[i].angerLevel;
                ++totalAngryDown;
                
                //get values to calc highest num of people w highest anger lvls
                if (floor.people[i].angerLevel > 7) {
                    ++angerAboveSevenDown;
                }
                else if (floor.people[i].angerLevel > 5) {
                    ++angerAboveFiveDown;
                }
                else if (floor.people[i].angerLevel > 2) {
                    ++angerAboveTwoDown;
                }
                else {
                    ++angerLowDown;
                }
            }
        }
        
        //if more up requests
        if (sumUpRequests > sumDownRequests) {
            peopleToPickUp = upRequests;
        }
        //if more down requests
        else if (sumUpRequests < sumDownRequests) {
            peopleToPickUp = downRequests;
        }
        //if equal focus on anger levels
        else {
            averageAngerUp = sumMostAngryUp / totalAngryUp;
            averageAngerDown = sumMostAngryDown / totalAngryDown;
            
            if (averageAngerUp > averageAngerDown) {
                peopleToPickUp = upRequests;
            }
            else if (averageAngerUp < averageAngerDown) {
                peopleToPickUp = downRequests;
            }
            else {
                if (angerAboveSevenUp > angerAboveSevenDown) {
                    peopleToPickUp = upRequests;
                }
                else if (angerAboveSevenUp < angerAboveSevenDown) {
                    peopleToPickUp = downRequests;
                }
                if (angerAboveFiveUp > angerAboveFiveDown) {
                    peopleToPickUp = upRequests;
                }
                else if (angerAboveFiveUp < angerAboveFiveDown) {
                    peopleToPickUp = downRequests;
                }
                if (angerAboveTwoUp > angerAboveTwoDown) {
                    peopleToPickUp = upRequests;
                }
                else if (angerAboveTwoUp < angerAboveTwoDown) {
                    peopleToPickUp = downRequests;
                }
                if (angerLowUp > angerLowDown) {
                    peopleToPickUp = upRequests;
                }
                else if (angerLowUp < angerLowDown) {
                    peopleToPickUp = downRequests;
                }
                //should i get more specific??
                else {
                    peopleToPickUp = upRequests;
                }
            }
        }
    }
    
    return peopleToPickUp;
}


