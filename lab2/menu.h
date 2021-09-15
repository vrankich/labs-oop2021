#pragma once

#include <iostream> 
#include "cardioid.h" 

class invalid_input: public std::exception {
	virtual const char* what() const throw();
};

class eof_exception: public std::exception {
	virtual const char* what() const throw();
};

void menu();