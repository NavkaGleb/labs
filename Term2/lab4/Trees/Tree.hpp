#pragma once

#include <string>

// abstract class
namespace ng {

	class Tree {
	public:
		// constructor / destructor
		Tree() = default;
		virtual ~Tree() = default;

		// accessors
		[[nodiscard]] virtual const std::string& type() const = 0;
		[[nodiscard]] virtual bool empty() const = 0;

	};

} // namespace ng