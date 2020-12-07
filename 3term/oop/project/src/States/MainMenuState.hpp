#pragma once

#include <unordered_map>
#include <memory>

#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "State.hpp"
#include "Gui/Button.hpp"

namespace ng {

    class MainMenuState : public State {
    public:
        // constructor / destructor
        MainMenuState();
        ~MainMenuState() override = default;

        // public methods
        void mouseButtonPressed(const sf::Event& event) override;
        void mouseButtonReleased(const sf::Event& event) override;

        void update(const float& dt) override;
        void render(sf::RenderTarget& target) const override;

    private:
        // enums
        enum class ButtonRole : short { MandelbrotSet = 0, FluidSimulation };

        // aliases
        using ButtonContainer = std::unordered_map<ButtonRole, std::unique_ptr<gui::Button>>;

        // member data
        ButtonContainer m_buttons;
        sf::Font        m_font;

        // member methods
        void loadFonts();
        void initButtons();

    };

} // namespace ng