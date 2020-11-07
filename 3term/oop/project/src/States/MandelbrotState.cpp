#include "MandelbrotState.hpp"

namespace ng {

    // constructor / destructor
    MandelbrotState::MandelbrotState() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("ng::MandelbrotState::MandelbrotState: failed to load font");

        // mandelbrot set
        m_mandelbrotSet.setSize(sf::Vector2<double>(800.f, 800.f));

        // text
        m_text.setFont(m_font);
        m_text.setCharacterSize(20);
        m_text.setPosition(5.f, 70.f);
    }

    // public methods
    void MandelbrotState::updateInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            m_mandelbrotSet.setImplementation(0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            m_mandelbrotSet.setImplementation(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            m_mandelbrotSet.increaseIterations();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            m_mandelbrotSet.decreaseIterations();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            m_mandelbrotSet.zoomIn();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            m_mandelbrotSet.zoomOut();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_mandelbrotSet.move(0.f, 10.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_mandelbrotSet.move(10.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_mandelbrotSet.move(0.f, -10.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_mandelbrotSet.move(-10.f, 0.f);
    }

    void MandelbrotState::update(const float& ftime) {
        updateInput();

        m_mandelbrotSet.update(ftime);

        m_text.setString(
            "Iterations: " + std::to_string(m_mandelbrotSet.getIterations()) + "\n" +
            "Implementation: " + m_mandelbrotSet.getImplementationName()
        );
    }

    void MandelbrotState::render(sf::RenderTarget& target) {
        target.draw(m_mandelbrotSet);
        target.draw(m_text);
    }

} // namespace ng