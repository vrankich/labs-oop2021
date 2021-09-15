#include "menu.h" 

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

const char *invalid_input::what() const throw()
{
	return "Invalid input or end of file";
}

const char *eof_exception::what() const throw()
{
	return "Invalid input or end of file";
}

double *get_positive_number(const char *msg, double &num)
{
	const char *err = "";
	do {
		std::cout << err << std::endl;
		std::cout << msg;
		std::cin >> num;
		err = "Enter a positive real number...";
		if (std::cin.eof()) {
			throw eof_exception();
			return nullptr;
		}
		if (!std::cin.good()) {
			throw invalid_input();
			return nullptr;
		}
	} while (num <= 0);
	return &num;
}

double *get_radius(double &r)
{
	if (!get_positive_number("Enter radius: ", r)) {
		return nullptr;
	}
	return &r;
}

double degrees_to_radians(double angle)
{
	return (M_PI * angle) / 180;
}

double *get_angle(double &radians)
{
	double angle;
	if (!get_positive_number("Enter angle in degrees: ", angle)) {
		return nullptr;
	}
	radians = degrees_to_radians(angle);
	return &radians;
}

void menu()
{
	int c = 0;
	do {
		c = dialog(FUNCS, FUNCS_SIZE);
		switch(c) {
			case 0:
			// quit
				break;
			case 1:

				break;
			case 2:

				break;
			case 3:

				break;
			case 4:

				break;
			case 5:

				break;
		}
	} while (c != 0);
}