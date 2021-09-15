#pragma once

#include <iostream>
#include <exception> 

namespace SparseMatrix 
{
	struct Element {
		int col;
		double data;
		Element *next;
	};

	struct Row {
		int index;
		int not_zero;
		Element *first;
		Row *next;
	};

	struct Matrix {
		int n_rows;
		int n_col;
		Row *first;
	};

	inline bool is_greater(double a, double b)
	{
		return a > b;
	}

	inline bool is_less(double a, double b)
	{
		return a < b;
	}

	template <typename T>
	int get_num(T &n) 
	{
		std::cin >> n;
		if (!std::cin.good()) {
			return -1;
		}
		return 1;
	}

	void delete_matrix(Matrix *&);
	Matrix *get_matrix();
	void change_matrix(Matrix *&);
	const void print_matrix(const void (*)(Matrix *&), const char *, Matrix *&) noexcept;
	const void output(const char *, Matrix *&) noexcept;
}

namespace sm = SparseMatrix;
