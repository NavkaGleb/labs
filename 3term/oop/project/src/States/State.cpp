#include "State.hpp"

namespace ng {

    // static protected accessors
    StateStack& State::getStateStack() {
        static StateStack states;
        return states;
    }

    State::Context& State::getContext() {
        static Context context;
        return context;
    }

} // namespace ng