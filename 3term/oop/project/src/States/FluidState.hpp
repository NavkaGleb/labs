#pragma once

#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "States/State.hpp"
#include "Gui/Slider.hpp"
#include "Simulations/FluidSimulation/Fluid.cuh"

namespace ng {
    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Header
    ////////////////////////////////////////////////////////////////////////////////////////////
    class FluidState : public State {
    public:
        // constructor / destructor
        FluidState();
        ~FluidState() override = default;

        // public methods
        void mouseMoved(const sf::Event& event) override;
        void mouseButtonPressed(const sf::Event& event) override;
        void mouseButtonReleased(const sf::Event& event) override;
        void keyPressed(const sf::Event& event) override;

        void update(const float& dt) override;
        void render(sf::RenderTarget& target) const override;

    private:
        // enums
        enum class SliderRole : short {
            Pressure = 0,
            Vorticity,
            VelocityDiffusion, ColorDiffusion, DensityDiffusion,
            ForceScale,
            BloomIntense,
            Radius
        };

        // inner structs
        struct SliderContainerItem {
            sf::Text text;
            std::unique_ptr<gui::Slider> slider;
        };

        // aliases
        using SliderContainer = std::unordered_map<SliderRole, SliderContainerItem>;

        // member data
        sf::Font m_font;
        ng::Fluid m_fluid;
        sf::Vector2i m_pos1;
        sf::Vector2i m_pos2;
        bool m_isPressed;
        bool m_isPaused;
        bool m_showSliders;
        SliderContainer m_sliders;

        // member methods
        void loadFonts();
        void initSliders();

        template <typename T>
        void updateSliderValue(std::unique_ptr<gui::Slider>& slider, float x, T& param);

        bool isSliderHovered() const;
        bool isSliderPressed() const;
        void updateSliders();
        void updateMousePosition();
        void updateFluid(float dt);

    }; // class FluidState

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Source
    ////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    void FluidState::updateSliderValue(std::unique_ptr<gui::Slider>& slider, float x, T& param) {
        slider->movePointer(x);
        param = slider->getValue();
    }

} // namespace ng