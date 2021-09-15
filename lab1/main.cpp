#include "prog1.h" 

const void output(const char *msg, sm::Matrix *&matrix)
{
	std::cout << "\nChoose output of matrix:\n" << std::endl;
	std::cout << "1. Print matrix with zeros" << std::endl;
	std::cout << "2. Print matrix without zeros" << std::endl;
	int choice;
	const char *err = "";
	do {
		std::cout << err << std::endl;
		err = "\nIncorrect choice...\nTry again\n";
		if (sm::get_num(choice) < 0) {
			return;
		}
	} while (choice != 1 && choice != 2);
	if (choice == 1) {
		sm::print_matrix(&sm::print_with_zeros, msg, matrix);
	} else {
		sm::print_matrix(&sm::print_without_zeros, msg, matrix);
	}
}

int main()
{
	sm::Matrix *matrix = sm::get_matrix();
	output("Matrix from input:\n", matrix);

	sm::make_new_matrix(matrix);
	output("New matrix with sorted row:\n", matrix);

	sm::delete_matrix(matrix);

	return 0; 
}