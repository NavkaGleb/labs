#include "MandelbrotState.hpp"

#include <iostream>
#include <cmath>

//#include <boost/lexical_cast.hpp>

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
            m_mandelbrotSet.move(0.f, -200.f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_mandelbrotSet.move(0.f, 200.f * ftime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_mandelbrotSet.move(200.f * ftime, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_mandelbrotSet.move(-200.f * ftime, 0.f);
    }

    void MandelbrotState::updateText() {
        static sf::Vector2u windowSize = State::getContext().window->getSize();
        static std::function<std::string(MandelbrotSet::PointType)> toString = [](MandelbrotSet::PointType point) {
//            return boost::lexical_cast<std::string>(point);
            return std::to_string(point);
        };

        m_text[TextRole::Statistic].setString(
            "Iterations: " + std::to_string(m_mandelbrotSet.getIterations()) + "\n" +
            "Implementation: " + m_implementations[m_mandelbrotSet.getImplementation()] + "\n"
        );

        if (!m_showCoordinates)
            return;

        // left text
        m_text[TextRole::MinX].setString(toString(m_mandelbrotSet.getLocalBounds().at(MandelbrotSet::Bounds::MinX)));
        m_text[TextRole::MinX].setPosition(
            std::floor(0.f),
            std::floor((windowSize.y + m_text[TextRole::MinX].getGlobalBounds().height) / 2.0f)
        );

        // right text
        m_text[TextRole::MaxX].setString(toString(m_mandelbrotSet.getLocalBounds().at(MandelbrotSet::Bounds::MaxX)));
        m_text[TextRole::MaxX].setPosition(
            std::floor(windowSize.x - m_text[TextRole::MaxX].getGlobalBounds().width * 2.f),
            std::floor((windowSize.y + m_text[TextRole::MaxX].getGlobalBounds().height) / 2.0f)
        );

        // top text
        m_text[TextRole::MaxY].setString(toString(m_mandelbrotSet.getLocalBounds().at(MandelbrotSet::Bounds::MaxY)));
        m_text[TextRole::MaxY].setPosition(
            std::floor((windowSize.x - m_text[TextRole::MaxY].getGlobalBounds().width) / 2.0f),
            std::floor(0.0f)
        );

        // bottom text
        m_text[TextRole::MinY].setString(toString(m_mandelbrotSet.getLocalBounds().at(MandelbrotSet::Bounds::MinY)));
        m_text[TextRole::MinY].setPosition(
            std::floor((windowSize.x - m_text[TextRole::MinY].getGlobalBounds().width) / 2.0f),
            std::floor(windowSize.y - m_text[TextRole::MinY].getGlobalBounds().height * 2.0f)
        );
    }

    void MandelbrotState::update(const float& ftime) {
        updateInput(ftime);
        m_mandelbrotSet.update();
        updateText();
    }

    void MandelbrotState::render(sf::RenderTarget& target) const {
        target.draw(m_mandelbrotSet);
        target.draw(m_text.at(TextRole::Statistic));

        if (!m_showCoordinates)
            return;

        target.draw(m_text.at(TextRole::MinX));
        target.draw(m_text.at(TextRole::MaxX));
        target.draw(m_text.at(TextRole::MinY));
        target.draw(m_text.at(TextRole::MaxY));
    }

    // member methods
    void MandelbrotState::loadFonts() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("ng::MandelbrotState::MandelbrotState: failed to load font");
    }

    void MandelbrotState::initText() {
        unsigned statisticTextCharacterSize = 20;
        unsigned coordinatesTextCharacterSize = 12;

        // statistic text
        m_text[TextRole::Statistic].setFont(m_font);
        m_text[TextRole::Statistic].setCharacterSize(statisticTextCharacterSize);
        m_text[TextRole::Statistic].setPosition(5.f, 70.f);

        // left text
        m_text[TextRole::MinX].rotate(-90.0f);
        m_text[TextRole::MinX].setFont(m_font);
        m_text[TextRole::MinX].setCharacterSize(coordinatesTextCharacterSize);

        // right text
        m_text[TextRole::MaxX].rotate(-90.0f);
        m_text[TextRole::MaxX].setFont(m_font);
        m_text[TextRole::MaxX].setCharacterSize(coordinatesTextCharacterSize);

        // top text
        m_text[TextRole::MaxY].setFont(m_font);
        m_text[TextRole::MaxY].setCharacterSize(coordinatesTextCharacterSize);

        // bottom text
        m_text[TextRole::MinY].setFont(m_font);
        m_text[TextRole::MinY].setCharacterSize(coordinatesTextCharacterSize);
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