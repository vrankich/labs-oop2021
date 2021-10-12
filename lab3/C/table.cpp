#include "table.h"

const char *equal_key::what() const throw()
{
	return "Key is already in the table";
}

const char *invalid_index::what() const throw()
{
	return "Invalid index";
}

const char *invalid_size::what() const throw()
{
	return "Invalid size";
}

Item::Item(int _key, char *_info)
	: busy(1)
	, key(_key)
{
	int len = strlen(_info);
	info = new char[len + 1];
	memcpy(info, _info, len + 1);
}

Item::~Item() 
{ 
	if (info) {
		delete [] info;
	} 
}

Item& Item::operator =(Item &&item)
{
	if (this == &item) { return *this; }

	if (item.busy == 0) {
		this->busy = 0;
		return *this;
	}

	if (this->info) {	
		delete [] this->info;
		this->info = nullptr;
	}

	this->busy = 1;
	this->key = item.key;
	this->info = item.info;
	item.info = nullptr;

	return *this;
}

Item& Item::operator =(const Item &item)
{
	if (this == &item) { return *this; }

	if (item.busy == 0) {
		this->busy = 0;
		return *this;
	}

	if (this->info) {	
		delete [] this->info;
		this->info = nullptr;
	}

	this->busy = 1;
	this->key = item.key;
	if (item.info) {
		int len = strlen(item.info);
		this->info = new char[len + 1];
		memcpy(this->info, item.info, len + 1);
	}

	return *this;
}

bool Item::operator ==(const Item &item) noexcept
{
	if (this->busy == 0 || item.busy == 0) { return false; }
	if	(this->key != item.key) { return false; }
	if	(strcmp(this->info, item.info) != 0) { return false; }
	return true;
}

bool Item::operator !=(const Item &item) noexcept
{
	return !(*this == item);
}

/* copying constructor */
Table::Table(const Table &table)
{
	m_size = table.m_size;
	m_n = table.m_n;
	m_table = new Item[m_size];
	for(int i = 0; i < m_n; i++) {
		if (table.m_table[i].busy) {
			m_table[i].busy = 1;
			m_table[i].key = table.m_table[i].key;
			if (table.m_table[i].info) {
				int len = strlen(table.m_table[i].info);
				m_table[i].info = new char[len + 1];
				memcpy(m_table[i].info, table.m_table[i].info, len + 1);
			} else {
				m_table[i].info = nullptr;
			}
		}
	}
}

Table::Table(const std::pair<int, char*> *arr_key_info, int n_pairs)
{
	m_n = m_size = n_pairs;
	m_table = new Item[m_size];
	
	int len;
	for (int i = 0; i < n_pairs; i++) {
		len = strlen(arr_key_info[i].second);
		m_table[i].busy = 1;
		m_table[i].key = arr_key_info[i].first;
		m_table[i].info = new char[len + 1];
		memcpy(this->m_table[i].info, arr_key_info[i].second, len + 1);
	}
}

Table::~Table() noexcept
{
	if (m_table) {
		delete [] m_table;
	}
}

void Table::add(const std::pair<int, const char*> &key_info)
{
	for (int i = 0; i < m_n; i++) {
		if (m_table[i].busy && m_table[i].key == key_info.first) {
			throw equal_key();
		}
	}

	if (m_n == m_size) {
		refresh();
	}

	/* if  there is no free space in the table after refreshing - reallocate */
	if (m_n == m_size) {
		if (m_size == 0) {
			m_table = new Item[++m_size];
		} else {
			Item *temp = new Item[++m_size];
			for(int i = 0; i < m_n; i++) {
				temp[i] = m_table[i];
			}
			delete [] m_table;
			m_table = temp;
			temp = nullptr;
		}
	}

	/* add item to n-th place */
	m_table[m_n].busy = 1;
	m_table[m_n].key = key_info.first;
	if (key_info.second) {
		int len = strlen(key_info.second);
		m_table[m_n].info = new char[len + 1];
		memcpy(m_table[m_n].info, key_info.second, len + 1);
	} else {
		m_table[m_n].info = nullptr;
	}
	m_n++;
}

void Table::add(const Item &new_item)
{
	for (int i = 0; i < m_n; i++) {
		if (m_table[i].busy == 1 && m_table[i].key == new_item.key) {
			throw equal_key();
		}
	}
	if (m_n == m_size) {
		refresh();
	}

	/* if  there is no free space in the table after refreshing - reallocate */
	if (m_n == m_size) {
		if (m_size == 0) {
			m_table = new Item[++m_size];
		} else {
			Item *temp = new Item[++m_size];
			for(int i = 0; i < m_n; i++) {
				temp[i] = m_table[i];
			}
			delete [] m_table;
			m_table = temp;
			temp = nullptr;
		}
	}

	/* add item to n-th place */
	if (new_item.busy) {
		m_table[m_n].busy = 1;
		m_table[m_n].key = new_item.key;
		if (new_item.info) {
			int len = strlen(new_item.info);
			m_table[m_n].info = new char[len + 1];
			memcpy(m_table[m_n].info, new_item.info, len + 1);
		} else {
			m_table[m_n].info = nullptr;
		}
		m_n++;
	} else {
		m_table[m_n++].busy = 0;
	}
}

/* reorganize table */
void Table::refresh() noexcept
{
	if (this->m_n == 0) { return; }

	int n = m_n;
	for (int i = 0, j = 0; i < n; i++) {
		if (m_table[i].busy) {
			m_table[j++] = m_table[i];
		} else {
			m_n--;
		}
	}
	/* set to zero elements after m_n */
	for (int i = m_n; i < n; i++) {
		m_table[i].busy = 0;
		delete [] m_table[i].info;
		m_table[i].info = nullptr;
	}
}

