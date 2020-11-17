#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <StateStack/StateStack.hpp>

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