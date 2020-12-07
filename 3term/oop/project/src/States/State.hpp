#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "StateInterface.hpp"
#include "StateStack/StateStack.hpp"

namespace ng {

    class State : public StateInterface {
    public:
        // constructor / destructor
        State() = default;
        ~State() override = default;

        // public methods
        void mouseMoved(const sf::Event& event) override {};
        void mouseButtonPressed(const sf::Event& event) override {};
        void mouseButtonReleased(const sf::Event& event) override {};
        void keyPressed(const sf::Event& event) override {};

        void update(const float& dt) override = 0;
        void render(sf::RenderTarget& target) const override = 0;

        // friends
        friend class Application;

    private:
        // inner structs
        struct Context {
            // data
            sf::RenderWindow* window              = nullptr;
            unsigned          framerateLimit      = 60;
            bool              verticalSyncEnabled = true;
        };

    protected:
        // static protected accessors
        [[nodiscard]] static StateStack& getStateStack();
        [[nodiscard]] static Context& getContext();

    }; // class State

} // namespace ng