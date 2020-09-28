#ifndef DEQUE_H
#define DEQUE_H

#include <vector>

#include "DequeInterface.h"
#include "DList.h"
#include "Array.h"

/* ---------------------------------- template container realisation ---------------------------------- */
namespace ng {

	/* ------------------------------- header ------------------------------- */

	template <class T, template <class> class C = ng::DList>
	class Deque : public DequeInterface<T> {
	private:
	    class Node {
	        int val;
	    };
		// variables
		C<Node> container;

	public:
		// constructor / destructor
		Deque() = default;
		~Deque() override;

		// accessors
		[[nodiscard]] const char* get_allocator_type() const override;
		[[nodiscard]] unsigned size() const override;
		T& front() override;
		T& back() override;

		// methods
		[[nodiscard]] bool empty() const override;
		void clear() override;

		void pushFront(const T& value) override;
		void pushBack(const T& value) override;

		void popFront() override;
		void popBack() override;

	};

	/* ------------------------------- source ------------------------------- */

	// destructor

	template <class T, template <class> class C>
	Deque<T, C>::~Deque() {

		this->clear();

	}

	// accessors

	template <class T, template <class> class C>
	const char* Deque<T, C>::get_allocator_type() const {

		return this->container._get_typename_();

	}

	template <class T, template <class> class C>
	unsigned Deque<T, C>::size() const {

		return this->container.size();

	}

	template <class T, template <class> class C>
	T& Deque<T, C>::front() {

		return this->container.front();

	}

	template <class T, template <class> class C>
	T& Deque<T, C>::back() {

		return this->container.back();

	}

	// methods
	template <class T, template <class> class C>
	bool Deque<T, C>::empty() const {

		return this->container.empty();

	}

	template <class T, template <class> class C>
	void Deque<T, C>::clear() {

		this->container.clear();

	}

	template <class T, template <class> class C>
	void Deque<T, C>::pushFront(const T& value) {

		this->container.pushFront(value);

	}

	template <class T, template <class> class C>
	void Deque<T, C>::pushBack(const T& value) {

		this->container.pushBack(value);

	}

	template <class T, template <class> class C>
	void Deque<T, C>::popFront() {

		this->container.popFront();

	}

	template <class T, template <class> class C>
	void Deque<T, C>::popBack() {

		this->container.popBack();

	}

}

/* ---------------------------------- vector realisation ---------------------------------- */
namespace ngvc {

	/* ------------------------------- header ------------------------------- */

	template <class T>
	class Deque : public ng::DequeInterface<T> {
	private:
		// variables
		std::vector<T> vec;
		const char* _container_;

	public:
		// constructor / destructor
		Deque();
		~Deque() override;

		// accessors
		[[nodiscard]] const char* get_allocator_type() const;
		[[nodiscard]] unsigned size() const override;
		T& front() override;
		T& back() override;

		// methods
		[[nodiscard]] bool empty() const override;
		void clear() override;

		void pushFront(const T& value) override;
		void pushBack(const T& value) override;

		void popFront() override;
		void popBack() override;

	};

	/* ------------------------------- source ------------------------------- */

	// constructor / destructor

	template <class T>
	Deque<T>::Deque() : _container_("std::vector") {}

	template <class T>
	Deque<T>::~Deque() {

		this->clear();

	}

	// accessors

	template <class T>
	const char* Deque<T>::get_allocator_type() const {

		return this->_container_;

	}

	template <class T>
	unsigned Deque<T>::size() const {

		return this->vec.size();

	}

	template <class T>
	T& Deque<T>::front() {

		return this->vec.front();

	}

	template <class T>
	T& Deque<T>::back() {

		return this->vec.back();

	}

	// methods

	template <class T>
	bool Deque<T>::empty() const {

		return this->vec.empty();

	}

	template <class T>
	void Deque<T>::clear() {

		this->vec.clear();
		this->vec.shrink_to_fit();

	}

	template <class T>
	void Deque<T>::pushFront(const T& value) {

		this->vec.emplace(this->vec.begin(), value);

	}

	template <class T>
	void Deque<T>::pushBack(const T& value) {

		this->vec.emplace_back(value);

	}

	template <class T>
	void Deque<T>::popFront() {

		this->vec.erase(this->vec.begin());

	}

	template <class T>
	void Deque<T>::popBack() {

		this->vec.pop_back();

	}

}

/* ---------------------------------- constant array realisation ---------------------------------- */
namespace ngca {

	/* ------------------------------- header ------------------------------- */

	template <class T>
	class Deque : public ng::DequeInterface<T> {
	private:
		// variables
		T* values;								// array ( pointer )
		unsigned capacity;						// capacity of array
		unsigned start;							// start index
		unsigned end;							// end index
		unsigned shift;							// for move elements from start to shift size
		const char* _container_;				// type of container

		// methods
		void move();

	public:
		// constructor / destructor
		Deque(unsigned capacity, unsigned shift);
		~Deque() override;

