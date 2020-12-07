#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.hpp"
#include "Fractals/MandelbrotSet.hpp"

namespace ng {

    class MandelbrotState : public State {
    public:
        // constructor / destructor
        MandelbrotState();
        ~MandelbrotState() override = default;

        // public methods
        void keyPressed(const sf::Event& event) override;

        void updateInput(const float& dt);
        void updateText();

        void update(const float& dt) override;
        void render(sf::RenderTarget& target) const override;

    private:
        // enums
        enum class TextRole : short { Statistic = 0 };

        // aliases
        using ImplementationContainer = std::unordered_map<MandelbrotSet::ImplementationType, std::string>;

        // member data
        MandelbrotSet                          m_mandelbrotSet;
        sf::Font                               m_font;
        std::unordered_map<TextRole, sf::Text> m_text;
        bool                                   m_showCoordinates;
        ImplementationContainer                m_implementations;

        // member methods
        void loadFonts();
        void initText();
        void initImplementations();

    }; // class MandelbrotState

} // namespace ng