#include "FluidState.hpp"

#include <iostream>

//#include <Simulations/FluidSimulation/FluidPlane.hpp>

namespace ng {

    // constructor / destructor
    FluidState::FluidState()
        : m_fluid(State::getContext().window->getSize(), 2),
          m_isPressed(false),
          m_isPaused(false) {

        m_pos1 = { -1, -1 };
        m_pos2 = { -1, -1 };
    }

    FluidState::~FluidState() {

    }

    // public methods
    void FluidState::mouseMoved(const sf::Event& event) {
        std::swap(m_pos1, m_pos2);

        m_pos2 = { event.mouseMove.x, event.mouseMove.y };
        m_pos2.x /= m_fluid.getScale();
        m_pos2.y /= m_fluid.getScale();
    }

    void FluidState::mouseButtonPressed(const sf::Event& event) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            m_pos1 = { event.mouseButton.x, event.mouseButton.y };
            m_pos1.x /= m_fluid.getScale();
            m_pos1.y /= m_fluid.getScale();
            m_isPressed = true;
        }

        if (event.mouseButton.button == sf::Mouse::Right)
            m_isPaused = !m_isPaused;
    }

    void FluidState::mouseButtonReleased(const sf::Event& event) {
        m_isPressed = false;
    }

    void FluidState::keyPressed(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Escape)
            State::getStateStack().pop();
    }

    void FluidState::update(const float& ftime) {
        if (!m_isPaused)
            m_fluid.update(ftime, m_pos1, m_pos2, m_isPressed);
    }

    void FluidState::render(sf::RenderTarget& target) const {
        target.draw(m_fluid);
    }

    // member methods

} // namespace ng