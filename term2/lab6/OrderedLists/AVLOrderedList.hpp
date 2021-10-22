#pragma once

#include "OrderedList.hpp"
#include "../Trees/AVLTree.hpp"

namespace ng {

	/* ------------------------------------------ header ------------------------------------------ */
	template <typename T>
	class AVLOrderedList : public OrderedList<T> {
	private:
		// variables
		AVLTree<T> _tree;
		const char* _type;

	public:
		// constructor / destructor
		AVLOrderedList();
		AVLOrderedList(std::initializer_list<T>& list);
		~AVLOrderedList() override;

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
		void print(std::ostream &ostream) const override;

	}; // AVLOrderedList

	/* ------------------------------------------ source ------------------------------------------ */
	// constructor / destructor
	template <typename T>
	AVLOrderedList<T>::AVLOrderedList() : _type("ng::BSTOrderedList") {}

	template <typename T>
	AVLOrderedList<T>::AVLOrderedList(std::initializer_list<T>& list) : _type("ng::BSTOrderedList") {

		for (const auto& e : list)
			this->_tree.push(e);

	}

	template <typename T>
	AVLOrderedList<T>::~AVLOrderedList() { std::cout << "ng::BSTOrderedList destructor" << std::endl; }

	// accessors
	template <typename T>
	const char* AVLOrderedList<T>::type() const { return this->_type; }

	template <typename T>
	const unsigned& AVLOrderedList<T>::size() const { return this->_tree.size(); }

	template <typename T>
	const T& AVLOrderedList<T>::front() const { return this->_tree.root()->value(); }

	template <typename T>
	const T& AVLOrderedList<T>::back() const { return this->_tree.root()->value(); }

	template <typename T>
	bool AVLOrderedList<T>::empty() const { return this->_tree.empty(); }

	// public methods
	template <typename T>
	void AVLOrderedList<T>::clear() { this->_tree.clear(); }

	template <typename T>
	void AVLOrderedList<T>::push(const T& value) { this->_tree.push(value); }

	template <typename T>
	void AVLOrderedList<T>::pop(const T& value) { this->_tree.pop(value); }

	template <typename T>
	bool AVLOrderedList<T>::exist(const T& value) const { return this->_tree.exist(value) != nullptr; }

	template<typename T>
	std::vector<T> AVLOrderedList<T>::search(const T& left, const T& right) const {

		std::vector<T> result;
		typename ng::BST<T>::Iterator end(this->_tree.nearestLess(right));

		end.next();

		for (typename ng::BST<T>::Iterator it(this->_tree.nearestGreater(left)); it != end; ++it)
			result.emplace_back(*it);

		return result;

	}

	template<typename T>
	void AVLOrderedList<T>::print() const {

		for (const auto& e : this->_tree)
			std::cout << e << " ";
		std::cout << std::endl;

	}

	template<typename T>
	void AVLOrderedList<T>::print(std::ostream &ostream) const {

		for (const auto& e : this->_tree)
			ostream << e << " ";
		ostream << std::endl;

	}

} // namespace ng