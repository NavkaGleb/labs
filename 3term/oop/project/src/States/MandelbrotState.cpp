#include "MandelbrotState.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <boost/lexical_cast.hpp>

namespace ng {

    // constructor / destructor
    MandelbrotState::MandelbrotState(float width, float height)
        : m_windowSize(width, height) {

        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("ng::MandelbrotState::MandelbrotState: failed to load font");

        // mandelbrot set
        m_mandelbrotSet.setSize(sf::Vector2<double>(m_windowSize));

        // text
        m_text.setFont(m_font);
        m_text.setCharacterSize(20);
        m_text.setPosition(5.f, 70.f);

        // left text
        m_left.rotate(-90.0f);
        m_left.setFont(m_font);
        m_left.setCharacterSize(12);

        // right text
        m_right.rotate(-90.0f);
        m_right.setFont(m_font);
        m_right.setCharacterSize(12);

        // bottom text
        m_bottom.setFont(m_font);
        m_bottom.setCharacterSize(12);

        // top text
        m_top.setFont(m_font);
        m_top.setCharacterSize(12);
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
            m_mandelbrotSet.zoom(300.0f * ftime);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            m_mandelbrotSet.zoom(-300.0f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_mandelbrotSet.move(0.f, 300.f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_mandelbrotSet.move(300.f * ftime, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_mandelbrotSet.move(0.f, -300.f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_mandelbrotSet.move(-300.f * ftime, 0.f);
    }

    void MandelbrotState::updateText() {
        // left text
        m_left.setString(boost::lexical_cast<std::string>(m_mandelbrotSet.getLeft()));
        m_left.setPosition(
            std::floor(0.f),
            std::floor((m_windowSize.y + m_left.getGlobalBounds().height) / 2.0f)
        );

        // right text
        m_right.setString(boost::lexical_cast<std::string>(m_mandelbrotSet.getRight()));
        m_right.setPosition(
            std::floor(m_windowSize.x - m_right.getGlobalBounds().width * 2.f),
            std::floor((m_windowSize.y + m_right.getGlobalBounds().height) / 2.0f)
        );

        // bottom text
        m_bottom.setString(boost::lexical_cast<std::string>(m_mandelbrotSet.getBottom()));
        m_bottom.setPosition(
            std::floor((m_windowSize.x - m_bottom.getGlobalBounds().width) / 2.0f),
            std::floor(m_windowSize.y - m_bottom.getGlobalBounds().height * 2.0f)
        );

        // top text
        m_top.setString(boost::lexical_cast<std::string>(m_mandelbrotSet.getTop()));
        m_top.setPosition(
            std::floor((m_windowSize.x - m_bottom.getGlobalBounds().width) / 2.0f),
            std::floor(0.0f)
        );

        m_text.setString(
            "Iterations: " + std::to_string(m_mandelbrotSet.getIterations()) + "\n" +
            "Implementation: " + m_mandelbrotSet.getImplementationName()
        );
    }

    void MandelbrotState::update(const float& ftime) {
        updateInput(ftime);
        m_mandelbrotSet.update(ftime);
        updateText();
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