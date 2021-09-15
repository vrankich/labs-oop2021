#include "prog1.h" 

int main()
{
	sm::Matrix *matrix = sm::get_matrix();
	sm::output("Matrix from input:\n", matrix);

<<<<<<< HEAD
	sm::change_matrix(matrix);
=======
	sm::make_new_matrix(matrix);
>>>>>>> 88be4e22a3c77e9bb9a116d43616eab7564e081b
	sm::output("New matrix with sorted row:\n", matrix);

	sm::delete_matrix(matrix);

	return 0; 
}