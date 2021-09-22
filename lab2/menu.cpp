#include "menu.h" 

const char *FUNCS[] = {"0. Quit",
					   "1. Calculate distance to the center in polar coordinate system",
                       "2. Get coordinates of points farthest from the cardioid axis",
                       "3. Get radii of curvature at characteristic points",
                       "4. Calculate area",
                       "5. Calculate arc length"};

const int FUNCS_SIZE = sizeof(FUNCS) / sizeof(FUNCS[0]);

int dialog(const char *funcs[], int n)
{
	const char *err = "";
	int choice;
	do {
		std::cout << err << std::endl;
		err = "\nIncorrect input...\n";
		for (int i = 0; i < n; i++) {
			std::cout << funcs[i] << std::endl;
		}
		std::cout << "\nMake a choice: ";
		choice = getchar() - '0';
		while (getchar() != '\n') {}
	} while (choice < 0 || choice >= n);
	return choice;
}

bool is_ivalid_radius(const double &r)
{
	return r <= 0;
}

bool is_invalid_andle(const double &angle)
{
	return angle < 0;
}

error get_value(bool (*cond)(const double &), const char *err_msg, 
	                         const char *msg, double &v)
{
	const char *err = "";
	do {
		std::cout << err << std::endl;
		std::cout << msg;
		std::cin >> v;
		err = err_msg;
		if (std::cin.bad()) { return INPUT_CRASH; }
		if (std::cin.eof()) { return END_OF_FILE; }
		if (std::cin.fail()) { 
			std::cin.clear(); 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue; 
		}
		std::cin.ignore(32767, '\n');
	} while (cond(v));
	return SUCCESS;
}

double degrees_to_radians(const double &angle)
{
	return (PI * angle) / 180;
}

error get_cardioid(heart::Cardioid &c)
{
	try {
		std::cout << "\n- NEW CARDIOID -\n";
		double r;
		error err_type = get_value(&is_ivalid_radius, "\nIvalid radius...\n", "Enter radius: ", r);
		if (err_type == SUCCESS) { c.set_r(r); }
		return err_type;
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		return INVALID_RADIUS;
	}
}

void menu()
{
	heart::Cardioid cardioid;
	if (get_cardioid(cardioid) != SUCCESS) {
		std::cout << "\nInvalid input or end of file...\n" << std::endl;
		return;
	}

	int c = 0;
	double angle;
	error err_type;
	do {
		c = dialog(FUNCS, FUNCS_SIZE);
		switch(c) {
			case 0:
				break;
			case 1:
				err_type = get_value(&is_invalid_andle, "\nInvalid input...\n", "Enter angle in degrees: ", angle);
				if (err_type == INPUT_CRASH || err_type == END_OF_FILE) {
					std::cout << "\nInvalid input or end of file...\n" << std::endl;
					c = 0;
					break;
				}
				std::cout << std::endl;
				std::cout << "Distance: " <<cardioid.polar_distance(angle) << std::endl;
				break;
			case 2:
			{
				heart::MostDistantPoints points = cardioid.most_distant_points();
				std::cout << "\n(x1, y1) = (" << points.point1.x << ", " << points.point1.y << ")";
				std::cout << "\n(x2, y2) = (" << points.point2.x << ", " << points.point2.y << ")";
				std::cout << std::endl;
				break;
			}
			case 3:
			{
				std::cout << std::endl;
				heart::Radius *radii = cardioid.r_of_curvature();
				for (int i = 0; i < 4; i++) {
					std::cout << std::fixed << std::setprecision(2);
					std::cout << "Radius of curvature for angle " << radii[i].angle << ": ";
					std::cout << radii[i].r << std::endl;
				}
				delete [] radii;
				break;
			}
			case 4:
				std::cout << std::endl;
				std::cout << "Area: " << cardioid.area() << std::endl;
				break;
			case 5:
				err_type = get_value(&is_invalid_andle, "\nInvalid input...\n", "Enter angle: ", angle);
				if (err_type == INPUT_CRASH || err_type == END_OF_FILE) {
					std::cout << "\nInvalid input or end of file...\n" << std::endl;
					c = 0;
					break;
				}
				std::cout << std::endl;
				std::cout << "Arc lenght: " << cardioid.polar_arc_lenght(angle) << std::endl;
				break;
		}
	} while (c != 0);
}
