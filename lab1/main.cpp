#include "sparse_matrix.h" 

int main()
{
	sm::Matrix *matrix = sm::get_matrix();
	sm::output("Matrix from input:\n", matrix);

	sm::change_matrix(matrix);
	sm::output("New matrix with sorted row:\n", matrix);

	sm::delete_matrix(matrix);

	return 0; 
}