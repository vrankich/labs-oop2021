#pragma once

#include <cmath> 
#include <exception> 

class invalid_radius: public std::exception {
	virtual const char* what() const throw();
};

constexpr double PI = 3.14159265358979;

namespace MathEquation 
{
	struct Point {
		double x, y;
		Point(double x_0 = 0, double y_0 = 0)
			: x(x_0)
			, y(y_0) {}
	};

	class Cardioid {
	private:
		double m_r;
	public:
		Cardioid(double r = 1);
		~Cardioid() = default;
		void set_r(const double r) { r < 0 ? throw invalid_radius() : this->m_r = r; } /* setter */
		double get_r() const noexcept { return this->m_r; } /* getter */
		double polar_distance(const double angle) const noexcept 
			{ return this->m_r * (1 + fabs(cos(angle))); }
		void most_distant_points(Point &, Point &) const noexcept;
		void radii_of_curvature(double &, double &, double &) const noexcept;
		double area() const noexcept { return (3.0 / 2) * PI * this->m_r * this->m_r; }
		double arc_lenght(const double angle) const noexcept 
			{ return fabs(4 * this->m_r * sin(angle / 2)); }
	};
}

namespace heart = MathEquation;