		// accessors
		[[nodiscard]] const char* get_allocator_type() const override;
		[[nodiscard]] unsigned size() const override;
		T& front() override;
		T& back() override;

		// methods
		[[nodiscard]] bool empty() const override;
		void clear() override;

		void pushFront(const T& value) override;
		void pushBack(const T& value) override;

		void popFront() override;
		void popBack() override;

	};

	/* ------------------------------- source ------------------------------- */

	// private methods

	template <class T>
	void Deque<T>::move() {

		for (unsigned i = this->start; i < this->end; i++)
			this->values[i - this->start] = this->values[i];

		this->end = this->end - this->start;
		this->start = 0;

	}

	// constructor / destructor

	template <class T>
	Deque<T>::Deque(unsigned capacity, unsigned shift)
		: values(nullptr), capacity(capacity), start(0), end(0), shift(shift), _container_("const arr") {}

	template <class T>
	Deque<T>::~Deque() {

		delete [] this->values;

	}

	// accessors

	template <class T>
	const char* Deque<T>::get_allocator_type() const {

		return this->_container_;

	}

	template <class T>
	unsigned Deque<T>::size() const {

		return this->end - this->start;

	}

	template <class T>
	T& Deque<T>::front() {

		assert(this->start != this->end);
		return this->values[this->start];

	}

	template <class T>
	T& Deque<T>::back() {

		assert(this->start != this->end);
		return this->values[this->end - 1];

	}

	template <class T>
	bool Deque<T>::empty() const {

		return this->start == this->end;

	}

	template <class T>
	void Deque<T>::clear() {

		delete [] this->values;
		this->values = nullptr;
		this->start = 0;
		this->end = 0;

	}

	template <class T>
	void Deque<T>::pushFront(const T& value) {

		if (this->start == 0 && this->end == this->capacity)
			return;

		if (!this->values)
			this->values = new T[this->capacity];

		if (this->start == 0 && this->shift + this->end <= this->capacity) {

			for (unsigned i = this->end; i > 0; i--)
				this->values[i + this->shift - 1] = this->values[i - 1];

			this->values[this->shift - 1] = value;
			this->start = this->shift - 1;
			this->end = this->end + this->shift;
			return;

		}

		if (this->start == 0) {

			for (unsigned i = this->end; i > 0; i--)
				this->values[i] = this->values[i - 1];

			this->values[0] = value;
			this->end++;
			return;

		}

		this->values[--this->start] = value;

	}

	template <class T>
	void Deque<T>::pushBack(const T& value) {

		if (this->start == 0 && this->end == this->capacity)
			return;

		if (!this->values)
			this->values = new T[this->capacity];

		if (this->end == this->capacity)
			this->move();

		this->values[this->end++] = value;

	}

	template <class T>
	void Deque<T>::popFront() {

		if (this->start == this->end)
			return;

		this->start++;

	}

	template <class T>
	void Deque<T>::popBack() {

		if (this->start == this->end)
			return;

		this->end--;

	}

}

/* ---------------------------------- std::deque realisation ---------------------------------- */
namespace stdng {

	/* ------------------------------- header ------------------------------- */

	template <class T>
	class Deque : public ng::DequeInterface<T> {
	private:
		// variables
		std::deque<T> dec;
		const char* _container_;

	public:
		// constructor / destructor
		Deque();
		~Deque() override;

		// accessors
		[[nodiscard]] const char* get_allocator_type() const;
		[[nodiscard]] unsigned size() const override;
		T& front() override;
		T& back() override;

		// methods
		[[nodiscard]] bool empty() const override;
		void clear() override;

		void pushFront(const T& value) override;
		void pushBack(const T& value) override;

		void popFront() override;
		void popBack() override;

	};

	/* ------------------------------- source ------------------------------- */

	// constructor / destructor

	template <class T>
	Deque<T>::Deque() : _container_("std::deque") {}

	template <class T>
	Deque<T>::~Deque() {

		this->clear();

	}

	// accessors

	template <class T>
	const char* Deque<T>::get_allocator_type() const {

		return this->_container_;

	}

	template <class T>
	unsigned Deque<T>::size() const {

		return this->dec.size();

	}

	template <class T>
	T& Deque<T>::front() {

		return this->dec.front();

	}

	template <class T>
	T& Deque<T>::back() {

		return this->dec.back();

	}

	// methods

	template <class T>
	bool Deque<T>::empty() const {

		return this->dec.empty();

	}

	template <class T>
	void Deque<T>::clear() {

		this->dec.clear();

	}

	template <class T>
	void Deque<T>::pushFront(const T& value) {

		this->dec.emplace_front(value);

	}

	template <class T>
	void Deque<T>::pushBack(const T& value) {

		this->dec.emplace_back(value);

	}

	template <class T>
	void Deque<T>::popFront() {

		this->dec.pop_front();

	}

	template <class T>
	void Deque<T>::popBack() {

		this->dec.pop_back();

	}

}

#endif // DEQUE_H