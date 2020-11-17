#pragma once

#include "State.hpp"
#include "Fractals/MandelbrotSet.hpp"

namespace ng {

    class MandelbrotState : public State {
    public:
        // constructor / destructor
        MandelbrotState(float width, float height);
        ~MandelbrotState() override;

        // accessors
        [[nodiscard]] inline const MandelbrotSet& getMandelbrotSet() const { return m_mandelbrotSet; }

        // public methods
        void mouseWheelMoved(const sf::Event& event) final;
        void keyPressed(const sf::Event& event) final;

        void updateInput(const float& ftime);
        void updateText();

        void update(const float& ftime) final;
        void render(sf::RenderTarget& target) final;

    private:
        // member data
        sf::Vector2f  m_windowSize;
        MandelbrotSet m_mandelbrotSet;
        sf::Font      m_font;
        sf::Text      m_text;
        sf::Text      m_left;
        sf::Text      m_right;
        sf::Text      m_bottom;
        sf::Text      m_top;
        bool          m_showCoordinates;

        // member methods
        void loadFonts();
        void initText();
        void initMandelbrotSet();

    }; // class MandelbrotState

} // namespace ng