#include "physics.h" 

using namespace Dynamic;

const char *invalid_dimension::what() const throw()
{
	return "Vector should be three-dimensional";
}

double vector_len(const Vector &v)
{
	double res = 0;
	for (unsigned int i = 0; i < v.getCurSize(); i++) {
		res += pow(v[i], 2);
	}
	return sqrt(res);
}

namespace ElectricCharge {
	std::ostream& coulomb_force(std::ostream &out, double q1, double q2, const Vector &r)
	{
		if (r.getCurSize() > 3) {
			throw invalid_dimension();
		}

		Vector force = k * q1 * q2 / pow(vector_len(r), 3) * r;
		double value = k * fabs(q1) * fabs(q2) / pow(vector_len(r), 2);
		out << "--- electric charge ---" << std::endl;
		out << "Coulomb force: " << std::endl;
		out << "q1 = " << q1 << ", q2 = " << q2 << std::endl;
		out << "r = { " << r << "}" << std::endl;
		out << "F = { " << force << "}" << std::endl;
		out << "F = " << value << std::endl;

		return out;
	}

	std::ostream& electric_field(std::ostream &out, double q, const Vector &r) 
	{
		if (r.getCurSize() > 3) {
			throw invalid_dimension();
		}

		Vector field = k * q / pow(vector_len(r), 3) * r;
		double value = k * fabs(q) / pow(vector_len(r), 2);
		out << "--- electric charge ---" << std::endl;
		out << "Electric field: " << std::endl;
		out << "q = " << q << std::endl;
		out << "r = { " << r << "}" << std::endl;
		out << "E = { " << field << "}" << std::endl;
		out << "E = " << value << std::endl;

		return out;
	}

	std::ostream &potential(std::ostream &out, double q, const Vector &r)
	{
		if (r.getCurSize() > 3) {
			throw invalid_dimension();
		}

		double value = k * fabs(q) / vector_len(r);
		out << "--- electric charge ---" << std::endl;
		out << "Potential: " << std::endl;
		out << "q = " << q << std::endl;
		out << "r = { " << r << "}" << std::endl;
		out << "value = " << value << std::endl;

		return out;
	}

	std::ostream& potential_energy(std::ostream &out, double q1, double q2, const Vector &r)
	{
		if (r.getCurSize() > 3) {
			throw invalid_dimension();
		}

		double value = k * q1 * q2 / vector_len(r);
		out << "--- electric charge ---" << std::endl;
		out << "Potential energy: " << std::endl;
		out << "q1 = " << q1 << ", q2 = " << q2 << std::endl;
		out << "r = { " << r << "}" << std::endl;
		out << "W = " << value << std::endl;

		return out;
	}
}

namespace Dipole {
	std::ostream& coulomb_force(std::ostream &out, double q1, double q2, const Vector &r)
	{
		if (r.getCurSize() > 3) {
			throw invalid_dimension();
		}

		Vector force = k * q1 * q2 / pow(vector_len(r), 3) * r;
		double value = k * fabs(q1) * fabs(q2) / pow(vector_len(r), 2);
		out << "--- dipole ---" << std::endl;
		out << "Coulomb force: " << std::endl;
		out << "q1 = " << q1 << ", q2 = " << q2 << std::endl;
		out << "r = { " << r << "}" << std::endl;
		out << "F = { " << force << "}" << std::endl;
		out << "F = " << value << std::endl;

		return out;
	}

	std::ostream& electric_field(std::ostream &out, const Vector &p, const Vector &r)
	{
		if (p.getCurSize() > 3 || r.getCurSize() > 3) {
			throw invalid_dimension();
		}
	
		double r_len = vector_len(r);
		Vector field = (-k) * p / pow(r_len, 3) + 3 * k * ((p * r) * r) / pow(r_len, 5);
		out << "--- dipole ---" << std::endl;
		out << "Electric field: " << std::endl;
		out << "p = { " << p << "}" << std::endl;
		out << "r = { " << r << "}" << std::endl;
		out << "E = { " << field << "}" << std::endl;
		out << "E = " << vector_len(field) << std::endl;

		return out;
	}
	
	std::ostream& potential(std::ostream &out, const Vector &p, const Vector &r)
	{
		if (p.getCurSize() > 3 || r.getCurSize() > 3) {
			throw invalid_dimension();
		}
	
		double value = k * (p * r) / pow(vector_len(r), 3);
		out << "--- dipole ---" << std::endl;
		out << "Potential: " << std::endl;
		out << "p = { " << p << "}" << std::endl;
		out << "r = { " << r << "}" << std::endl;
		out << "value = " << value << std::endl;

		return out;
	}
	
	std::ostream& potential_energy(std::ostream &out, const Vector &p, const Vector &r)
	{
		if (p.getCurSize() > 3 || r.getCurSize() > 3) {
			throw invalid_dimension();
		}

		double r_len = vector_len(r);
		Vector E = (-k) * p / pow(r_len, 3) + 3 * k * ((p * r) * r) / pow(r_len, 5);
		double value = - (E * p);
		out << "--- dipole ---" << std::endl;
		out << "Potential energy: " << std::endl;
		out << "p = { " << p << "}" << std::endl;
		out << "r = { " << r << "}" << std::endl;
		out << "W = " << value << std::endl;

		return out;
	}
}