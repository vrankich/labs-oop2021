#pragma once

#include <iostream>
#include <cstring> 
#include <limits> 
#include "physics.h" 

class closed_file: public std::exception {
	virtual const char* what() const throw();
};

enum type { EL_CHARGE, DIPOLE, QUIT, };

void menu();