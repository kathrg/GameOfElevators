# Project 5 — Elevators
## Description
Program entails the operation of three elevators within a multi-story building where individuals spawn with varying anger levels. The primary objective of the program is to facilitate the efficient movement of individuals throughout the building, with the aim of maximizing satisfaction amongst spawned occupants. Player can play on their own or watch programmed AI play.

## Legend and Specifics
The game mechanics of this program entail a discrete representation of time: one "tick" represents one unit of time passing. The game commences at a tick value of zero and concludes upon reaching 99 ticks (this duration can be modified). Within the game, a small "o" icon represents an individual, and their level of anger is displayed as a numerical value ranging from 0 to 9. This value increases by 1 for every 5 ticks elapsed, and once an anger level of 10 is reached, the individual explodes.

Additionally, the program uses the symbols "U" and "D" to indicate the direction of travel desired by spawned individuals. The specific direction of travel for each individual is not provided. Furthermore, it is not allowed to simultaneously pick up individuals traveling in both directions, and once an elevator is set in motion, it cannot stop until it reaches its designated destination.

## Moves
* Pass move (hit [enter])
* Pickup move: e#p (such as e1p)
* Service move: e#f# (f represents desired target floor, such as "e2f9")
* Game move: s or q (to save or quit, respecively)

## Functions and Files Implemented
* Functions: all functions above "DO NOT MODIFY ANY CODE BENEATH THIS LINE" in move.cpp, game.cpp, building.cpp, floor.cpp, elevator.cpp, and person.cpp.
* Files: ai.cpp.
* _Note: All other files, functions, and RMEs were provided for me by the University of Michigan EECS 183 and are subject to copyright laws._

## Scores
Scores gained from the programmed AI using the various .in files (to test, change string variable NEW_FILENAME under utility.cpp to one of the provided .in files)
* new.in: 818
* eveningMellowGame.in: 438
* morningNormalGame.in: 802
* morningPeacefulGame.in: 655
* randomGame.in: 421
* randomShortGame.in: 543

## Language Used
* C++

## Environment Used
* Xcode on macOS Monterey (version 12.5)
