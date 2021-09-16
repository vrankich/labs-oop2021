#include "menu.h" 

const char *invalid_input::what() const throw()
{
	return "Invalid input or end of file";
}

const char *FUNCS[] = {"0. Quit",
                                          "1. Calculate distance to the center in polar coordinate system",
                                          "2. Get coordinates of points farthest from the cardioid axis",
                                          "3. Get radius of curvature at characteristic points",
                                          "4. Calculate area",
                                          "5. Calculate arc length"};

const int FUNCS_SIZE = sizeof(FUNCS) / sizeof(FUNCS[0]);

int dialog(const char *funcs[], int n)
{
	const char *err = "";
	int choice;
	do {
		std::cout << err << std::endl;
		err = "Incorrect input...";
		for (int i = 0; i < n; i++) {
			std::cout << funcs[i] << std::endl;
		}
		std::cout << "\nMake a choice: ";
		choice = getchar() - '0';
		while (getchar() != '\n') {}
	} while (choice < 0 || choice >= n);
	return choice;
}

double *get_radius(double &r)
{
	const char *err = "";
	do {
		std::cout << err << std::endl;
		std::cout << "Enter radius: ";
		std::cin >> r;
		err = "Enter a positive real number...";
		if (!std::cin.good()) {
			return nullptr;
		}
	} while (r <= 0);
	return &r;
}

double degrees_to_radians(const double &angle)
{
	return (M_PI * angle) / 180;
}

double *get_angle(double &radians)
{
	int angle;	
	const char *err = "";
	do {
		std::cout << err << std::endl;
		std::cout << "Enter angle in degrees: ";
		std::cin >> angle;
		err = "Incorrect input...";
		if (!std::cin.good()) {
			throw invalid_input();
			return nullptr;
		}
	} while (angle < INT_MIN || angle > INT_MAX);
	radians = degrees_to_radians(angle);
	return &radians;
}

heart::Point *get_point(const char *msg, heart::Point &p)
{
	std::cout << msg;
	std::cout << "x coordinate ---> ";
	std::cin >> p.x;
	if (!std::cin.good()) {
		return nullptr;
	}
	std::cout << "y coordinate ---> ";
	std::cin >> p.y;
	if (!std::cin.good()) {
		return nullptr;
	}
	return &p;
}

heart::Cardioid *get_cardioid(heart::Cardioid &c)
{
	try {
		double r;
		heart::Point p;
		if (!get_point("\nEnter coordinates of a center:\n", p)) {
			throw invalid_input();
		}
		if (!get_radius(r)) {
			throw invalid_radius();
		}
		c.set_center(p);
		c.set_r(r);
		return &c;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

void menu()
{
	heart::Cardioid cardioid;
	if (!get_cardioid(cardioid)) {
		return;
	}

	double angle;
	heart::Radius *radii = nullptr;
	int c = 0;
	do {
		c = dialog(FUNCS, FUNCS_SIZE);
		switch(c) {
			case 0:
			// quit
				break;
			case 1:
				if (!get_angle(angle)) {
					return;
				}
				std::cout << std::endl;
				std::cout << cardioid.polar_distance(angle) << std::endl;
				break;
			case 2:

				break;
			case 3:
				radii = cardioid.r_of_curvature();
				for (int i = 0; i < 4; i++) {
					std::cout << "Radius of curvature for " << radii[i].angle << ": ";
					std::cout << radii[i].r << std::endl;
				}
				delete [] radii;
				break;
			case 4:
				std::cout << std::endl;
				std::cout << cardioid.area() << std::endl;
				break;
			case 5:
				if (!get_angle(angle)) {
					return;
				}
				std::cout << std::endl;
				std::cout << cardioid.polar_arc_lenght(angle) << std::endl;
				break;
		}
	} while (c != 0);
}