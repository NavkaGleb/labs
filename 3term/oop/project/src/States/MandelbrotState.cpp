#include "MandelbrotState.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <boost/lexical_cast.hpp>

namespace ng {

    // constructor / destructor
    MandelbrotState::MandelbrotState(float width, float height)
        : m_windowSize(width, height),
          m_showCoordinates(true) {

        loadFonts();

        initMandelbrotSet();
        initText();
    }

    MandelbrotState::~MandelbrotState() {
        std::cout << "destructor MandelbrotState" << std::endl;
    }

    // public methods
    void MandelbrotState::mouseWheelMoved(const sf::Event& event) {

    }

    void MandelbrotState::keyPressed(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Escape)
            State::getStateStack().pop();

        if (event.key.code == sf::Keyboard::C)
            m_showCoordinates = !m_showCoordinates;

        if (event.key.code == sf::Keyboard::F)
            m_mandelbrotSet.setColoring(!m_mandelbrotSet.getColoring());

        if (event.key.code == sf::Keyboard::Num1)
            m_mandelbrotSet.setImplementation(0);

        if (event.key.code == sf::Keyboard::Num2)
            m_mandelbrotSet.setImplementation(1);

        if (event.key.code == sf::Keyboard::Num3)
            m_mandelbrotSet.setImplementation(2);

        if (event.key.code == sf::Keyboard::Num4)
            m_mandelbrotSet.setImplementation(3);

        if (event.key.code == sf::Keyboard::Num5)
            m_mandelbrotSet.setImplementation(4);
    }

    void MandelbrotState::updateInput(const float& ftime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            m_mandelbrotSet.decreaseIterations();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            m_mandelbrotSet.increaseIterations();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            m_mandelbrotSet.zoom(300.0f * ftime);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            m_mandelbrotSet.zoom(-300.0f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_mandelbrotSet.move(0.f, 200.f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_mandelbrotSet.move(0.f, -200.f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_mandelbrotSet.move(200.f * ftime, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_mandelbrotSet.move(-200.f * ftime, 0.f);
    }

    void MandelbrotState::updateText() {
        m_text.setString(
            "Iterations: " + std::to_string(m_mandelbrotSet.getIterations()) + "\n" +
            "Implementation: " + m_mandelbrotSet.getImplementationName()
        );

        if (!m_showCoordinates)
            return;

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
    }

    void MandelbrotState::update(const float& ftime) {
        updateInput(ftime);
        m_mandelbrotSet.update(ftime);
        updateText();
    }

    void MandelbrotState::render(sf::RenderTarget& target) {
        target.draw(m_mandelbrotSet);
        target.draw(m_text);

        if (!m_showCoordinates)
            return;

        target.draw(m_left);
        target.draw(m_right);
        target.draw(m_bottom);
        target.draw(m_top);
    }

    // member methods
    void MandelbrotState::loadFonts() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("ng::MandelbrotState::MandelbrotState: failed to load font");
    }

    void MandelbrotState::initText() {
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

    void MandelbrotState::initMandelbrotSet() {
        m_mandelbrotSet.setSize(sf::Vector2<MandelbrotSet::PointType>(m_windowSize));
    }

} // namespace ng