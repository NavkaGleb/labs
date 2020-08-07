#ifndef MODE_H
#define MODE_H

#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <random>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <map>
#include <algorithm>

#include "../ngContainers/Deque.h"
#include "../ngContainers/Array.h"
#include "../ngContainers/List.h"
#include "../ngContainers/DList.h"
#include "../ngContainers/Stack.h"

#include "../geometry/Geometry.h"

// abstract class
class Mode {
private:
	// methods
	void parseCommands(const short& command, ng::DequeInterface<Circle>* deque, bool showCommand = false) const;

public:
	// destructor
	virtual ~Mode() = default;

	// methods
	virtual void run() = 0;

	friend class InteractiveMode;
	friend class DemoMode;

};

#endif // MODE_H
