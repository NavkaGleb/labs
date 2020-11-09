#include "MandelbrotSet.hpp"

#include <iostream>
#include <complex>
#include <thread>

#include <SFML/System/Thread.hpp>

namespace ng {

    // constructor / destructor
    MandelbrotSet::MandelbrotSet()
        : m_iterations(40),
          m_left(-2.5e-0), m_right(1.5),
          m_bottom(-2.0), m_top(2.0),
          m_sizeX(0.0), m_sizeY(0.0),
          m_implementation(1) {

        // sfml
        m_image.create(800.0, 800.0);
        m_texture.loadFromImage(m_image);
        m_sprite.setTexture(m_texture);

        m_implementations[0].name = "Pseudo code";
        m_implementations[0].func = [&](const float& ftime, std::size_t startI, std::size_t endI,
                                                            std::size_t startJ, std::size_t endJ) {
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
        m_implementations[1].func = [&](const float& ftime, std::size_t startI, std::size_t endI,
                                                            std::size_t startJ, std::size_t endJ) {
            for (std::size_t i = startI; i < endI; i += 1) {
                for (std::size_t j = startJ; j < endJ; j += 1) {
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
                    m_image.setPixel(i, j, sf::Color(factor, factor, factor));
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

    void MandelbrotSet::zoom(float factor) {
        m_left += m_sizeX * factor;
        m_right -= m_sizeX * factor;
        m_bottom += m_sizeY * factor;
        m_top -= m_sizeY * factor;

        m_sizeX = (m_right - m_left) / m_size.x;
        m_sizeY = (m_top - m_bottom) / m_size.y;
    }

    void MandelbrotSet::increaseIterations() { ++m_iterations; }

    void MandelbrotSet::decreaseIterations() {
        if (m_iterations > 0)
            --m_iterations;
    }

    void MandelbrotSet::update(const float& ftime) {
        std::thread thread1(m_implementations[m_implementation].func, ftime, 0,                m_size.x / 2, 0,                m_size.y / 2);
        std::thread thread2(m_implementations[m_implementation].func, ftime, m_size.x / 2 + 1, m_size.x,     0,                m_size.y / 2);
        std::thread thread3(m_implementations[m_implementation].func, ftime, 0,                m_size.x / 2, m_size.y / 2 + 1, m_size.y);
        m_implementations[m_implementation](                          ftime, m_size.x / 2 + 1, m_size.x,     m_size.y / 2 + 1, m_size.y);
        thread1.join();
        thread2.join();
        thread3.join();
    }

    // member methods
    void MandelbrotSet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_sprite, states);
    }

} // namespace ng