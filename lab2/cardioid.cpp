#include "cardioid.h" 

const char *invalid_radius::what() const throw()
{
	return "Invalid radius";
}

namespace MathEquation 
{
	Cardioid::Cardioid(double r)
		: m_center(0, 0) {
			if (r < 0) {
				throw invalid_radius();
			}
			this->m_r = r;
		}

	Cardioid::Cardioid(const Point &p, double r)
		: m_center(p) {
			if (r < 0) {
				throw invalid_radius();
			}
			this->m_r = r;
		}

	Cardioid::Cardioid(double x_0, double y_0, double r)
		: m_center(x_0, y_0) {
			if (r < 0) {
				throw invalid_radius();
			}
			this->m_r = r;
		}

	Cardioid& Cardioid::set_center(const Point &p)
	{
		this->m_center = p;
		return *this;
	}

	Cardioid& Cardioid::set_r(double r)
	{
		if (r < 0) {
			throw invalid_radius();
		}
		this->m_r = r;
		return *this;
	}

	Point Cardioid::get_center() const
	{
		return this->m_center;
	}

	double Cardioid::get_r() const
	{
		return this->m_r;
	}

	const double calculate_angle(const Point &p)
	{
		return atan(p.y / p.x);
	}

//	double calculate_r_polar(const double r, const Point &p)
//	{
//		return 2 * r * (1 - cos(calculate_angle(p)));
//	}

	const double calculate_r_polar(const double r, double angle)
	{
		return 2 * r * (1 - cos(angle));
	}

	/* Вернуть расстояние до центра в полярной системе координат
	 * в зависимости от угла для точки принадлежащей кардиоиде */
	double Cardioid::polar_distance(double angle) const
	{
		return 2 * this->m_r * (1 + cos(angle));
	}


	/* Вернуть координаты наиболее удаленных от оси кардиоиды точек */
	Point *Cardioid::most_distant_points() const
	{
		Point *points = nullptr;

		return points;
	}

	const double calculate_r_of_curvature(double r, double angle)
	{
		return (8 / 3) * r * sin(angle / 2);
	}

	// TABLE
	/* Вернуть радиуса кривизны в характерных точках кардиоиды */
	double *Cardioid::r_of_curvature() const
	{
		double *r_arr = new double[4];
		r_arr[0] = calculate_r_of_curvature(this->m_r, 0);
		r_arr[1] = calculate_r_of_curvature(this->m_r, M_PI / 6);
		r_arr[2] = calculate_r_of_curvature(this->m_r, M_PI / 4);
		r_arr[3] = calculate_r_of_curvature(this->m_r, M_PI / 3);
		return r_arr;
	}

	/* Вернуть площадь описываемую кардиоидой */
	double Cardioid::area() const
	{
		return 6 * M_PI * this->m_r;
	}

	/* Вернуть длину дуги кардиоиды в зависимости от угла полярного радиуса */
	double Cardioid::polar_arc_lenght(const double angle) const
	{
		double r_polar = calculate_r_polar(this->m_r, angle);
		return 16 * r_polar * pow(sin(angle / 2), 2);
	}
}