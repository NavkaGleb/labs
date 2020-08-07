#include "TextTable.hpp"

#include <iostream>
#include <iomanip>

std::string ng::to_string(double value) {

	std::string result(std::to_string(value));
	result.erase(result.find_last_not_of('0') + (result.find_last_not_of('0') == result.find('.') ? 0 : 1), std::string::npos);

	return result;

}

// private methods
void ng::TextTable::_initWidth() {

	this->_width.assign(this->_rows[0].size(), 0);

	for (const auto& row : this->_rows) {

		for (int i = 0; i < row.size(); i++)
			this->_width[i] = this->_width[i] > row[i].size() ? this->_width[i] : row[i].size();

	}

}

std::string ng::TextTable::_ruler() const {

	std::string result(1, this->_corner);

	for (const auto& width : this->_width)
		result += std::string(width, this->_horizontal) + this->_corner;

	return result;

}

// constructor / destructor
ng::TextTable::TextTable(char horizontal, char vertical, char corner) :
	_horizontal(horizontal), _vertical(vertical), _corner(corner) {


}

// public methods
void ng::TextTable::add(const std::string& content) {

	this->_currentRow.emplace_back(content);

}

void ng::TextTable::endOfRow() {

	this->_rows.emplace_back(this->_currentRow);
	this->_currentRow.clear();

}

void ng::TextTable::print(std::ostream& ostream) {

	this->_initWidth();

	ostream << this->_ruler() << "\n";

	for (const auto& row : this->_rows) {

		ostream << this->_vertical;

		for (int i = 0; i < row.size(); i++) {

			ostream << std::setw(this->_width[i]) << std::right << row[i];
			ostream << this->_vertical;

		}

		ostream << "\n";
		ostream << this->_ruler() << "\n";

	}

}