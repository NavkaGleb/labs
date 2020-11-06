#include "MandelbrotState.hpp"

namespace ng {

    // constructor / destructor
    MandelbrotState::MandelbrotState() : m_mandelbrotSet(1280, 720) {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("ng::MandelbrotState::MandelbrotState: failed to load font");

        // text
        m_text.setFont(m_font);
        m_text.setCharacterSize(20);
        m_text.setPosition(5.f, 30.f);
    }

    // public methods
    void MandelbrotState::updateInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            m_mandelbrotSet.increaseIterations();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_mandelbrotSet.decreaseIterations();
    }

    void MandelbrotState::update(const float& ftime) {
        updateInput();

        m_mandelbrotSet.update(ftime);

        m_text.setString("Iterations: " + std::to_string(m_mandelbrotSet.getIterations()));
    }

    void MandelbrotState::render(sf::RenderTarget& target) {
        target.draw(m_mandelbrotSet);
        target.draw(m_text);
    }

} // namespace ng