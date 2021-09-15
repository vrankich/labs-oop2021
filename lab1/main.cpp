#include "prog1.h" 

int main()
{
	sm::Matrix *matrix = sm::get_matrix();
	sm::print_matrix("Matrix from input:\n", matrix);
	
	sm::make_new_matrix(matrix);
	sm::print_matrix("New matrix with sorted row:\n", matrix);

	sm::delete_matrix(matrix);

	return 0; 
}