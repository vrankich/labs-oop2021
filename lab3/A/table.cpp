#include "table.h"

const char *invalid_info_lenght::what() const throw()
{
	return "Invalid lenght of information";
}

const char *invalid_table_size::what() const throw()
{
	return "Invalid table size";
}

const char *table_overflow::what() const throw()
{
	return "Table is full";
}

void Table::add(Item new_item)
{
	if (this->m_n == this->m_size) {
		this->refresh();
	}
	/* if  there is no free space in the table after refreshing - throw exception */
	if (this->m_n == this->m_size) {
		throw table_overflow();
	}
	/* add item to n-th place */
	this->m_table[this->m_n++] = new_item;
}

void Table::refresh() noexcept
{

}

Table::Table(const std::pair<int, char*> *arr_key_info, const int n_pairs)
{
	if (n_pairs > this->m_size) {
		throw invalid_table_size();
	}

	int len;
	for (int i = 0; i < this->m_size; i++) {
		this->m_table[i].busy = 1;
		this->m_table[i].key = arr_key_info[i].first;
		len = strlen(arr_key_info[i].second);
		if (len != this->m_info_len) {
			throw invalid_info_lenght();
		}
		memcpy(this->m_table[i].info, arr_key_info[i].second, this->m_info_len);
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
	output << "Enter information: ";

	/* input info */
	char *buf = new  char(this->m_info_len + 1);
	fgets(buf, this->m_info_len, f_input);
	/* if EOF or reached buffer length is smaller that info length - return */
	if (feof(f_input)) { return END_OF_FILE; }
	if (strlen(buf) != this->m_info_len) { return INVALID; }
	memcpy(new_item.info, buf, this->m_info_len);
	delete buf;

	this->add(new_item);
	return GOOD;
}

std::ostream &Table::output_table(std::ostream &stream) const noexcept
{
	if (this->m_n == 0) {
		stream << "Empty table" << std::endl;
		return stream;
	}

	for (int i = 0; i < this->m_size; i++) {
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

	for (int i = 0; i < N_ITEM; i++) {
		if (this->m_table[i].key == _key && this->m_table[i].busy == 1) {
			_item = this->m_table[i];
			return SUCCESS;
		}
	}
 	/* key is not found */
	return FAIL;
}

search Table::search_info(char *&res, const int _key, const int len)
{
	/* empty table */
	if (this->m_n == 0) { return FAIL; }

	for (int i = 0; i < this->m_size; i++) {
		if (this->m_table[i].key == _key && this->m_table[i].busy == 1) {
			/* if info lenght doesn't mach - throw exception */
			if (len != this->m_info_len) {
				throw invalid_info_lenght();
			}
			res = this->m_table[i].info;
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

	for (int i = 0; i < this->m_size; i++) {
		if (this->m_table[i].key == _key && this->m_table[i].busy == 1) {
			/* free field */
			this->m_table[i].busy = 0;
			return SUCCESS;
		}
	}
	/* key is not found */
	return FAIL;
}