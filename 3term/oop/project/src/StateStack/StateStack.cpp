#include "StateStack.hpp"

#include <States/State.hpp>

namespace ng {

    // public methods
    void StateStack::push(StatePtr&& state) {
        m_states.push(std::move(state));
    }

    void StateStack::pop() {
        m_states.pop();
    }

    void StateStack::mouseMoved(const sf::Event& event) {
        if (!m_states.empty())
            m_states.top()->mouseMoved(event);
    }

    void StateStack::mouseButtonPressed(const sf::Event& event) {
        if (!m_states.empty())
            m_states.top()->mouseButtonPressed(event);
    }

    void StateStack::mouseButtonReleased(const sf::Event& event) {
        if (!m_states.empty())
            m_states.top()->mouseButtonReleased(event);
    }

    void StateStack::keyPressed(const sf::Event& event) {
        if (!m_states.empty())
            m_states.top()->keyPressed(event);
    }

    void StateStack::update(const float& dt) {
        if (!m_states.empty())
            m_states.top()->update(dt);
    }

    void StateStack::render(sf::RenderTarget& target) const {
        if (!m_states.empty())
            m_states.top()->render(target);
    }

} // namespace ng