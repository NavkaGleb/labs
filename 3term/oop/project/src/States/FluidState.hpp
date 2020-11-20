#pragma once

#include "States/State.hpp"
#include "Simulations/Fluid.hpp"

namespace ng {

    class FluidState : public State {
    public:
        // constructor / destructor
        FluidState() = default;
        ~FluidState() override = default;

        // public methods
        void update(const float& ftime) override;
        void render(sf::RenderTarget& target) const override;

    private:
        // member data
        Fluid m_fluid;

    }; // class FluidState

} // namespace ng