#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace ng {

    class MandelbrotSet : public sf::Drawable {
    public:
        // constructor / destructor
        MandelbrotSet(int width, int height);
        ~MandelbrotSet() override;

        // accessors
        [[nodiscard]] inline int getIterations() const { return m_iterations; }

        // public methods
        void increaseIterations();
        void decreaseIterations();
        void update(const float& ftime);

    private:
        // member data
        sf::Image m_image;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        int m_iterations;

        // member methods
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    }; // class MandelbrotSet

} // namespace ng