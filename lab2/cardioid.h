#pragma once

#include <math.h> 
#include <string.h>
#include <stdio.h>
#include <exception> 
//#include <strstream> // ???
//#include  "gtest/gtest_prod.h"

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

	class Cardioid {
	private:
		Point m_center;
		double m_r;
	public:
		Cardioid(double r = 1);
		Cardioid(const Point &p, double r = 1);
		Cardioid(double x_0, double y_0, double r = 1);
		~Cardioid() = default;
		Cardioid &set_center(const Point &);
		Cardioid &set_r(double);
		Point get_center() const;
		double get_r() const;
		double polar_distance(double) const;
		Point *most_distant_points() const;
		double **r_of_curvature() const;
		double area() const;
		double polar_arc_lenght(const double) const;
	};
}

namespace heart = MathEquation;