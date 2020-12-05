#pragma once

#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "States/State.hpp"
#include "Simulations/FluidSimulation/FluidPlane.hpp"

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
        // aliases
        using PixelBuffer = std::vector<uint8_t>;

        // member data
        FluidPlane m_fluidPlane;
        int m_scale;
        sf::Vector2i m_pos1;
        sf::Vector2i m_pos2;
        bool m_active;
        bool m_paused;
        PixelBuffer m_pixelBuffer;
        sf::Texture m_texture;
        sf::Sprite m_sprite;

        // member methods

    }; // class FluidState

} // namespace ng