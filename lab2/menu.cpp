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
		std::cout << std::endl << err << std::endl;
		err = "Incorrect input...";
		for (int i = 0; i < n; i++) {
			std::cout << funcs[i] << std::endl;
		}
		std::cout << "Make a choice: ";
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
	return true;
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
	return (PI * std::abs(angle)) / 180;
}

error get_cardioid(heart::Cardioid &c)
{
	try {
		std::cout << "\n- NEW CARDIOID -\n";
		double r;
		error err_type = get_value(&is_ivalid_radius, "Ivalid radius...", "Enter radius: ", r);
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
		std::cout << "Invalid input or end of file..." << std::endl;
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
				err_type = get_value(&is_invalid_andle, "Invalid input...", "Enter angle in degrees: ", angle);
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
				heart::Point point1, point2;
				cardioid.most_distant_points(point1, point2);
				std::cout << "\n(x1, y1) = (" << point1.x << ", " << point1.y << ")";
				std::cout << "\n(x2, y2) = (" << point2.x << ", " << point2.y << ")";
				std::cout << std::endl;
				break;
			}
			case 3:
				double r_of_curv1, r_of_curv2, r_of_curv3;
				cardioid.radii_of_curvature(r_of_curv1, r_of_curv2, r_of_curv3);
				std::cout << std::endl;
				std::cout << std::fixed << std::setprecision(2);
				std::cout << "Radius of curvature #1: " << r_of_curv1 << std::endl;
				std::cout << "Radius of curvature #2: " << r_of_curv2 << std::endl;
				std::cout << "Radius of curvature #3: " << r_of_curv3 << std::endl;
				break;
			case 4:
				std::cout << std::endl;
				std::cout << "Area: " << cardioid.area() << std::endl;
				break;
			case 5:
				err_type = get_value(&is_invalid_andle, "Invalid input...", "Enter angle: ", angle);
				if (err_type == INPUT_CRASH || err_type == END_OF_FILE) {
					std::cout << "Invalid input or end of file..." << std::endl;
					c = 0;
					break;
				}
				std::cout << std::endl;
				std::cout << "Arc lenght: " << cardioid.arc_lenght(angle) << std::endl;
				break;
		}
	} while (c != 0);
}
