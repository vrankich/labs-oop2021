#include "prog1.h"

namespace SparseMatrix
{
	Element *new_el(int index, double num) 
	{
		try {
			Element *el = new Element;
			el->col = index;
			el->data = num;
			el->next = nullptr;
			return el;
		} catch (std::bad_alloc &e) {
			std::cout << "Not enough memory for allocation...\n"
							<< e.what() << std::endl;
			return nullptr;
		}
	}

	Row *new_row(int i) 
	{
		try {
			Row *line = new Row;
			line->index = i;
			line->not_zero = 0;
			line->first = nullptr;
			line->next = nullptr;
			return line;
		} catch (std::bad_alloc &e) {
			std::cout << "Not enough memory to alocate...\n"
							<< e.what() << std::endl;
			return nullptr;
		}
	}

	Matrix *new_matrix(int m, int n)
	{
		try {
			Matrix *arr = new Matrix;
			arr->n_rows = m;
			arr->n_col = n;
			arr->first = nullptr;
			return arr;
		} catch (std::bad_alloc &e) {
			std::cout << "Not enough memory to allocate...\n"
							<< e.what() << std::endl;
			return nullptr;
		}
	}

	void delete_matrix_helper(Row *&row)
	{
		if (!row) {
			return;
		}
	
		Element *el = row->first, *temp;
		while (el) {
			temp = el;
			el = el->next;
			delete temp;
			temp = nullptr;
		}
		delete row;
		row = nullptr;
	}

	void delete_matrix(Matrix *&arr)
	{
		if (!arr) {
			return;
		}
	
		Row *row = arr->first, *temp;
		while (row) {
			temp = row;
			row = row->next;
			delete_matrix_helper(temp);
		}
		delete arr;
		arr = nullptr;
	}

	int *get_index(const char *msg, int &n)
	{
		const char *err_msg = "";
		do {
			std::cout << err_msg << std::endl;
			std::cout << msg;
			err_msg = "\nIndex shoud be a natural number...\n";
			if (get_num(n) < 0) {
				return nullptr;
			} 
		}	while (n < 1);
		return &n;
	}

	int get_row(Row *&row, int n_col)
	{
		if (!row) {
			return -1;
		}
	
		try {
			int num;
			Element *ptr ;
			for (int i = 0; i < n_col; i++) {
				if (get_num(num) < 0) {
						return -1;
				}
				/* For zeros memory should not be allocated */
				if (num) {
					if (!row->first) {
						row->first = new_el(i, num);
						if (!row->first) {
							return -1;
						}
						ptr = row->first;
					} else {
						ptr->next = new_el(i, num);
						if (!ptr->next) {
							return -1;
						}
						ptr = ptr->next;
					}
					row->not_zero++;
				}
			}
		} catch (std::bad_alloc &e) {
			std::cout << "Not enough memory for allocation...\n"
							<< e.what() << std::endl;
			return -1;
		} 
	
		return 1;
	}

	Matrix *get_matrix()
	{
		std::cout << "\nNEW SPARSE MATRIX\n";
		int n, m ;
		if (!get_index("Enter number of rows: ", m) ||
			!get_index("Enter number of columns: ", n)) {
			return nullptr;
		}

		Matrix *arr = new_matrix(m, n);
		if (!arr) {
			return nullptr;
		}
		
		Row *ptr = nullptr, *temp = nullptr;
		for (int i = 0; i < m; i++) {
			temp = new_row(i);
			if (!temp) {
				return nullptr;
			}
			std::cout << "\nEnter elements for row [" << i + 1 << "]:\n";
			if (get_row(temp, arr->n_col) < 0) {
				delete_matrix(arr);
				delete_matrix_helper(temp);
				return nullptr;
			}
			if (!temp->first) {
				delete temp;
			} else {
				if (!arr->first) {
					arr->first = temp;
					ptr = arr->first;
				} else {
					ptr->next = temp;
					ptr = ptr->next;
				}
			}
		 }
		
		return arr;
	}

	const double sum_in_row(Row *&row)
	{
		if (!row) {
			return 0;
		}
	
		double sum = 0;
		Element *ptr = row->first;
		while (ptr) {
			sum += ptr->data;
			ptr = ptr->next;
		}
		return sum;
	}

