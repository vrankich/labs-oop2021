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

	struct MostDistantPoints {
		Point point1;
		Point point2;
	};

	struct Radius {
		double r, angle;
		Radius(double r_0 = 0, double angle_0 = 0);
	};

	class Cardioid {
	private:
		double m_r;
	public:
		Cardioid(double r = 1);
		~Cardioid() = default;
		inline void set_r(const double r) { r < 0 ? throw invalid_radius() : this->m_r = r; }
		inline double get_r() const { return this->m_r; }
		inline double polar_distance(const double &angle) const noexcept 
			{  return 2 * this->m_r * (1 + cos(angle)); }
		MostDistantPoints most_distant_points() const noexcept;
		Radius *r_of_curvature() const noexcept; 
		inline double area() const noexcept { return 6 * PI * this->m_r; }
		double polar_arc_lenght(const double &) const noexcept;
	};
}

namespace heart = MathEquation;