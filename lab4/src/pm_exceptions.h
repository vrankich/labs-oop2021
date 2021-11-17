#pragma once

#include <exception> 

class conflicting_package: public std::exception {
	virtual const char *what() const throw();
};