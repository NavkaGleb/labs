#include "FluidState.hpp"

#include <iostream>

namespace ng {

    // constructor / destructor
    FluidState::FluidState()
        : m_fluid(State::getContext().window->getSize().x / 2, State::getContext().window->getSize().y / 2, 2),
          m_isPressed(false),
          m_isPaused(false),
          m_slider(0, 25) {

        m_slider.setValue(10);
        m_slider.setPosition(400, 50);
        m_slider.setSize(sf::Vector2f(200, 30));
        m_slider.setBackgroundColor(sf::Color::Blue);
        m_slider.setForegroundColor(sf::Color::Green);
    }

    FluidState::~FluidState() {

    }

    // public methods
    void FluidState::mouseMoved(const sf::Event& event) {
        int x = std::min<int>(State::getContext().window->getSize().x, std::max(0, event.mouseMove.x));
        int y = std::min<int>(State::getContext().window->getSize().y, std::max(0, event.mouseMove.y));

        std::swap(m_pos1, m_pos2);
        m_pos2 = { static_cast<int>(x / m_fluid.getScale()), static_cast<int>(y / m_fluid.getScale()) };
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

        if (event.mouseButton.button == sf::Mouse::Left && m_slider.isHovered())
            std::cout << "fuck this shit" << std::endl;
    }

    void FluidState::mouseButtonReleased(const sf::Event& event) {
        m_isPressed = false;
    }

    void FluidState::keyPressed(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Escape)
            State::getStateStack().pop();
    }

    void FluidState::update(const float& ftime) {
        m_slider.update(sf::Mouse::getPosition(*State::getContext().window));
        updateMousePosition();

        if (!m_isPaused)
            m_fluid.update(ftime, m_pos1, m_pos2, m_isPressed);
    }

    void FluidState::render(sf::RenderTarget& target) const {
        target.draw(m_fluid);
        target.draw(m_slider);
    }

    // member methods
    void FluidState::updateMousePosition() {
        if (std::abs(m_pos1.x - m_pos2.x) == 1 || std::abs(m_pos1.y - m_pos2.y) == 1)
            m_pos2 = m_pos1;
    }

} // namespace ng