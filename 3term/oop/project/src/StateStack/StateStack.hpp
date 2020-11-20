#pragma once

#include <stack>
#include <memory>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace ng {

    class State;

    // TODO: State interface

    class StateStack {
    public:
        // alias
        using StatePtr = std::unique_ptr<State>;

        // constructor / destructor
        StateStack() = default;
        virtual ~StateStack() = default;

        // public methods
        void push(StatePtr&& state);
        void pop();

        void mouseButtonPressed(const sf::Event& event);
        void mouseButtonReleased(const sf::Event& event);
        void mouseWheelMoved(const sf::Event& event);
        void keyPressed(const sf::Event& event);

        void update(const float& ftime);
        void render(sf::RenderTarget& target) const;

    private:
        // member data
        std::stack<StatePtr> m_states;

    }; // class StackState

} // namespace ng