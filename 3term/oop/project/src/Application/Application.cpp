#include "Application.hpp"

#include <iostream>
#include <cmath>

#include "States/MandelbrotState.hpp"

namespace ng {

    // constructor / destructor
    Application::Application() :
        m_window(sf::VideoMode(800, 800), "SFML", sf::Style::Close),
        m_frameLimit(60u) {

        // window
        m_window.setFramerateLimit(m_frameLimit);
        m_window.setVerticalSyncEnabled(true);

        // font
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("Application::Application: failed to load font");

        // text
        m_statisticsText.setFont(m_font);
        m_statisticsText.setPosition(std::floor(5.0f), std::floor(5.0f));
        m_statisticsText.setCharacterSize(20);
        m_statisticsText.setFillColor(sf::Color::White);

        // states
        m_states.push(new MandelbrotState(800.f, 800.f));
    }

    // public methods
    void Application::run() {
        while (m_window.isOpen()) {
            updateFrameTime();
            pollEvent();
            update();
            render();
        }
    }

    // member methods
    void Application::updateFrameTime() {
        m_lastFrameTime = m_frameTime;
        m_frameTime = m_clock.restart();
    }

    void Application::pollEvent() {
        sf::Event event{};

        while (m_window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    m_window.close();
                    break;

                case sf::Event::MouseButtonPressed:
                    m_states.mouseButtonPressed(event);
                    break;

                case sf::Event::MouseWheelMoved:
                    m_states.mouseWheelMoved(event);
                    break;
                case sf::Event::KeyPressed:
                    m_states.keyPressed(event);
                    break;

                default:
                    break;
            }
        }
    }

    void Application::update() {
        m_states.top()->update(m_frameTime.asSeconds());

        m_statisticsText.setString(
            "FPS: " + std::to_string((1.0f / m_frameTime.asSeconds())) + "\n" +
            "time: " + std::to_string(m_frameTime.asSeconds())
        );
    }

    void Application::render() {
        m_window.clear();
        m_states.render(m_window);

        m_window.draw(m_statisticsText);

        m_window.display();
    }

} // namespace ng