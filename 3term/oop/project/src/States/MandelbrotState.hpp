#pragma once

#include "State.hpp"

namespace ng {

    class MandelbrotState : public State {
    public:
        // constructor / destructor
        MandelbrotState();
        ~MandelbrotState() override = default;

        // public methods
        void update(const float& ftime) final;
        void render(sf::RenderTarget& target) final;

    private:
        // member data
        sf::RectangleShape m_shape;

    }; // class MandelbrotState

} // namespace ng