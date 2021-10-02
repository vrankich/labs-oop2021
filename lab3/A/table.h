#pragma once

#include <iostream> 
#include <exception>
#include <cstring> 

#define N_ITEM 10
#define N_CHAR 6

enum search {
	FAIL = 0,
	SUCCESS = 1,
};

enum input {
	CRASH = 0,
	END_OF_FILE = 1,
	INVALID = 2,
	GOOD = 3,
};

class invalid_info_lenght: public std::exception {
	virtual const char *what() const throw();
};

class invalid_table_size: public std::exception {
	virtual const char *what() const throw();
};

class table_overflow: public std::exception {
	virtual const char *what() const throw();
};

struct Item {
	int busy;
	int key;
	char info[N_CHAR];
	Item()
		: busy(0) {}
	Item(const int _key, const char _info[]);
};

class Table {
private:
	static const int m_size = N_ITEM;
	static const int m_info_len = N_CHAR - 1;
	int m_n;
	Item m_table[N_ITEM];
	void add(Item);
	void refresh() noexcept;
public:
	Table()
		: m_n(0) {}
	Table(const std::pair<int, char*> *, const int);
	int get_size() const noexcept { return this->m_size; }
	int get_n() const noexcept { return this->m_n; }
	const Item *get_table() const noexcept { return this->m_table; }
	input input_item(std::ostream &, std::istream &, FILE *);
	std::ostream &output_table(std::ostream &) const noexcept;
	search search_item(Item &, const int) noexcept;
	search search_info(char *&, const int, const int);
	search delete_item(const int) noexcept;
};