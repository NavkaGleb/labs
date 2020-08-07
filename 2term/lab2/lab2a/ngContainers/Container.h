#ifndef CONTAINER_H
#define CONTAINER_H

// abstract class
template <class T>
class Container {
public:
	// constructor / destructor
	Container() = default;
	virtual ~Container() = default;

	// accessors
	[[nodiscard]] virtual const char* _get_typename_() const = 0;
	[[nodiscard]] virtual const unsigned& size() const = 0;
	[[nodiscard]] virtual T& front() = 0;
	[[nodiscard]] virtual T& back() = 0;
	[[nodiscard]] virtual const T& front() const = 0;
	[[nodiscard]] virtual const T& back() const = 0;

	// methods
	[[nodiscard]] virtual bool empty() const = 0;
	virtual void clear() = 0;

	virtual void insert(int index, const T& value) = 0;
	virtual void pushFront(const T& value) = 0;
	virtual void pushBack(const T& value) = 0;

	virtual void erase(int index) = 0;
	virtual void popFront() = 0;
	virtual void popBack() = 0;

};

#endif // CONTAINER_H
