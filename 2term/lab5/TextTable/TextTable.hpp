#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace ng {

	std::string to_string(double value);

	class TextTable {
	private:
		// variables
		char _horizontal;
		char _vertical;
		char _corner;

		std::vector<std::string> _currentRow;
		std::vector<std::vector<std::string>> _rows;

		std::vector<int> _width;

		// private methods
		void _initWidth();

		[[nodiscard]] std::string _ruler() const;

	public:
		// constructor / destructor
		TextTable(char horizontal = '-', char vertical = '|', char corner = '+');
		virtual ~TextTable() = default;

		// public methods
		void add(const std::string& content);
		void endOfRow();

		void print(std::ostream& ostream = std::cout);

	}; // class TextTable

} // namespace ng