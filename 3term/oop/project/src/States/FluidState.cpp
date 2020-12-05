#include "FluidState.hpp"

#include <iostream>

#include <Simulations/FluidSimulation/FluidPlane.hpp>

namespace ng {

    // constructor / destructor
    FluidState::FluidState()
        : m_scale(2),
          m_active(false),
          m_paused(false) {

        const sf::Vector2u& windowSize = State::getContext().window->getSize();

        cudaInit(windowSize.x / m_scale, windowSize.y / m_scale);

        m_pixelBuffer.resize(windowSize.x * windowSize.y);
        m_texture.create(windowSize.x / m_scale, windowSize.y / m_scale);

        m_pos1 = { -1, -1 };
        m_pos2 = { -1, -1 };
    }

    FluidState::~FluidState() {
        cudaExit();
    }

    // public methods
    void FluidState::mouseMoved(const sf::Event& event) {
        std::swap(m_pos1, m_pos2);

        m_pos2 = { event.mouseMove.x, event.mouseMove.y };
        m_pos2.x /= m_scale;
        m_pos2.y /= m_scale;
    }

    void FluidState::mouseButtonPressed(const sf::Event& event) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            m_pos1 = { event.mouseButton.x, event.mouseButton.y };
            m_pos1.x /= m_scale;
            m_pos1.y /= m_scale;
            m_active = true;
        }

        if (event.mouseButton.button == sf::Mouse::Right)
            m_paused = !m_paused;
    }

    void FluidState::mouseButtonReleased(const sf::Event& event) {
        m_active = false;
    }

    void FluidState::keyPressed(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Escape)
            State::getStateStack().pop();
    }

    void FluidState::update(const float& ftime) {
        if (!m_paused)
            m_fluidPlane.update(ftime, m_pixelBuffer.data(), m_pos1.x, m_pos1.y, m_pos2.x, m_pos2.y, m_active);

        m_texture.update(m_pixelBuffer.data());
        m_sprite.setTexture(m_texture);
        m_sprite.setScale({ static_cast<float>(m_scale), static_cast<float>(m_scale) });
    }

    void FluidState::render(sf::RenderTarget& target) const {
        target.draw(m_sprite);
    }

    // member methods

} // namespace ng