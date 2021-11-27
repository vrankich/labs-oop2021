#pragma once

#include <iostream> 
#include  "../pm_exceptions.h"

struct Version {
	unsigned major;
	unsigned minor;
	unsigned patch;
	
	Version(unsigned maj = 0, unsigned min = 0, unsigned p = 1)
		: major(maj)
		, minor(min)
		, patch(p) {}
	Version(const Version&);
	
	Version& set(unsigned, unsigned, unsigned);
	Version& operator =(const Version&);
	Version& operator ++() noexcept;
	Version operator ++(int) noexcept;
	friend std::ostream& operator <<(std::ostream&, const Version&);
	friend std::istream& operator >>(std::istream&, Version&);
};

enum class package_operations {
	NULL_P,
	EXISTS_IN_PM,
	NOT_IN_PM,
	CONFLICT,
	ADDED,
	REMOVED,
	NOT_REMOVED,
	INSTALLED,
	NOT_FOUND,
};


