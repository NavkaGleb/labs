#include "MandelbrotState.hpp"

namespace ng {

    // constructor / destructor
    MandelbrotState::MandelbrotState() {
        m_shape.setPosition(50.f, 50.f);
        m_shape.setSize(sf::Vector2f(100.f, 100.f));
        m_shape.setFillColor(sf::Color::Red);
    }

    // public methods
    void MandelbrotState::update(const float& ftime) {
        m_shape.move(10.f * ftime, 10.f * ftime);
        m_shape.setScale(m_shape.getScale() * 0.9999f);
    }

    void MandelbrotState::render(sf::RenderTarget& target) {
        target.draw(m_shape);
    }

} // namespace ng