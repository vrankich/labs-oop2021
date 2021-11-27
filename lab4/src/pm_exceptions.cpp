#include "pm_exceptions.h"

const char *conflicting_package::what() const throw()
{
	return "Package or its dependencie conflicts with another package";
}

const char *invalid_vector_size::what() const throw()
{
	return "Passed vector size doesn't match needed size";
}

const char *invalid_version::what() const throw()
{
	return "Some of the three parts of version number is invalid";
}