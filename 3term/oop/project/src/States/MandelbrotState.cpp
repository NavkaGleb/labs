#include "MandelbrotState.hpp"

#include <iostream>

namespace ng {

    // constructor / destructor
    MandelbrotState::MandelbrotState()
        : m_mandelbrotSet(State::getContext().window->getSize()),
          m_showCoordinates(true) {

        loadFonts();
        initText();
        initImplementations();
    }

    // public methods
    void MandelbrotState::keyPressed(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::Escape)
            State::getStateStack().pop();

        if (event.key.code == sf::Keyboard::C)
            m_showCoordinates = !m_showCoordinates;

        if (event.key.code == sf::Keyboard::F)
            m_mandelbrotSet.setColoring(!m_mandelbrotSet.getColoring());

        if (event.key.code == sf::Keyboard::Num1)
            m_mandelbrotSet.setImplementation(MandelbrotSet::ImplementationType::Pseudocode);

        if (event.key.code == sf::Keyboard::Num2)
            m_mandelbrotSet.setImplementation(MandelbrotSet::ImplementationType::ByHand);

        if (event.key.code == sf::Keyboard::Num3)
            m_mandelbrotSet.setImplementation(MandelbrotSet::ImplementationType::AVX2);

        if (event.key.code == sf::Keyboard::Num4)
            m_mandelbrotSet.setImplementation(MandelbrotSet::ImplementationType::Threads);

        if (event.key.code == sf::Keyboard::Num5)
            m_mandelbrotSet.setImplementation(MandelbrotSet::ImplementationType::ThreadPool);
    }

    void MandelbrotState::updateInput(const float& dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            m_mandelbrotSet.decreaseIterations();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            m_mandelbrotSet.increaseIterations();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            m_mandelbrotSet.zoom(300.0f * dt);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            m_mandelbrotSet.zoom(-300.0f * dt);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_mandelbrotSet.move(0.f, -200.f * dt);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_mandelbrotSet.move(0.f, 200.f * dt);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_mandelbrotSet.move(200.f * dt, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_mandelbrotSet.move(-200.f * dt, 0.f);
    }

    void MandelbrotState::updateText() {
        m_text[TextRole::Statistic].setString(
            "Iterations: " + std::to_string(m_mandelbrotSet.getIterations()) + "\n" +
            "Implementation: " + m_implementations[m_mandelbrotSet.getImplementation()] + "\n"
        );
    }

    void MandelbrotState::update(const float& dt) {
        updateInput(dt);
        m_mandelbrotSet.update();
        updateText();
    }

    void MandelbrotState::render(sf::RenderTarget& target) const {
        target.draw(m_mandelbrotSet);
        target.draw(m_text.at(TextRole::Statistic));
    }

    // member methods
    void MandelbrotState::loadFonts() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("ng::MandelbrotState::MandelbrotState: failed to load font");
    }

    void MandelbrotState::initText() {
        unsigned statisticTextCharacterSize = 18;

        // statistic text
        m_text[TextRole::Statistic].setFont(m_font);
        m_text[TextRole::Statistic].setCharacterSize(statisticTextCharacterSize);
        m_text[TextRole::Statistic].setPosition(5.0f, 5.0f);
    }

    void MandelbrotState::initImplementations() {
        using Type = MandelbrotSet::ImplementationType;

        m_implementations[Type::Pseudocode] = "Pseudo Code";
        m_implementations[Type::ByHand]     = "By Hand";
        m_implementations[Type::AVX2]       = "AVX2";
        m_implementations[Type::Threads]    = "Threads";
        m_implementations[Type::ThreadPool] = "ThreadPool";
    }

} // namespace ng