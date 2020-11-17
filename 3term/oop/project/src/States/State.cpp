#include "State.hpp"

namespace ng {

    StateStack& State::getStateStack() {
        static StateStack states;
        return states;
    }

} // namespace ng