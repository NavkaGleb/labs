#include "FluidState.hpp"

namespace ng {

    // constructor / destructor

    // public methods
    void FluidState::update(const float& ftime) {

    }

    void FluidState::render(sf::RenderTarget& target) const {
        target.draw(m_fluid);
    }

} // namespace ng