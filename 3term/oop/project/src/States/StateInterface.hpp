#pragma once

namespace ng {

    class StateInterface {
    public:
        // constructor / destructor
        StateInterface() = default;
        virtual ~StateInterface() = default;

        // public methods
        virtual void mouseMoved(const sf::Event& event) = 0;
        virtual void mouseButtonPressed(const sf::Event& event) = 0;
        virtual void mouseButtonReleased(const sf::Event& event) = 0;
        virtual void keyPressed(const sf::Event& event) = 0;

        virtual void update(const float& dt) = 0;
        virtual void render(sf::RenderTarget& target) const = 0;

    }; // class StateInterface

} // namespace ng