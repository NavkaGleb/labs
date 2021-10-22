#ifndef HASHEDARRAYTREE_HPP
#define HASHEDARRAYTREE_HPP

#include <iostream>
#include <algorithm>
#include <cassert>

#include "Container.h"

namespace ng {

	template <class T>
	class HashedArrayTree : public Container<T> {
	private:
		// variables
		T** roots;
		int power;
		unsigned base;
		unsigned sz;
		const char* _typename_;

		// methods
		unsigned uipow(unsigned base, unsigned degree);
		void resize(const unsigned& dir);

	public:
		// constructor / destructor
		HashedArrayTree();
		~HashedArrayTree() override;

		// accessors
		[[nodiscard]] const char* _get_typename_() const override;
		[[nodiscard]] const unsigned& size() const override;
		[[nodiscard]] unsigned capacity() const;

		[[nodiscard]] T& front() override;
		[[nodiscard]] T& back() override;
		[[nodiscard]] const T& front() const override;
		[[nodiscard]] const T& back() const override;

		// methods
		[[nodiscard]] bool empty() const override;
		void clear() override;

		void insert(int index, const T& value);
		void pushFront(const T& value) override;
		void pushBack(const T& value) override;

		void erase(int index) override;
		void popFront() override;
		void popBack() override;

		void print(const std::string& separator = " ", const std::string& lseparator = "", bool showIndex = false) const;

	};

	// private methods

	template <class T>
	unsigned HashedArrayTree<T>::uipow(unsigned base_, unsigned degree) {

		unsigned result = 1;

		for (unsigned i = 0; i < degree; i++)
			result *= base_;

		return result;

	}

	template <class T>
	void HashedArrayTree<T>::resize(const unsigned& dir) {

		unsigned nbase = this->uipow(2, this->power + dir);							// new base
		unsigned nri = 0;															// new root i
		unsigned nrj = 0;															// new root j
		unsigned csz = 0;															// current size
		bool stop = false;															// for stop iteration
		auto** nroots = new T*[nbase];												// new roots

		for (unsigned i = 0; i < nbase; i++)
			nroots[i] = nullptr;

		for (unsigned i = 0; !stop && i < this->base; i++) {

			for (unsigned j = 0; j < this->base; j++) {

				if (i * this->base + j == this->sz) {

					stop = true;
					break;

				}

				nri = csz / nbase;
				nrj = csz - nri * nbase;
				csz++;

				if (!nroots[nri])
					nroots[nri] = new T[nbase];

				nroots[nri][nrj] = this->roots[i][j];

			}

		}

		for (unsigned i = 0; i < this->base; i++)
			delete [] this->roots[i];
		delete [] this->roots;

		this->roots = nroots;
		this->base = nbase;
		this->power += dir;

	}

	// constructor / destructor

	template <class T>
	HashedArrayTree<T>::HashedArrayTree()
		: power(0), base(1), sz(0), _typename_("ng::HashedArrayTree") {

		this->roots = new T*[this->base];

		for (unsigned i = 0; i < this->base; i++)
			this->roots[i] = nullptr;

	}

	template <class T>
	HashedArrayTree<T>::~HashedArrayTree() {

		for (unsigned i = 0; i < this->base; i++)
			delete [] this->roots[i];
		delete [] this->roots;

	}

	// accessors

	template <class T>
	const char* HashedArrayTree<T>::_get_typename_() const {

		return this->_typename_;

	}

	template <class T>
	const unsigned& HashedArrayTree<T>::size() const {

		return this->sz;

	}

	template <class T>
	unsigned HashedArrayTree<T>::capacity() const {

		return this->base * this->base;

	}

	template <class T>
	T& HashedArrayTree<T>::front() {

		assert(this->sz != 0);
		return this->roots[0][0];

	}

	template <class T>
	T& HashedArrayTree<T>::back() {

		assert(this->sz != 0);
		return this->roots[(this->sz - 1) / this->base][(this->sz - 1) - ((this->sz - 1) / this->base) * this->base];

	}

	template <class T>
	const T& HashedArrayTree<T>::front() const {

		assert(this->sz != 0);
		return this->roots[0][0];

	}

