#include "pm_exceptions.h"

const char *conflicting_package::what() const throw()
{
	return "Package or its dependencie conflicts with another package";
}