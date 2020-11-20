#include "Application.hpp"

#include <iostream>
#include <cmath>

#include "States/MainMenuState.hpp"

namespace ng {

    // constructor / destructor
    Application::Application() :
        m_window(sf::VideoMode(768, 768), "SFML", sf::Style::Close),
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
        State::Context& context = State::getContext();

        context.framerateLimit = 60u;
        context.verticalSyncEnabled = true;
        context.window = &m_window;

        m_window.setFramerateLimit(context.framerateLimit);
        m_window.setVerticalSyncEnabled(context.verticalSyncEnabled);
    }

    void Application::initText() {
        m_statisticsText.setFont(m_font);
        m_statisticsText.setPosition(std::floor(5.0f), std::floor(5.0f));
        m_statisticsText.setCharacterSize(20);
        m_statisticsText.setFillColor(sf::Color::White);
    }

    void Application::initStates() {
        State::getStateStack().push(new MainMenuState(m_window));
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
                    State::getStateStack().mouseButtonPressed(event);
                    break;

                case sf::Event::MouseButtonReleased:
                    State::getStateStack().mouseButtonReleased(event);

                case sf::Event::MouseWheelMoved:
                    State::getStateStack().mouseWheelMoved(event);
                    break;
                case sf::Event::KeyPressed:
                    State::getStateStack().keyPressed(event);
                    break;

                default:
                    break;
            }
        }
    }

    void Application::update() {
        if (m_paused)
            return;

        State::getStateStack().update(m_frameTime.asSeconds());
        m_statisticsText.setString(
            "FPS: " + std::to_string((1.0f / m_frameTime.asSeconds())) + "\n" +
            "time: " + std::to_string(m_frameTime.asSeconds())
        );
    }

    void Application::render() {
        // clear
        m_window.clear();

        // display
        State::getStateStack().render(m_window);
        m_window.draw(m_statisticsText);
        m_window.display();
    }

} // namespace ng