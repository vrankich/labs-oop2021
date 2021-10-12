#include "table.h"

const char *invalid_table_size::what() const throw()
{
	return "Invalid table size";
}

const char *table_overflow::what() const throw()
{
	return "Table is full";
}

const char *equal_key::what() const throw()
{
	return "Key is already in the table";
}

const char *invalid_len::what() const throw()
{
	return "Invalid information lenght";
}

const char *invalid_index::what() const throw()
{
	return "Invalid index";
}

Item::Item(int _key, const char *_info)
	: busy(1)
	, key(_key)
{
	int len = strlen(_info);
	if (len > N_CHAR - 1) {
		throw invalid_len();
	}
	memcpy(info, _info, len + 1);
}

/* copying constructor */
Table::Table(const Table &table) noexcept
{
	this->m_n = table.m_n;
	for (int i = 0; i < m_n; i++) {
		this->m_table[i] = table.m_table[i];
	}
}

Table::Table(const std::pair<int, char*> *arr_key_info, int n_pairs)
{
	if (n_pairs > Table::m_size) {
		throw invalid_table_size();
	}

	int len;
	m_n = n_pairs;
	for (int i = 0; i < n_pairs; i++) {
		len = strlen(arr_key_info[i].second);
		if (len < m_info_len) {
			throw invalid_len();
		}
		this->m_table[i].busy = 1;
		this->m_table[i].key = arr_key_info[i].first;
		memcpy(this->m_table[i].info, arr_key_info[i].second, len);
	}
}

void Table::add(const std::pair<int, const char*> &key_info)
{
	int len = strlen(key_info.second);
	if (len > m_info_len) {
		throw invalid_len();
	}
	if (m_n == m_size) {
		refresh();
	}
	/* if  there is no free space in the table after refreshing - throw exception */
	if (m_n == m_size) {
		throw table_overflow();
	}
	for (int i = 0; i < m_n; i++) {
		if (m_table[i].busy && m_table[i].key == key_info.first) {
			throw equal_key();
		}
	}
	/* add item to n-th place */
	m_table[m_n].busy = 1;
	m_table[m_n].key = key_info.first;
	memcpy(m_table[m_n].info, key_info.second, len + 1);
	m_n++;
}

void Table::add(const Item &new_item)
{
	if (m_n == m_size) {
		refresh();
	}
	/* if  there is no free space in the table after refreshing - throw exception */
	if (m_n == m_size) {
		throw table_overflow();
	}
	for (int i = 0; i < m_n; i++) {
		if (m_table[i].busy && m_table[i].key == new_item.key) {
			throw equal_key();
		}
	}
	/* add item to n-th place */
	m_table[m_n++] = new_item;
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
	}
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

search Table::search_info(char *res, int _key, int len)
{
	if (len < Table::m_info_len) {
		throw invalid_len();
	}
	/* empty table */
	if (this->m_n == 0) { return FAIL; }

	for (int i = 0; i < this->m_n; i++) {
		if (this->m_table[i].key == _key && this->m_table[i].busy == 1) {
			memcpy(res, this->m_table[i].info, N_CHAR);
			return SUCCESS;
		}
	}
	/* key is not found */
	return FAIL;
}

search Table::delete_item(int _key) noexcept
{
	/* empty table */
	if (this->m_n == 0) { return FAIL; }

	for (int i = 0; i < this->m_n; i++) {
		if (this->m_table[i].key == _key && this->m_table[i].busy == 1) {
			/* free field */
			this->m_table[i].busy = 0;
			return SUCCESS;
		}
	}
	/* key is not found */
	return FAIL;
}

/* not safe to change Item */
const Item& Table::operator []  (int index) const
{
	if (index > m_size) {
		throw invalid_index();
	}
	return m_table[index];
}

/* prefix operator for refreshing
 * just for practice (not really practical for real life programming) */
Table& Table::operator --() noexcept
{
	this->refresh();
	return *this;
}

/* postfix operator for refreshing*/
const Table Table::operator --(int) noexcept
{
	Table temp_table(*this);
	this->refresh();
	return temp_table;
}

Table operator + (const Table &table1, const Table &table2)
{
	if (table1.m_n + table2.m_n > N_ITEMS) {
		throw table_overflow();
	}
	
	Table res;
	for (int i = 0; i < table1.m_n; i++) {
		res.add(table1.m_table[i]);
	}
	for (int i = table1.m_n; i < table1.m_n + table2.m_n; i++) {
		res.add(table2.m_table[i - table1.m_n]);
	}
	return res;
}

Table& operator += (Table &table1, const Table &table2)
{
	if (table1.m_n + table2.m_n > N_ITEMS) {
		throw table_overflow();
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

std::istream& operator >> (std::istream &input, Item &item) noexcept
{
	item.busy = 1;
	input >> item.key;
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	input.get(item.info, N_CHAR - 1, '\n');
	return input;
}

bool operator == (const Item &item1, const Item &item2) noexcept
{
	if (item1.busy == 0 || item2.busy == 0) { return false; }
	if	(item1.key != item2.key) { return false; }
	if	(strcmp(item1.info, item2.info) != 0) { return false; }
	return true;
}

bool operator != (const Item &item1, const Item &item2) noexcept
{
	return !(item1 == item2);
}
