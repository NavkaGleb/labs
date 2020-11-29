#pragma once

#include "States/State.hpp"
#include "Simulations/FluidSimulation/Fluid.hpp"

namespace ng {

    class FluidState : public State {
    public:
        // constructor / destructor
        FluidState();
        ~FluidState() override = default;

        // public methods
        void mouseMoved(const sf::Event& event) override;
        void mouseButtonPressed(const sf::Event& event) override;
        void keyPressed(const sf::Event& event) override;

        void update(const float& ftime) override;
        void render(sf::RenderTarget& target) const override;

    private:
        // member data
        Fluid m_fluid;
        sf::Vector2f m_currentMousePosition;
        sf::Vector2f m_previousMousePosition;

        // member methods
        void updateMousePosition();

    }; // class FluidState

} // namespace ng