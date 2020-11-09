#include "MandelbrotState.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <boost/lexical_cast.hpp>

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

        m_left.setFont(m_font);
        m_left.setCharacterSize(12);
        m_left.setPosition(0.0f, 370.0f);

        m_right.setFont(m_font);
        m_right.setCharacterSize(12);
        m_right.setPosition(750.0f, 370.0f);

        m_bottom.setFont(m_font);
        m_bottom.setCharacterSize(12);
        m_bottom.setPosition(370.0f, 0.0f);

        m_top.setFont(m_font);
        m_top.setCharacterSize(12);
        m_top.setPosition(370.0f, 770.0f);
    }

    // public methods
    void MandelbrotState::mouseWheelMoved(const sf::Event& event) {
        if (event.mouseWheel.delta == -1)
            m_mandelbrotSet.decreaseIterations();
        if (event.mouseWheel.delta == 1)
            m_mandelbrotSet.increaseIterations();
    }

    void MandelbrotState::updateInput(const float& ftime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            m_mandelbrotSet.setImplementation(0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            m_mandelbrotSet.setImplementation(1);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            m_mandelbrotSet.zoomIn();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            m_mandelbrotSet.zoomOut();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_mandelbrotSet.move(0.f, 500.f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_mandelbrotSet.move(500.f * ftime, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_mandelbrotSet.move(0.f, -500.f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_mandelbrotSet.move(-500.f * ftime, 0.f);
    }

    void MandelbrotState::update(const float& ftime) {
        static std::function<std::string(double)> toString = [](double value) {
            std::stringstream ss;
            ss << std::fixed << value;

            return ss.str();
        };

        updateInput(ftime);

        m_mandelbrotSet.update(ftime);

        m_left.setString(boost::lexical_cast<std::string>(m_mandelbrotSet.getLeft()));
        m_right.setString(std::to_string(m_mandelbrotSet.getRight()));
        m_bottom.setString(std::to_string(m_mandelbrotSet.getBottom()));
        m_top.setString(std::to_string(m_mandelbrotSet.getTop()));

        m_text.setString(
            "Iterations: " + std::to_string(m_mandelbrotSet.getIterations()) + "\n" +
            "Implementation: " + m_mandelbrotSet.getImplementationName()
        );
    }

    void MandelbrotState::render(sf::RenderTarget& target) {
        target.draw(m_mandelbrotSet);
        target.draw(m_text);
        target.draw(m_left);
        target.draw(m_right);
        target.draw(m_bottom);
        target.draw(m_top);
    }

} // namespace ng