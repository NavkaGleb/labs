#pragma once

#include <stack>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "States/State.hpp"

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
        unsigned m_frameLimit;
        sf::Clock m_clock;
        sf::Time m_frameTime;
        sf::Time m_lastFrameTime;
        sf::Font m_font;
        sf::Text m_statisticsText;
        std::stack<std::unique_ptr<State>> m_states;

        // member methods
        void updateFrameTime();
        void pollEvent();
        void update();
        void render();

    }; // class Application

} // namespace ng