#include "MandelbrotSet.hpp"

#include <iostream>
#include <complex>

namespace ng {

    // constructor / destructor
    MandelbrotSet::MandelbrotSet(int width, int height) : m_iterations(256) {
        m_image.create(width, height);
        m_texture.loadFromImage(m_image);
        m_sprite.setTexture(m_texture);
    }

    MandelbrotSet::~MandelbrotSet() {

    }

    // public methods
    void MandelbrotSet::increaseIterations() { ++m_iterations; }

    void MandelbrotSet::decreaseIterations() {
        if (m_iterations > 0)
            --m_iterations;
    }

    void MandelbrotSet::update(const float& ftime) {
        for (std::size_t i = 0; i < m_image.getSize().x; i += 1) {
            for (std::size_t j = 0; j < m_image.getSize().y; j += 1) {
                std::complex<float> z = 0;
                std::complex<float> c(
                    -2.5f + i / 1280.f * (1.f - (-2.5f)),
                    -1 + j / 720.f * (1 - (-1))
                );

                int iteration;

                for (iteration = 0; iteration < m_iterations && std::abs(z) < 2.0; ++iteration)
                    z = z * z + c;

                int factor = int(std::sqrt(float(iteration) / float(m_iterations)) * float(m_iterations));
                sf::Color color;
                color = sf::Color(factor, factor, factor);
                m_image.setPixel(i, j, color);
            }
        }

        m_texture.loadFromImage(m_image);
    }

    // member methods
    void MandelbrotSet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_sprite, states);
    }

} // namespace ng