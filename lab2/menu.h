#pragma once

#include <iostream>
#include  <climits> 
#include "cardioid.h" 

class invalid_input: public std::exception {
	virtual const char* what() const throw();
};

//struct Angle {
//	int degrees, min, sec;
//	Angle(int d = 0, int m = 0, int s = 0)
//		: degrees(d)
//		, min(m)
//		, sec(s) {}
//};

void menu();