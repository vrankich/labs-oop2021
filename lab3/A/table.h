#pragma once

#include <iostream> 
#include <exception>
#include <cstring> 
#include <limits> 

enum search { FAIL, SUCCESS };

enum input {
	CRASH,
	END_OF_FILE,
	INVALID,
	GOOD,
};

class invalid_table_size: public std::exception {
	virtual const char *what() const throw();
};

class table_overflow: public std::exception {
	virtual const char *what() const throw();
};

class equal_key: public std::exception {
	virtual const char *what() const throw();
};

class invalid_len: public std::exception {
	virtual const char *what() const throw();
};

inline const int N_ITEMS = 5;
inline const int N_CHAR = 6;

struct Item {
	int busy;
	int key;
	char info[N_CHAR];
	Item() 
		: busy(0) {}
	Item(const int _key, const char *_info);
};

class Table {
private:
	static const int m_size = N_ITEMS;
	static const int m_info_len = N_CHAR - 1;
	int m_n;
	Item m_table[N_ITEMS];
public:
	Table() noexcept
		: m_n(0) {}
	Table(const std::pair<int, char*> *, int);
	int get_size() const noexcept { return Table::m_size; }
	int get_info_len() const noexcept { return Table::m_info_len; }
	int get_n() const noexcept { return this->m_n; }
	const Item *get_table() const noexcept { return m_table; }
	void add(const std::pair<int, const char*> &);
	void refresh() noexcept;
	input input_item(std::ostream &, std::istream &);
	std::ostream &output_table(std::ostream &) const noexcept;
	search search_item(Item &, int) noexcept;
	search search_info(char *, int, int);
	search delete_item(int) noexcept;
};
