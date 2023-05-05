/*
 * Copyright 2023 University of Michigan EECS183
 *
 * test.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * Final Project - Elevators
 */
 
////////////////////////////////////////////////
// You will submit test.cpp to the autograder //
// FOR LAB 8, but NOT the final project Core  //
////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include "AI.h"
#include "Building.h"
#include "BuildingState.h"
#include "Elevator.h"
#include "Floor.h"
#include "Game.h"
#include "Move.h"
#include "Person.h"
#include "SatisfactionIndex.h"
#include "Utility.h"

using namespace std;

void test_person();
void test_floor();
// declare your test functions here

void start_tests() {
    // call your test functions here
    test_person();
    test_floor();
    
    return;
}

void test_person() {
    
    
}

void test_floor() {
    Person first("0f0t10a10");
    Person second("0f0t1a3");
    Floor();
    Floor one;
    
    one.addPerson(first, 10);
    one.addPerson(second, 1);
    
    
}

// write test functions here
