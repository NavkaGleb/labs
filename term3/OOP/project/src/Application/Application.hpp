#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ng {

    class Application {
    public:
        // constructor / destructor
        Application();
        virtual ~Application() = default;

        // public methods
        void run();

    private:
        // member data
        sf::RenderWindow m_window;
        sf::Clock m_clock;
        sf::Time m_frameTime;
        sf::Time m_lastFrameTime;
        sf::Text m_statisticsText;
        bool m_isPaused;
        sf::Music m_music;
        bool m_playMusic;
        float m_volume;

        // member methods
        void loadMusic();
        void initWindow();

        void pause();
        void resume();

        void updateMusic(const sf::Event& event);
        void updateFrameTime();
        void pollEvent();
        void update();
        void render();

    }; // class Application

} // namespace ng