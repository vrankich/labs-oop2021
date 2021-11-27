#pragma once

#include <exception> 

class conflicting_package: public std::exception {
	virtual const char *what() const throw();
};

class invalid_vector_size: public std::exception {
	virtual const char *what() const throw();
};

class invalid_version: public std::exception {
	virtual const char *what() const throw();
};
