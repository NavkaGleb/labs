#include "Application.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include "States/MainMenuState.hpp"

namespace ng {

    // constructor / destructor
    Application::Application() :
        m_window(sf::VideoMode(768, 768), "Default :)", sf::Style::Close),
        m_paused(false) {

        initWindow();
        State::getStateStack().push(std::make_unique<MainMenuState>());
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
    void Application::initWindow() {
        State::Context& context = State::getContext();

        context.framerateLimit = 120u;
        context.verticalSyncEnabled = true;
        context.window = &m_window;

        m_window.setFramerateLimit(context.framerateLimit);
        m_window.setVerticalSyncEnabled(context.verticalSyncEnabled);
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
            if (event.type == sf::Event::Closed)
                m_window.close();

            if (event.type == sf::Event::LostFocus)
                pause();

            if (event.type == sf::Event::GainedFocus)
                resume();

            if (event.type == sf::Event::MouseMoved)
                State::getStateStack().mouseMoved(event);

            if (event.type == sf::Event::MouseButtonPressed)
                State::getStateStack().mouseButtonPressed(event);

            if (event.type == sf::Event::MouseButtonReleased)
                State::getStateStack().mouseButtonReleased(event);

            if (event.type == sf::Event::KeyPressed)
                State::getStateStack().keyPressed(event);
        }
    }

    void Application::update() {
        if (m_paused)
            return;

        State::getStateStack().update(m_frameTime.asSeconds());

        std::ostringstream oss;
        oss << std::setprecision(3) << "Project FPS: " << 1.0f / m_frameTime.asSeconds() << " "
                                    << "Time taken: " << m_frameTime.asSeconds();
        m_window.setTitle(oss.str());
    }

    void Application::render() {
        // clear
        m_window.clear();

        // display
        State::getStateStack().render(m_window);

        m_window.display();
    }

} // namespace ng