	/* Returns pointer to the row with max sum */
	Row *find_max_sum(Matrix *&arr)
	{
		if (!arr || !arr->first) {
			return nullptr;
		}
		
		double sum;
		double max_sum = sum_in_row(arr->first);
		Row *res = arr->first;
		Row *ptr = arr->first->next;
		
		while (ptr) {
			sum = sum_in_row(ptr);
			if (sum > max_sum) {
				max_sum = sum;
				res = ptr;
			}
			ptr = ptr->next;
		}
		
		return res;
	}

	/* Changes old row to new sorted row */
	void make_row_from_vector(Row *&row, double *&arr, int n, int i, int first_index)
	{
		if (!row || !row->first) {
			return;
		}
	
		Element *ptr = row->first;
		for (int i = 0; i < n && ptr; i++) {
			ptr->data = arr[i];
			ptr->col = first_index++;
			ptr = ptr->next;
		}
	}

	void swap (double &n1, double &n2)
	{
		double temp = n1;
		n1 = n2;
		n2 = temp;
	}

	/* Quick sort for array */
	void sort(double *arr, int left, int right, bool (*comp)(double, double))
	{
		int wall = left - 1;
		if (left >= right) {
			return;
		}
		for (int i = left; i <= right - 1; i++) {
			if (comp(arr[i], arr[right])) {
				wall++;
				swap(arr[wall], arr[i]);
			}
		}
		wall++;
		swap(arr[wall], arr[right]);
		sort(arr, left, wall - 1, comp);
		sort(arr, wall + 1, right, comp);
	}

	void sort_row(Row *&row, int n)
	{
		if (!row || !row->first) {
			return;
		}

		double *temp_arr = new double[n];
		
		Element *ptr = row->first;
		for (int i = 0; ptr; i++) {
			temp_arr[i] = ptr->data;
			ptr = ptr->next;
		}
		
		if (row->first->col <= 0) {
			sort(temp_arr, 0, row->not_zero - 1, &is_less);
			make_row_from_vector(row, temp_arr, row->not_zero, row->index, n - row->not_zero);
		} else {
			sort(temp_arr, 0, row->not_zero - 1, &is_greater);
			make_row_from_vector(row, temp_arr, row->not_zero, row->index, 0);
		}
		
		delete [] temp_arr;
	}

	/* Sorts row with max sum of elements */
	void change_matrix(Matrix *&arr)
	{
		if (!arr) {
			return;
		}
	
		Row *max_sum_row = find_max_sum(arr);
		if (!max_sum_row) {
			return;
		}
		sort_row(max_sum_row, arr->n_col);
	}

	const void print_with_zeros(const Matrix *arr)
	{
		Row *ptr_row = arr->first;
		Element *ptr_el = nullptr;

		for (int i = 0; i < arr->n_rows; i++) {
			if (!ptr_row || i != ptr_row->index) {	
				for (int j = 0; j < arr->n_col; j++) {
					std::cout << "0 | ";
				}
			} else {
				ptr_el = ptr_row->first;
				for (int j = 0; j < arr->n_col; j++) {
					if (ptr_el && ptr_el->col == j) {
						std::cout << ptr_el->data << " | ";
						ptr_el = ptr_el->next;
					} else {
						std::cout << "0 | ";
					}
				}
				ptr_row = ptr_row->next;
			}
			std::cout << std::endl;
		}
	}
	
	const void print_without_zeros(const Matrix *arr)
	{
		if (!arr->first) {
			std::cout << "Matrix does not have nonzero elements...\n";
		}

		Row *ptr_row = arr->first;
		Element *ptr_el = nullptr;

		while (ptr_row) {
			ptr_el = ptr_row->first;
			while (ptr_el) {
				std::cout << "[" << ptr_row->index + 1 << "]";
				std::cout << "[" << ptr_el->col + 1<< "]: ";
				std::cout << ptr_el->data << std::endl;
				ptr_el = ptr_el->next;
			}
			ptr_row = ptr_row->next;
			std::cout << std::endl;
		}
	}

	const void print_matrix(const void (*print)(const Matrix *), const char *msg, const Matrix *arr) noexcept
	{
		if (!arr) {
			std::cout << "\nEmpty matrix\n\n";
			return;
		}
		std::cout << std::endl << msg << std::endl;
		print(arr);
		std::cout << std::endl;
	}

	const void output(const char *msg, const Matrix *arr) noexcept
	{
		if (!arr) {
			std::cout << "\nEmpty matrix\n";
			return;
		}
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
			print_matrix(print_with_zeros, msg, arr);
		} else {
			print_matrix(print_without_zeros, msg, arr);
		}
	}
}
