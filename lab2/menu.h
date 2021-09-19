#pragma once

#include <iostream>
#include  <climits> 
#include "cardioid.h" 

class invalid_input: public std::exception {
	virtual const char* what() const throw();
};

void menu();