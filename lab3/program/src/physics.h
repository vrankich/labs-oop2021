#pragma once

#include <iostream> 
#include <fstream>
#include <cmath> 
#include "../lib/Vector.h"
#include "../lib/auxFuncs.h"

using namespace Dynamic;

constexpr double k = 1.3E23;

class invalid_dimension: public std::exception {
	virtual const char* what() const throw();
};

namespace ElectricCharge {
	std::ostream& coulomb_force(std::ostream &out, double q1, double q2, const Vector &r);
	std::ostream& electric_field(std::ostream &out, double q, const Vector &r); 
	std::ostream &potential(std::ostream &out, double q, const Vector &r);
	std::ostream& potential_energy(std::ostream &out, double q1, double q2, const Vector &r);
}

namespace ech = ElectricCharge;

namespace Dipole {
	std::ostream& coulomb_force(std::ostream &out, double q1, double q2, const Vector &r);
	std::ostream& electric_field(std::ostream &out, const Vector &p, const Vector &r);
	std::ostream& potential(std::ostream &out, const Vector &p, const Vector &r);
	std::ostream& potential_energy(std::ostream &out, const Vector &p, const Vector &r);
}

namespace dip = Dipole;
