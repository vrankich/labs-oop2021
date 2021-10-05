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

Item::Item(const int _key, char _info[N_CHAR])
	: busy(1)
	, key(_key)
{
	memcpy(info, _info, N_CHAR);
}

Table::Table(const std::pair<int, char[N_CHAR]> *arr_key_info, const int n_pairs)
{
	if (n_pairs > Table::m_size) {
		throw invalid_table_size();
	}

	this->m_n = n_pairs;
	for (int i = 0; i < n_pairs; i++) {
		this->m_table[i].busy = 1;
		this->m_table[i].key = arr_key_info[i].first;
		memcpy(this->m_table[i].info, arr_key_info[i].second, N_CHAR);
	}
}

void Table::get_table(Item *table, int size)
{
	if (size < Table::m_size) {
		throw invalid_table_size();
	}

	for (int i = 0; i < Table::m_size; i++) {
		table[i] = this->m_table[i];
	}
}

void Table::add(Item new_item)
{
	if (this->m_n == Table::m_size) {
		this->refresh();
	}
	/* if  there is no free space in the table after refreshing - throw exception */
	if (this->m_n == Table::m_size) {
		throw table_overflow();
	}
	for (int i = 0; i < this->m_n; i++) {
		if (this->m_table[i].busy && this->m_table[i].key == new_item.key) {
			throw equal_key();
		}
	}
	/* add item to n-th place */
	this->m_table[this->m_n].busy = 1;
	this->m_table[this->m_n++] = new_item;
}

/* reorganize table */
void Table::refresh() noexcept
{
	if (this->m_n == 0) { return; }

	for (int i = 0; i < this->m_n; i++) {
		if (this->m_table[i].busy == 0) {
			/* empty place found */
			for (int j = i; j < this->m_n - 1; j++) {
				this->m_table[j] = this->m_table[j + 1];
			}
			this->m_table[--this->m_n].busy = 0;
		}
	}
}

input Table::input_item(std::ostream &output, std::istream &input, FILE *f_input)
{
	Item new_item;
	new_item.busy = 1;
	output << "Enter key: ";
	input >>  new_item.key;
	if (input.bad()) { return CRASH; }
	if (input.eof()) { return END_OF_FILE; }
	if (input.fail()) { return INVALID; }
	input.ignore(32767, '\n');
	output << "Enter information: ";

	/* input info */
	char buf[Table::m_info_len + 1];
	fgets(buf, Table::m_info_len, f_input);
	/* if EOF or reached buffer length is smaller that info length - return */
	if (feof(f_input)) { return END_OF_FILE; }
	input.ignore(32767, '\n');
	memcpy(new_item.info, buf, Table::m_info_len);

	this->add(new_item);
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

search Table::search_item(Item &_item, const int _key) noexcept
{
	/* empty table */
	if (this->m_n == 0) { return FAIL; }

	for (int i = 0; i < this->m_n; i++) {
		if (this->m_table[i].busy == 1 && this->m_table[i].key == _key ) {
			_item = this->m_table[i];
			return SUCCESS;
		}
	}
 	/* key is not found */
	return FAIL;
}

search Table::search_info(char *res, const int _key, const int len)
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

search Table::delete_item(const int _key) noexcept
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