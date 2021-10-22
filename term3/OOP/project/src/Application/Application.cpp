#include "Application.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#include "States/MainMenuState.hpp"

namespace ng {

    // constructor / destructor
    Application::Application() :
        m_window(sf::VideoMode(768, 768), "Default :)", sf::Style::Close),
        m_isPaused(false),
        m_playMusic(true),
        m_volume(100.0f) {

        loadMusic();
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
    void Application::loadMusic() {
        if (!m_music.openFromFile("../media/Music/Toccata - En Re Mineur.ogg"))
            throw std::invalid_argument("Application::loadMusic: failed to open the file");

        m_music.setLoop(true);
        m_music.play();
    }

    void Application::initWindow() {
        State::Context& context = State::getContext();

        context.framerateLimit = 120u;
        context.verticalSyncEnabled = true;
        context.window = &m_window;

        m_window.setFramerateLimit(context.framerateLimit);
        m_window.setVerticalSyncEnabled(context.verticalSyncEnabled);
    }

    void Application::pause() {
        m_isPaused = true;
    }

    void Application::resume() {
        m_isPaused = false;
    }

    void Application::updateMusic(const sf::Event& event) {
        switch (event.key.code) {
            case sf::Keyboard::M:
                m_playMusic = !m_playMusic;

                if (m_playMusic) m_music.play();
                else             m_music.pause();
                break;

            case sf::Keyboard::Up:
                m_music.setVolume(m_volume += 1.0f);
                break;

            case sf::Keyboard::Down:
                m_music.setVolume(m_volume -= 1.0f);
                break;

            default:
                break;
        }
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

            if (event.type == sf::Event::KeyPressed) {
                updateMusic(event);
                State::getStateStack().keyPressed(event);
            }
        }
    }

    void Application::update() {
        if (m_isPaused)
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