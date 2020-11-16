#pragma once

#include <unordered_map>
#include <memory>

#include "State.hpp"
#include "Gui/Button.hpp"

namespace ng {

    class MainMenuState final : public State {
    public:
        // constructor / destructor
        MainMenuState();
        ~MainMenuState() override = default;

        // public methods
        void update(const float& ftime) override;
        void render(sf::RenderTarget& target) override;

    private:
        // member data
        std::unordered_map<std::string, std::unique_ptr<gui::Button>> m_buttons;
        sf::Font m_font;

    };

} // namespace ng