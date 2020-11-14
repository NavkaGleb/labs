#include "StackState.hpp"

namespace ng {

    // public methods
    void StackState::push(State* state) {
        m_states.push(std::unique_ptr<State>(state));
    }

    void StackState::pop() {
        m_states.pop();
    }

    void StackState::mouseButtonPressed(const sf::Event& event) {
        if (!m_states.empty())
            m_states.top()->mouseButtonPressed(event);
    }

    void StackState::mouseWheelMoved(const sf::Event& event) {
        if (!m_states.empty())
            m_states.top()->mouseWheelMoved(event);
    }

    void StackState::keyPressed(const sf::Event& event) {
        if (!m_states.empty())
            m_states.top()->keyPressed(event);
    }

    void StackState::update(const float& ftime) {
        if (!m_states.empty())
            m_states.top()->update(ftime);
    }

    void StackState::render(sf::RenderTarget& target) {
        if (!m_states.empty())
            m_states.top()->render(target);
    }

} // namespace ng