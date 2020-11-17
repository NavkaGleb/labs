#pragma once

#include <stack>
#include <memory>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace ng {

    class State;

    class StateStack {
    public:
        // constructor / destructor
        StateStack() = default;
        virtual ~StateStack() = default;

        // public methods
        void push(State*);
        void pop();

        void mouseButtonPressed(const sf::Event& event);
        void mouseWheelMoved(const sf::Event& event);
        void keyPressed(const sf::Event& event);

        void update(const float& ftime);
        void render(sf::RenderTarget& target);

    private:
        // alias
        using StatePtr = std::unique_ptr<State>;

        // member data
        std::stack<StatePtr> m_states;

    }; // class StackState

} // namespace ng