	template <class T>
	const T& HashedArrayTree<T>::back() const {

		assert(this->sz != 0);
		return this->roots[(this->sz - 1) / this->base][(this->sz - 1) - ((this->sz - 1) / this->base) * this->base];

	}

	// public methods

	template <class T>
	bool HashedArrayTree<T>::empty() const {

		return this->sz == 0;

	}

	template <class T>
	void HashedArrayTree<T>::clear() {

		for (unsigned i = 0; i < this->base; i++)
			delete [] this->roots[i];
		delete [] this->roots;

		this->sz = 0;
		this->power = 1;
		this->base = 2;

	}

	template <class T>
	void HashedArrayTree<T>::insert(const int index, const T& value) {

		if (index < 0 || index > this->sz)
			return;

		if (index == this->sz) {

			this->pushBack(value);
			return;

		}

		if (this->sz == this->base * this->base)
			this->resize(1);

		unsigned ei = (this->sz - 1) / this->base;											// end i
		unsigned ej = this->sz - 1 - ei * this->base;										// end j
		unsigned si = index / this->base;													// start i
		unsigned sj = index - si * this->base;												// start j
		unsigned j;

		for (unsigned i = ei; i >= 0; i--) {

			if (i == ei) j = ej;
			else j = this->base - 1;

			while (j >= 0) {

				if (j != this->base - 1) {

					this->roots[i][j + 1] = this->roots[i][j];

				} else {

					if (!this->roots[i + 1])
						this->roots[i + 1] = new T[this->base];
					this->roots[i + 1][0] = this->roots[i][j];

				}

				if (i == si && j == sj) break;
				if (j == 0) break;

				j--;

			}

			if (i == si && j == sj) break;
			if (i == 0) break;

		}

		this->roots[si][sj] = value;
		this->sz++;

	}

	template <class T>
	void HashedArrayTree<T>::pushFront(const T& value) {

		this->insert(0, value);

	}

	template <class T>
	void HashedArrayTree<T>::pushBack(const T& value) {

		if (this->sz == this->base * this->base)
			this->resize(1);

		unsigned i = this->sz / this->base;
		unsigned j = this->sz - i * this->base;

		if (!this->roots[i])
			this->roots[i] = new T[this->base];

		this->roots[i][j] = value;
		this->sz++;

	}

	template <class T>
	void HashedArrayTree<T>::erase(const int index) {

		if (index < 0 || index > this->sz - 1)
			return;

		if (index == this->sz - 1) {

			this->popBack();
			return;

		}

		unsigned ei = (this->sz - 1) / this->base;											// end i
		unsigned ej = this->sz - 1 - ei * this->base;										// end j
		unsigned si = index / this->base;													// start i
		unsigned sj = index - si * this->base;												// start j
		unsigned j;

		for (unsigned i = si; i <= ei; i++) {

			if (i == si) j = sj;
			else j = 0;

			while (j < this->base) {

				if (j != this->base - 1)
					this->roots[i][j] = this->roots[i][j + 1];
				else if (i != ei)
					this->roots[i][j] = this->roots[i + 1][0];

				if (i == ei && j == ej) break;

				j++;

			}

			if (i == ei && j == ej) break;

		}

		this->sz--;

		if (this->sz == this->base * this->base / 16)
			this->resize(-1);

	}

	template <class T>
	void HashedArrayTree<T>::popFront() {

		this->erase(0);

	}

	template <class T>
	void HashedArrayTree<T>::popBack() {

		this->sz--;

		if (this->sz == this->base * this->base / 16)
			this->resize(-1);

	}

	template <class T>
	void HashedArrayTree<T>::print(const std::string& separator, const std::string& lseparator, const bool showIndex) const {

		if (this->sz == 0) {

			std::cout << "HashedArrayTree is empty (" << std::endl;
			return;

		}

		for (unsigned i = 0; i <= this->sz / this->base; i++) {

			if (this->roots[i]) {

				for (unsigned j = 0; i * this->base + j < this->sz && j < this->base; j++) {

					if (showIndex)
						std::cout << i * this->base + j << ": ";
					std::cout << this->roots[i][j] << separator;

				}

			}

			std::cout << lseparator;

		}

		std::cout << std::endl;

	}

}

#endif // HASHEDARRAYTREE_HPP
