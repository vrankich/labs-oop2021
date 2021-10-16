#pragma once

#include <iostream> 
#include <exception>
#include <cstring> 
#include <limits> 

/* is key found in the table or not */
enum search { FAIL, SUCCESS, };

enum input {
	CRASH, 
	END_OF_FILE, 
	INVALID, 
	GOOD,
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
	Item(const Item&item);
	Item(Item &&item)
		: busy(item.busy) 
		, info(item.info) { item.info = nullptr; } 
	Item(const int _key, char *_info);
	~Item() noexcept { delete [] info; }
	Item& operator =(Item &&) noexcept;
	Item& operator =(const Item &) noexcept;
	/* comparison of items */
	bool operator ==(const Item &) noexcept;
	bool operator !=(const Item &) noexcept;
};

std::ostream& operator <<(std::ostream &, const Item &) noexcept;
std::istream& operator >>(std::istream &, std::pair<int, char*> &) noexcept;

class Table {
private:
	int m_size;
	int m_n;
	Item *m_table;
	void add(const Item &);
public:
	Table(int size = 0)
		: m_size(size)
		, m_n(0) {
			if (size > 0) { m_table = new Item[size]; }
			else if (size == 0) { m_table = nullptr; }
			else { throw invalid_size(); }
		}
	Table(const Table &);
	/* constructor from array of keys and info */
	Table(const std::pair<int, char*> *, int);
	Table(Table &&table)
		: m_size(table.m_size)
		, m_n(table.m_n)
		, m_table(table.m_table) { table.m_table = nullptr; }
	~Table() noexcept { delete [] m_table; }
	int get_size() const noexcept { return Table::m_size; }
	int get_n() const noexcept { return this->m_n; }
	//void set_n(int n) noexcept { m_n = n; }
	const Item *get_table() const noexcept { return m_table; }
	/* add new item to the table */
	void add(const std::pair<int, const char*> &);
	/* reorganize table */
	void refresh() noexcept;
	const char *search_info(int) const noexcept;
	search search_item(Item &, int) noexcept;
	search delete_item(int) noexcept;
	const Item& operator [](int) const;
	Table& operator =(const Table&) noexcept;
	Table& operator =(Table&&) noexcept;
	/* add items from the table passed to function */
	Table& operator +=(const Table &);
	/* delete all items from this table which are in the table passed to function*/
	Table& operator -=(const Table &) noexcept;
	/* new table from the items which are in the first and second table */
	friend Table operator +(const Table &, const Table &);
	/* delete all items from the first table which are in the second */
	friend Table operator -(const Table &, const Table &) noexcept;
	friend std::ostream& operator <<(std::ostream &, const Table &) noexcept;
	friend std::istream& operator >>(std::istream &, Table &) noexcept;
};


