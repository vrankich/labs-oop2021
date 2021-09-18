#include "tests.h" 

void test_constructors()
{
	heart::Cardioid cardioid1(5);
	assert(cardioid1.get_r() == 5);
	
} 
