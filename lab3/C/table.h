#pragma once

#include <iostream> 
#include <exception>
#include <cstring> 
#include <limits> 

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

class equal_key: public std::exception {
	virtual const char *what() const throw();
};

class invalid_index: public std::exception {
	virtual const char *what() const throw();
};

class invalid_size: public std::exception {
	virtual const char *what() const throw();
};

struct Item {
	int busy;
	int key;
	char *info;
	Item() 
		: busy(0) 
		, info(nullptr) {}
	Item(const int _key, char *_info);
	virtual ~Item();
	Item& operator =(Item &&);
	Item& operator =(const Item &);
	bool operator ==(const Item &) noexcept;
	bool operator !=(const Item &) noexcept;
};

class Table {
private:
	int m_size;
	int m_n;
	Item *m_table;
	void add(const Item &);
	void delete_table() noexcept;
public:
	Table(int size = 0)
		: m_size(size)
		, m_n(0) {
			if (size > 0) { m_table = new Item[size]; }
			else if (size == 0) { m_table = nullptr; }
			else { throw invalid_size(); }
		}
	Table(const Table &);
	Table(const std::pair<int, char*> *, int);
	Table(Table &&table)
		: m_size(table.m_size)
		, m_n(table.m_n)
		, m_table(table.m_table) { table.m_table = nullptr; }
	virtual ~Table() noexcept;
	int get_size() const noexcept { return Table::m_size; }
	int get_n() const noexcept { return this->m_n; }
	const Item *get_table() const noexcept { return m_table; }
	void add(const std::pair<int, const char*> &);
	void refresh() noexcept;
	const char *search_info(int) const noexcept;
	search search_item(Item &, int) noexcept;
	search delete_item(int) noexcept;
	const Item& operator [](int) const;
	Table& operator =(const Table&) noexcept;
	Table& operator =(Table&&) noexcept;
	Table& operator --() noexcept;
	const Table operator --(int) noexcept;
	friend Table operator +(const Table &, const Table &);
	friend Table& operator +=(Table &, const Table &);
	friend Table& operator +=(Table &, const Item &);
	friend Table operator -(const Table &, const Table &) noexcept;
	friend Table& operator -=(Table &, const Table &) noexcept;
	friend std::ostream& operator <<(std::ostream &, const Table &) noexcept;
	friend std::istream& operator >>(std::istream &, Table &) noexcept;
};

std::istream& operator >>(std::istream &, char *&);
std::ostream& operator <<(std::ostream &, const Item &) noexcept;
std::istream& operator >>(std::istream &, Item &) noexcept;


