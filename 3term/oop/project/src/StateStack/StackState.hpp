#pragma once

#include <stack>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>

#include "States/State.hpp"

namespace ng {

    class StackState {
    public:
        // constructor / destructor
        StackState() = default;
        virtual ~StackState() = default;

        // accessors
        [[nodiscard]] inline State* top() const { return m_states.top().get(); }

        // public methods
        void push(State*);
        void pop();

        void mouseButtonPressed(const sf::Event& event);
        void mouseWheelMoved(const sf::Event& event);
        void keyPressed(const sf::Event& event);

        void update(const float& ftime);
        void render(sf::RenderTarget& target);

    private:
        // member data
        std::stack<std::unique_ptr<State>> m_states;

    }; // class StackState

} // namespace ng