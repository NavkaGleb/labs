#include "State.hpp"

// std lib
#include <stdexcept>

namespace ng {

    // public methods
    void State::mouseWheelMoved(const sf::Event& /* event */) {
        throw std::runtime_error("State::mouseWheelMoved: function not implemented");
    }

    void State::keyPressed(const sf::Event& /* event */) {
        throw std::runtime_error("State::keyPressed: function not implemented");
    }

} // namespace