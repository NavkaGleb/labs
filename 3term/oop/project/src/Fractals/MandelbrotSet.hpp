#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>

namespace ng {

    class MandelbrotSet : public sf::Drawable {
    public:
        // constructor / destructor
        MandelbrotSet();
        ~MandelbrotSet() override;

    private:
        // member data
        sf::Image m_image;

        // member methods
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    }; // class MandelbrotSet

} // namespace ng