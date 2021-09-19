#include "cardioid.h" 

const char *invalid_radius::what() const throw()
{
	return "\nInvalid radius\n";
}

namespace MathEquation 
{
	Cardioid::Cardioid(double r)
	 {
			if (r < 0) {
				throw invalid_radius();
			}
			this->m_r = r;
		}

	Cardioid& Cardioid::set_r(double r)
	{
		if (r < 0) {
			throw invalid_radius();
		}
		this->m_r = r;
		return *this;
	}

	const double calculate_r_polar(const double &r, const double &angle)
	{
		return 2 * r * (1 - cos(angle));
	}

	/* Вернуть расстояние до центра в полярной системе координат
	 * в зависимости от угла для точки принадлежащей кардиоиде */
	double Cardioid::polar_distance(const double &angle) const noexcept
	{
		return 2 * this->m_r * (1 + cos(angle));
	}

	/* Вернуть координаты наиболее удаленных от оси кардиоиды точек */
	MostDistantPoints Cardioid::most_distant_points() const noexcept
	{
		MostDistantPoints points;
		points.point1.x = (3 * this->m_r) / 2;
		points.point1.y = 0;
		points.point2.x = - (3 * this->m_r) / 2;
		points.point2.y = 0;
		return points;
	}

	const double calculate_r_of_curvature(const double &r, const double &angle) noexcept
	{
		return (8 / 3) * r * sin(angle / 2);
	}

	/* Вернуть радиуса кривизны в характерных точках кардиоиды */
	Radius *Cardioid::r_of_curvature() const noexcept
	{
		Radius *r_arr = new Radius[4];
		
		r_arr[0].angle = 0;
		r_arr[1].angle = M_PI / 6;
		r_arr[2].angle = M_PI / 4;
		r_arr[3].angle = M_PI / 3;
		
		r_arr[0].r = calculate_r_of_curvature(this->m_r, r_arr[0].angle);
		r_arr[1].r = calculate_r_of_curvature(this->m_r, r_arr[1].angle);
		r_arr[2].r = calculate_r_of_curvature(this->m_r, r_arr[2].angle);
		r_arr[3].r = calculate_r_of_curvature(this->m_r, r_arr[3].angle);	
	
		return r_arr;
	}

	/* Вернуть площадь описываемую кардиоидой */
	double Cardioid::area() const noexcept
	{
		return 6 * M_PI * this->m_r;
	}

	/* Вернуть длину дуги кардиоиды в зависимости от угла полярного радиуса */
	double Cardioid::polar_arc_lenght(const double &angle) const noexcept
	{
		double r_polar = calculate_r_polar(this->m_r, angle);
		return 16 * r_polar * pow(sin(angle / 2), 2);
	}
}