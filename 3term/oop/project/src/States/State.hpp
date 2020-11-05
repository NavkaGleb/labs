#pragma once

#include <SFML/Graphics.hpp>

namespace ng {

    class State {
    public:
        // constructor / destructor
        State() = default;
        virtual ~State() = default;

        // public methods
        virtual void update(const float& ftime) = 0;
        virtual void render(sf::RenderTarget& target) = 0;

    }; // class State

} // namespace ng