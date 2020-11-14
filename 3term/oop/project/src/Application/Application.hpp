#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "StateStack/StackState.hpp"

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
        sf::Font m_font;
        sf::Text m_statisticsText;
        StackState m_states;
        bool m_paused;

        // member methods
        void loadFonts();
        void initWindow();
        void initText();
        void initStates();

        void pause();
        void resume();

        void updateFrameTime();
        void pollEvent();
        void update();
        void render();

    }; // class Application

} // namespace ng