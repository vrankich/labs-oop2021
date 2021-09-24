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

	/* return points, farthest from the cardioid axis */
	void Cardioid::most_distant_points(Point &p1, Point &p2) const noexcept
	{
		p1.x = p2.x = (3 / 4) * this->m_r;
		p1.y = (3 * sqrt(3) * this->m_r) / 4;
		p2.y = - (3 * sqrt(3) * this->m_r) / 4;
	}

	/* return radii of curvature at characteristic points of cardioid */
	void Cardioid::radii_of_curvature(double &r1, double &r2, double &r3) const noexcept
	{
	 	r1 = (4 / 3) * this->m_r; 
	 	r2 = r3 = 0;
	}
}