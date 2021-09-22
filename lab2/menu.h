#pragma once

#include <iostream>
#include  <climits>
#include <limits> 
#include <iomanip> 
#include "cardioid.h" 

enum error {
	INVALID_INPUT = 0,
	INPUT_CRASH = 1,
	END_OF_FILE = 2,
	INVALID_RADIUS = 3,
	SUCCESS = 4,
};

void menu();