#pragma once

#include "OrderedList.hpp"
#include "../Containers/List.hpp"

namespace ng {

	/* ------------------------------------------ header ------------------------------------------ */
	template <typename T>
	class LOrderedList : public OrderedList<T> {
	private:
		// variables
		List<T> _container;
		const char* _type;

	public:
		// constructor / destructor
		LOrderedList();
		LOrderedList(std::initializer_list<T>& list);
		~LOrderedList() override;

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

		void print() const override;
		void print(std::ostream& ostream) const override;


	}; // class LOrderedList

	/* ------------------------------------------ source ------------------------------------------ */
	// constructor / destructor
	template <typename T>
	LOrderedList<T>::LOrderedList() : _type("ng::LOrderedList") {}

	template <typename T>
	LOrderedList<T>::LOrderedList(std::initializer_list<T>& list) : _type("ng::LOrderedList") {

		for (const auto& e : list)
			this->_container.push(e);

	}

	template <typename T>
	LOrderedList<T>::~LOrderedList() { std::cout << "OList destructor" << std::endl; }

	// accessors
	template <typename T>
	const char* LOrderedList<T>::type() const { return this->_type; }

	template <typename T>
	const unsigned& LOrderedList<T>::size() const { return this->_container.size();	}

	template <typename T>
	const T& LOrderedList<T>::front() const { return this->_container.front(); }

	template <typename T>
	const T& LOrderedList<T>::back() const { return this->_container.back(); }

	template <typename T>
	bool LOrderedList<T>::empty() const { return this->_container.empty(); }

	// public methods
	template <typename T>
	void LOrderedList<T>::clear() { this->_container.clear(); }

	template <typename T>
	void LOrderedList<T>::push(const T& value) {

		if (this->_container.empty())
			this->_container.pushBack(value);

		int index = 0;

		for (const auto& e : this->_container) {

			if (e < value) index++;
			else break;

		}

		this->_container.push(index, value);

	}

	template <typename T>
	void LOrderedList<T>::pop(const T& value) {

		if (this->_container.back() < value)
			return;

		int index = 0;

		for (typename ng::List<T>::Iterator it = this->_container.begin(); it != this->_container.end();) {

			if (*it == value)
				++it, this->_container.pop(index);
			else if (*it > value)
				break;
			else
				++it, index++;

		}

	}

	template <typename T>
	bool LOrderedList<T>::exist(const T& value) const {

		std::cout << "exist" << std::endl;
		return false;

	}

	template <typename T>
	std::vector<T> LOrderedList<T>::search(const T& left, const T& right) const {

		std::cout << "search" << std::endl;
		return std::vector<T>(0);

	}

	template <typename T>
	void LOrderedList<T>::print() const {

		for (const auto& e : this->_container)
			std::cout << e << " ";
		std::cout << std::endl;

	}

	template <typename T>
	void LOrderedList<T>::print(std::ostream& ostream) const {

		for (const auto& e : this->_container)
			ostream << e << " ";
		ostream << std::endl;

	}


} // namespace ng