#pragma once

#include <math.h> 
#include <string.h>
#include <stdio.h>
#include <exception> 
//#include <strstream> // ???

class invalid_radius: public std::exception {
	virtual const char* what() const throw();
};

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
//		MostDistantPoints(Point &p1, Point &p2)
//			: point1(p1)
//			, point2(p2) {}
	};

	struct Radius {
		double r, angle;
		Radius(double r_0 = 0, double angle_0 = 0)
			: r(r_0)
			, angle(angle_0) {}
	};

	class Cardioid {
	private:
		Point m_center;
		double m_r;
	public:
		Cardioid(double r = 1);
		~Cardioid() = default;
		Cardioid &set_r(double);
		inline double get_r() const { return this->m_r; }
		double polar_distance(const double &) const noexcept;
		MostDistantPoints most_distant_points() const noexcept;
		Radius *r_of_curvature() const noexcept;
		double area() const noexcept;
		double polar_arc_lenght(const double &) const noexcept;
	};
}

namespace heart = MathEquation;