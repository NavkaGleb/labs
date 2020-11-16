#include "Application.hpp"

#include <iostream>
#include <cmath>

#include "States/MainMenuState.hpp"

namespace ng {

    // constructor / destructor
    Application::Application() :
        m_window(sf::VideoMode(800, 800), "SFML", sf::Style::Close),
        m_paused(false) {

        loadFonts();

        initWindow();
        initText();
        initStates();
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
    void Application::loadFonts() {
        if (!m_font.loadFromFile("../media/Fonts/Baloo2-Medium.ttf"))
            throw std::invalid_argument("Application::Application: failed to load font");
    }

    void Application::initWindow() {
        m_window.setFramerateLimit(60u);
        m_window.setVerticalSyncEnabled(true);
    }

    void Application::initText() {
        m_statisticsText.setFont(m_font);
        m_statisticsText.setPosition(std::floor(5.0f), std::floor(5.0f));
        m_statisticsText.setCharacterSize(20);
        m_statisticsText.setFillColor(sf::Color::White);
    }

    void Application::initStates() {
        m_states.push(new MainMenuState(m_window));
    }

    void Application::pause() {
        m_paused = true;
    }

    void Application::resume() {
        m_paused = false;
    }

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

                case sf::Event::LostFocus:
                    pause();
                    break;

                case sf::Event::GainedFocus:
                    resume();
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
        if (m_paused)
            return;

        m_states.update(m_frameTime.asSeconds());
        m_statisticsText.setString(
            "FPS: " + std::to_string((1.0f / m_frameTime.asSeconds())) + "\n" +
            "time: " + std::to_string(m_frameTime.asSeconds())
        );
    }

    void Application::render() {
        // clear
        m_window.clear();

        // display
        m_states.render(m_window);
        m_window.draw(m_statisticsText);
        m_window.display();
    }

} // namespace ng