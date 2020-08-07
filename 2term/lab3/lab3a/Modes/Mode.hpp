#ifndef MODE_HPP
#define MODE_HPP

// abstract class
class Mode {
public:
	// constructor / destructor
	Mode() = default;
	virtual ~Mode() = default;

	// methods
	virtual void run() = 0;

};

#endif // MODE_HPP
