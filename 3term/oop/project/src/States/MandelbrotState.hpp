#pragma once

#include "State.hpp"
#include "Fractals/MandelbrotSet.hpp"

namespace ng {

    class MandelbrotState : public State {
    public:
        // constructor / destructor
        MandelbrotState();
        ~MandelbrotState() override = default;

        // accessors
        [[nodiscard]] inline const MandelbrotSet& getMandelbrotSet() const { return m_mandelbrotSet; }

        // public methods
        void updateInput();
        void update(const float& ftime) final;
        void render(sf::RenderTarget& target) final;

    private:
        // member data
        MandelbrotSet m_mandelbrotSet;
        sf::Font m_font;
        sf::Text m_text;

    }; // class MandelbrotState

} // namespace ng