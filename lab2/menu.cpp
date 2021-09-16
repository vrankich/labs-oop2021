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
			throw invalid_input();
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