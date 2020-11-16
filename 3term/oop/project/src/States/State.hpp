#pragma once

#include <SFML/Graphics.hpp>


namespace ng {

    class StateStack;

    class State {
    public:
        // constructor / destructor
        explicit State(StateStack& states);
        virtual ~State() = default;

        // public methods
        virtual void mouseButtonPressed(const sf::Event& event) {};
        virtual void mouseWheelMoved(const sf::Event& event) {};
        virtual void keyPressed(const sf::Event& event) {};

        virtual void update(const float& ftime) = 0;
        virtual void render(sf::RenderTarget& target) = 0;

    private:
        // member data
        ng::StateStack& m_states;

    }; // class State

} // namespace ng