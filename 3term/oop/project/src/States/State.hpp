#pragma once

#include <SFML/Graphics.hpp>

namespace ng {

    class State {
    public:
        // constructor / destructor
        State() = default;
        virtual ~State() = default;

        // public methods
        virtual void mouseButtonPressed(const sf::Event& event) {};
        virtual void mouseWheelMoved(const sf::Event& event) {};
        virtual void keyPressed(const sf::Event& event) {};

        virtual void update(const float& ftime) = 0;
        virtual void render(sf::RenderTarget& target) = 0;

    }; // class State

} // namespace ng