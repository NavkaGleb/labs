#pragma once

#include "OrderedList.hpp"
#include "../Containers/Array.hpp"

namespace ng {

	/* ------------------------------------------ header ------------------------------------------ */
	template <typename T>
	class AOrderedList : public OrderedList<T> {
	private:
		// variables
		ng::Array<T> _container;
		const char* _type;

		// private methods
		int _bsearch(const T& value) const;

	public:
		// constructor / destructor
		AOrderedList();
		AOrderedList(std::initializer_list<T>& list);
		~AOrderedList() override;

		// accessors
		[[nodiscard]] const char* type() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] const T& front() const override;
		[[nodiscard]] const T& back() const override;
		[[nodiscard]] bool empty() const override;

		// public methods
		void clear() override;

		void push(const T& value) override;
		void pop(const T& value) override;

		bool exist(const T& value) const override;
		std::vector<T> search(const T& left, const T& right) const override;

		void print() const;
		void print(std::ostream& ostream) const;

	}; // class AOrderedList

	/* ------------------------------------------ source ------------------------------------------ */

	// private methods
	template <typename T>
	int AOrderedList<T>::_bsearch(const T& value) const {

		if (this->_container.empty())
			return 0;

		int left = 0;
		int right = static_cast<int>(this->_container.size()) - 1;
		int middle = 0;

		while (left <= right) {

			middle = (left + right) / 2;

			if (this->_container[middle] == value)
				return middle;
			else if (this->_container[middle] > value)
				right = middle - 1;
			else
				left = middle + 1;

		}

		return this->_container[middle] < value ? middle + 1 : middle;

	}

	// constructor / destructor
	template <typename T>
	AOrderedList<T>::AOrderedList() : _type("ng::AOrderedList") {}

	template <typename T>
	AOrderedList<T>::AOrderedList(std::initializer_list<T>& list) : _type("ng::AOrderedList") {

		for (const auto& e : list)
			this->_container.push(e);

	}

	template <typename T>
	AOrderedList<T>::~AOrderedList() { std::cout << this->_type << " destructor" << std::endl; }

	// accessors
	template <typename T>
	const char* AOrderedList<T>::type() const { return this->_type; }

	template <typename T>
	const unsigned& AOrderedList<T>::size() const { return this->_container.size(); }

	template <typename T>
	const T& AOrderedList<T>::front() const { return this->_container.front(); }

	template <typename T>
	const T& AOrderedList<T>::back() const { return this->_container.back(); }

	template <typename T>
	bool AOrderedList<T>::empty() const { return this->_container.empty(); }

	// public methods
	template <typename T>
	void AOrderedList<T>::clear() { this->_container.clear(); }

	template <typename T>
	void AOrderedList<T>::push(const T& value) { this->_container.push(this->_bsearch(value), value); }

	template <typename T>
	void AOrderedList<T>::pop(const T& value) {

		for (int i = 0; i < this->_container.size(); i++) {

			if (this->_container[i] == value)
				this->_container.pop(i), i--;
			else if (this->_container[i] > value)
				break;

		}

	}

	template <typename T>
	bool AOrderedList<T>::exist(const T& value) const { return this->_container[this->_bsearch(value)] == value; }

	template <typename T>
	std::vector<T> AOrderedList<T>::search(const T& left, const T& right) const {

		std::vector<T> result;
		int lindex = this->_bsearch(left);
		int rindex = this->_bsearch(right);

		if (lindex == this->_container.size() && lindex != 0) lindex--;
		if (rindex == this->_container.size() && rindex != 0) rindex--;

		if (this->_container[lindex] != left && this->_container[lindex] < left) lindex++;
		if (rindex != 0 && this->_container[rindex] != right && this->_container[rindex] > right) rindex--;

		if (!(left <= rindex && this->_container[lindex] >= left && this->_container[rindex] <= right)) return result;

		for (int i = lindex; i <= rindex; i++) result.emplace_back(this->_container[i]);
		return result;

	}

	template <typename T>
	void AOrderedList<T>::print() const {

		for (int i = 0; i < this->_container.size(); i++)
			std::cout << this->_container[i] << " ";
		std::cout << std::endl;

	}

	template <typename T>
	void AOrderedList<T>::print(std::ostream& ostream) const {

		std::cout << "print(o)" << std::endl;

	}

} // namespace ng