const char *Table::search_info(int _key) const noexcept
{
	/* empty table */
	if (m_n == 0) { return nullptr; }

	for (int i = 0; i < m_n; i++) {
		if (m_table[i].key == _key && m_table[i].busy == 1) {
			return m_table[i].info;
		}
	}
	/* key is not found */
	return nullptr;
}

search Table::search_item(Item &_item, int _key) noexcept
{
	/* empty table */
	if (m_n == 0) { return FAIL; }

	for (int i = 0; i < this->m_n; i++) {
		if (m_table[i].busy == 1 && m_table[i].key == _key ) {
			_item = m_table[i];
			return SUCCESS;
		}
	}
 	/* key is not found */
	return FAIL;
}

search Table::delete_item(int _key) noexcept
{
	/* empty table */
	if (m_n == 0) { return FAIL; }

	for (int i = 0; i < m_n; i++) {
		if (m_table[i].key == _key && m_table[i].busy == 1) {
			/* free field */
			m_table[i].busy = 0;
			return SUCCESS;
		}
	}
	/* key is not found */
	return FAIL;
}

/* not safe to change Item */
const Item& Table::operator []  (int index) const
{
	if (index >= m_size) {
		throw invalid_index();
	}
	return m_table[index];
}

Table& Table::operator =(const Table &table) noexcept
{
	if (this == &table) { return *this; }

	if (m_table) { 
		delete [] m_table;
	}
	m_size = table.m_size;
	m_n = table.m_n;
	m_table = new Item[table.m_size];
	for (int i = 0; i < table.m_n; i++) {
		m_table[i] = table.m_table[i];
	}

	return *this;
}

Table& Table::operator =(Table &&table) noexcept
{
	if (this == &table) { return *this; }

	if (m_table) {
		delete [] m_table;
	}
	m_size = table.m_size;
	m_n = table.m_n;
	m_table = table.m_table;
	table.m_table = nullptr;
	return *this;
}

/* prefix operator for refreshing
 * just for practice (not really practical for real life programming) */
Table& Table::operator --() noexcept
{
	refresh();
	return *this;
}

/* postfix operator for refreshing*/
const Table Table::operator --(int) noexcept
{
	Table temp_table(*this);
	refresh();
	return temp_table;
}

Table operator + (const Table &table1, const Table &table2)
{	
	Table res(table1.m_n + table2.m_n);
	for (int i = 0; i < table1.m_n; i++) {
		res.add(table1.m_table[i]);
	}
	for (int i = table1.m_n; i < res.m_size; i++) {
		res.add(table2.m_table[i - table1.m_n]);
	}
	return res;
}

Table& operator += (Table &table1, const Table &table2)
{
	int n = table1.m_n + table2.m_n;
	if (n > table1.m_size) {
		Item *temp = table1.m_table;
		table1.m_table = new Item[n];
		table1.m_size = n;
		delete [] temp;
	}
	for (int i = table1.m_n; i < table2.m_n; i++) {
		table1.add(table2.m_table[i - table1.m_n]);
	}
	return table1;
}

/* throws exception in case of table overflow */
Table& operator += (Table &table, const Item &item)
{
	table.add(item);
	return table;
}

/* delete all elements from the first table which are in the second table */
Table operator - (const Table &table1, const Table &table2) noexcept
{
	if (table2.get_n() == 0) { return table1; }

	Table res;
	bool is_in_table;
	for (int i = 0; i < table1.m_n; i++) {
		is_in_table = false;
		for (int j = 0; j < table2.m_n; j++) {
			if (table1.m_table[i] == table2.m_table[j]) {
				is_in_table = true;
				break;
			}
		}
		if (!is_in_table) {
			res.add(table1.m_table[i]);
		}
	}
	return res;
}

Table& operator -= (Table &table1, const Table &table2) noexcept
{
	if (table2.get_n() == 0) { return table1; }

	for (int i = 0; i < table2.m_n; i++) {
		for (int j = 0; j < table1.m_n; j++) {
			if (table1.m_table[j] == table2.m_table[i]) {	
				/* remove item */
				table1.m_table[j].busy = 0;
			}
		}
	}
	return table1;
}

std::ostream& operator << (std::ostream &output, const Table &table) noexcept
{
	if (table.m_n == 0) {
		output << "Empty table" << std::endl;
		return output;
	}
	for (int i = 0; i < table.m_n; i++) {
		output << table.m_table[i];
	}
	return output;
}

std::istream& operator >> (std::istream &input, Table &table) noexcept
{
	for (int i = 0; i < table.m_n; i++) {
		input >> table.m_table[i];
	}
	return input;
}

std::ostream& operator << (std::ostream &output, const Item &item) noexcept
{
	if (item.busy == 1) {
		output << item.key << " -> " << item.info;
		output << std::endl;
	}
	return output;
}

std::istream& operator >>(std::istream &input, char *&res)
{
	int buf_size = 81;
	char buf[buf_size], *temp;
	int len = 0, chunk_len, str_len;
	
	do {
		input.get(buf, buf_size, '\n');
		if (!input.good()) {
			return input;
		} else if (input.gcount() > 0) {
			chunk_len = strlen(buf);
			str_len = len + chunk_len;
			temp = res;
			res = new char[str_len + 1];
			memcpy(res, temp, len);
			memcpy(res + len, buf, chunk_len);
			len = str_len;
			delete [] temp;
		} else {
			input.clear();
			input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	} while(input.gcount() == buf_size - 1);
	
	if (len > 0) {
		res[len] = '\0';
	} else {
		res = new char[1];
	}
	
	return input;
}

std::istream& operator >>(std::istream &input, Item &item) noexcept
{
	item.busy = 1;
	input >> item.key;
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	input >> item.info;
	if (!input.good()) {
		item.busy = 0;
	}

	return input;
}

