#pragma once

#include <unordered_map>
#include <memory>

#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "State.hpp"
#include "Gui/Button.hpp"

namespace ng {

    class MainMenuState final : public State {
    public:
        // constructor / destructor
        explicit MainMenuState(sf::RenderWindow& window);
        ~MainMenuState() override = default;

        // public methods
        void mouseButtonPressed(const sf::Event& event) override;

        void update(const float& ftime) override;
        void render(sf::RenderTarget& target) override;

    private:
        // member data
        std::unordered_map<std::string, std::unique_ptr<gui::Button>> m_buttons;
        sf::Font m_font;
        sf::RenderWindow& m_window;

        // member methods
        void loadFonts();
        void initButtons();

    };

} // namespace ng