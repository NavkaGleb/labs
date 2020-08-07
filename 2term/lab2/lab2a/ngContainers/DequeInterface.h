#ifndef DEQUEINTERFACE_H
#define DEQUEINTERFACE_H

namespace ng {

	// abstract class
	template <class T>
	class DequeInterface {
	public:
		// destructor
		virtual ~DequeInterface() = default;

		// accessors
		[[nodiscard]] virtual const char* get_allocator_type() const = 0;
		virtual unsigned size() const = 0;

		virtual T& front() = 0;
		virtual T& back() = 0;

		// methods
		[[nodiscard]] virtual bool empty() const = 0;
		virtual void clear() = 0;

		virtual void pushFront(const T& value) = 0;
		virtual void pushBack(const T& value) = 0;

		virtual void popFront() = 0;
		virtual void popBack() = 0;

	};

}

#endif // DEQUEINTERFACE_H
