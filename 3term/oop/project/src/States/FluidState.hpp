#pragma once

#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "States/State.hpp"
#include "Gui/Slider.hpp"
#include "Simulations/FluidSimulation/Fluid.cuh"

namespace ng {

    class FluidState : public State {
    public:
        // constructor / destructor
        FluidState();
        ~FluidState() override;

        // public methods
        void mouseMoved(const sf::Event& event) override;
        void mouseButtonPressed(const sf::Event& event) override;
        void mouseButtonReleased(const sf::Event& event) override;
        void keyPressed(const sf::Event& event) override;

        void update(const float& ftime) override;
        void render(sf::RenderTarget& target) const override;

    private:
        // member data
        ng::Fluid m_fluid;
        sf::Vector2i m_pos1;
        sf::Vector2i m_pos2;
        bool m_isPressed;
        bool m_isPaused;
        gui::Slider<int, sf::RectangleShape> m_slider;

        // member methods
        void updateMousePosition();

    }; // class FluidState

} // namespace ng