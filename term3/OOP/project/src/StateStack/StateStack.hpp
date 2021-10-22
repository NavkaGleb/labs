#pragma once

#include <stack>
#include <memory>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "States/StateInterface.hpp"

namespace ng {

    class State;

    // TODO: State interface

    class StateStack : public StateInterface {
    public:
        // alias
        using StatePtr = std::unique_ptr<State>;

        // constructor / destructor
        StateStack() = default;
        ~StateStack() override = default;

        // public methods
        void push(StatePtr&& state);
        void pop();

        void mouseMoved(const sf::Event& event) override;
        void mouseButtonPressed(const sf::Event& event) override;
        void mouseButtonReleased(const sf::Event& event) override;
        void keyPressed(const sf::Event& event) override;

        void update(const float& dt) override;
        void render(sf::RenderTarget& target) const override;

    private:
        // member data
        std::stack<StatePtr> m_states;

    }; // class StackState

} // namespace ng