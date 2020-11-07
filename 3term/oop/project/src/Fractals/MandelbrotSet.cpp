#include "MandelbrotSet.hpp"

#include <iostream>
#include <complex>

namespace ng {

    // constructor / destructor
    MandelbrotSet::MandelbrotSet()
        : m_iterations(40),
          m_left(-2.5), m_right(2.5),
          m_bottom(-2.5), m_top(2.5),
          m_sizeX(0.0), m_sizeY(0.0),
          m_implementation(1) {

        // sfml
        m_image.create(800.0, 800.0);
        m_texture.loadFromImage(m_image);
        m_sprite.setTexture(m_texture);

        m_implementations[0].name = "Pseudo code";
        m_implementations[0].func = [&](const float& ftime) {
            for (std::size_t i = 0; i < m_image.getSize().x; i += 1) {
                for (std::size_t j = 0; j < m_image.getSize().y; j += 1) {
                    std::complex<double> z = 0;
                    std::complex<double> c(
                        m_left + i / m_size.x * (m_right - m_left),
                        m_bottom + j / m_size.y * (m_top - m_bottom)
                    );

                    int iteration;

                    for (iteration = 0; std::abs(z) < 2.0 && iteration < m_iterations; ++iteration)
                        z = z * z + c;

                    int factor = int(std::sqrt(double(iteration) / double(m_iterations)) * double(m_iterations));
                    sf::Color color;
                    color = sf::Color(factor, factor, factor);
                    m_image.setPixel(i, j, color);
                }
            }

            m_texture.loadFromImage(m_image);
        };

        m_implementations[1].name = "Own implementation";
        m_implementations[1].func = [&](const float& ftime) {
            for (std::size_t i = 0; i < m_image.getSize().x; i += 1) {
                for (std::size_t j = 0; j < m_image.getSize().y; j += 1) {
                    double realZ = 0.0;
                    double imagineZ = 0.0;
                    double realC = m_left + i / m_size.x * (m_right - m_left);
                    double imagineC = m_bottom + j / m_size.y * (m_top - m_bottom);

                    int iteration;

                    for (iteration = 0; iteration < m_iterations && realZ * realZ + imagineZ * imagineZ < 4.0; ++iteration) {
                        double temp = realZ * realZ - imagineZ * imagineZ + realC;
                        imagineZ = 2 * realZ * imagineZ + imagineC;
                        realZ = temp;
                    }

                    int factor = int(std::sqrt(double(iteration) / double(m_iterations)) * double(m_iterations));
                    sf::Color color;
                    color = sf::Color(factor, factor, factor);
                    m_image.setPixel(i, j, color);
                }
            }

            m_texture.loadFromImage(m_image);
        };

    }

    MandelbrotSet::~MandelbrotSet() {

    }

    // modifiers
    void MandelbrotSet::setSize(const sf::Vector2<double>& size) {
        m_size = size;
        m_image.create(m_size.x, m_size.y);
        m_sizeX = (m_right - m_left) / m_size.x;
        m_sizeY = (m_top - m_bottom) / m_size.y;
    }

    void MandelbrotSet::setImplementation(int implementation) { m_implementation = implementation; }

    // public methods
    void MandelbrotSet::move(const sf::Vector2f& offset) {
        m_left -= offset.x * m_sizeX;
        m_right -= offset.x * m_sizeX;
        m_bottom -= offset.y * m_sizeY;
        m_top -= offset.y * m_sizeY;
    }

    void MandelbrotSet::move(float offsetX, float offsetY) {
        m_left -= offsetX * m_sizeX;
        m_right -= offsetX * m_sizeX;
        m_bottom -= offsetY * m_sizeY;
        m_top -= offsetY * m_sizeY;
    }

    void MandelbrotSet::zoomIn() {
        m_left += m_sizeX * 30.f;
        m_right -= m_sizeX * 30.f;
        m_bottom += m_sizeY * 30.f;
        m_top -= m_sizeY * 30.f;

        m_sizeX = (m_right - m_left) / m_size.x;
        m_sizeY = (m_top - m_bottom) / m_size.y;
    }

    void MandelbrotSet::zoomOut() {
        m_left -= m_sizeX * 20.f;
        m_right += m_sizeX * 20.f;
        m_bottom -= m_sizeY * 20.f;
        m_top += m_sizeY * 20.f;

        m_sizeX = (m_right - m_left) / m_size.x;
        m_sizeY = (m_top - m_bottom) / m_size.y;
    }

    void MandelbrotSet::increaseIterations() { ++m_iterations; }

    void MandelbrotSet::decreaseIterations() {
        if (m_iterations > 0)
            --m_iterations;
    }

    void MandelbrotSet::update(const float& ftime) {
        m_implementations[m_implementation](ftime);
    }

    // member methods
    void MandelbrotSet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_sprite, states);
    }

} // namespace ng