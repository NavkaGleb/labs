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
        explicit MainMenuState(sf::RenderWindow& window);
        ~MainMenuState() override = default;

        // public methods
        void mouseButtonPressed(const sf::Event& event) override;

        void update(const float& ftime) override;
        void render(sf::RenderTarget& target) override;

    private:
        // aliases
        using ButtonPtr = std::unique_ptr<gui::Button>;

        // member data
        std::unordered_map<std::string, ButtonPtr> m_buttons;
        sf::Font                                   m_font;
        sf::RenderWindow&                          m_window;

        // member methods
        void loadFonts();
        void initButtons();

    };

} // namespace ng