#include "FluidState.hpp"

#include <iostream>

namespace ng {

    // constructor / destructor
    FluidState::FluidState()
        : m_fluid(48, 0.0, 0.000001) {

    }

    // public methods
    void FluidState::mouseMoved(const sf::Event& event) {
//        std::cout << "mouse moved" << std::endl;
//        m_fluid.addDensity(event.mouseButton.x / 4, event.mouseButton.y / 4, 100);
    }

    void FluidState::mouseButtonPressed(const sf::Event& event) {
//        if (event.mouseButton.button == sf::Mouse::Left) {
//            std::cout << "clicked" << std::endl;
//            m_fluid.addDensity(event.mouseButton.x / 4, event.mouseButton.y / 4, 100);
//        }
    }

    void FluidState::keyPressed(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Escape)
            State::getStateStack().pop();
    }

    void FluidState::update(const float& ftime) {
        updateMousePosition();

        const auto& pos = sf::Mouse::getPosition(*State::getContext().window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            int posX = std::min(std::max(pos.x / 16, 0), 48);
            int posY = std::min(std::max(pos.y / 16, 0), 48);

            m_fluid.addDensity(posX, posY, 255);
            auto p = m_currentMousePosition - m_previousMousePosition;

            std::cout << "-------------------------------- " << pos.x / 16 << " " << pos.y / 16 << std::endl;
            std::cout << "-------------------------------- " << posX << " " << posY << std::endl;
            std::cout << "-------------------------------- " << p.x << " " << p.y << std::endl;
            std::cout << std::endl;

            m_fluid.addVelocity(posX, posY, static_cast<int>(pos.x) % 10, static_cast<int>(p.y) % 10);
        }

        m_fluid.update(ftime);
    }

    void FluidState::render(sf::RenderTarget& target) const {
        m_fluid.render(target);
    }

    // member methods
    void FluidState::updateMousePosition() {
        m_previousMousePosition = m_currentMousePosition;
        m_currentMousePosition = sf::Vector2f(sf::Mouse::getPosition(*State::getContext().window));
    }

} // namespace ng