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

Item::Item(int _key, const char *_info)
	: busy(1)
	, key(_key)
{
	int len = strlen(_info);
	if (len < N_CHAR - 1) {
		throw invalid_len();
	}
	memcpy(info, _info, len);
}

Table::Table(const std::pair<int, const char*> *arr_key_info, int n_pairs)
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

/* reorganize table */
void Table::refresh() noexcept
{
	if (this->m_n == 0) { return; }

	int n = m_n;

	// TODO: CHANGE
	for (int i = 0, j = 0; i < n; i++) {
		if (m_table[i].busy) {
			m_table[j++] = m_table[i];
		} else {
			m_n--;
		}
	}



	// OPTIMIZE !!! i and j
//	for (int i = 0; i < this->m_n; i++) {
//		if (this->m_table[i].busy == 0) {
//			/* empty place found */
//			for (int j = i; j < this->m_n - 1; j++) {
//				this->m_table[j] = this->m_table[j + 1];
//			}
//			this->m_table[--this->m_n].busy = 0;
//		}
//	}
}

//template <typename T>
//input input_value(std::istream &input, void (*f)(std::istream, T&), T &res)
//{
//	f(input, res);
//	if (input.bad()) { return CRASH; }
//	if (input.eof()) { return END_OF_FILE; }
//	if (input.fail()) { return INVALID; }
//	//input.ignore(32767, '\n');
//	//std::cin.clear(); 
//	//
//}

// MEMORY LEAK
input Table::input_item(std::ostream &output, std::istream &input)
{
	std::pair<int, char[N_CHAR]> key_info;
	output << "Enter key: ";
	input >>  key_info.first;
	if (input.bad()) { return CRASH; }
	if (input.eof()) { return END_OF_FILE; }
	if (input.fail()) { 
		input.clear();
		input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return INVALID; 
	}
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	/* input info */
	//key_info.second = new char[N_CHAR];
	output << "Enter information: ";
	input.get(key_info.second, N_CHAR - 1, '\n');
	if (input.bad()) { return CRASH; }
	if (input.eof()) { return END_OF_FILE; }
	if (input.fail()) { 
		input.clear();
		input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return INVALID; 
	}
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	add(key_info);
	//delete [] key_info.second;
	return GOOD;
}

std::ostream &Table::output_table(std::ostream &stream) const noexcept
{
	if (this->m_n == 0) {
		stream << "Empty table" << std::endl;
		return stream;
	}

	stream << std::endl;
	for (int i = 0; i < this->m_n; i++) {
		if (this->m_table[i].busy == 1) {
			stream << this->m_table[i].key << " -> " << this->m_table[i].info;
			stream << std::endl;
		}
	}
	return stream;
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