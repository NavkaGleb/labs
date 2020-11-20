#include "MandelbrotSet.hpp"

#include <iostream>
#include <complex>
#include <thread>
#include <vector>
#include <immintrin.h>

namespace ng {

    // constructor / destructor
    MandelbrotSet::MandelbrotSet(const sf::Vector2u& size)
        : m_bounds(4),
          m_implementation(ImplementationType::Pseudocode),
          m_zoom(1.0),
          m_threadPool(32),
          m_iterations(32),
          m_coloring(false) {

        // sfml
        m_image.create(size.x, size.y);
        m_texture.loadFromImage(m_image);
        m_sprite.setTexture(m_texture);

        // size of image
        m_size = static_cast<sf::Vector2<PointType>>(size);

        // bounds
        m_bounds[Bounds::MinX] = -2.5;
        m_bounds[Bounds::MaxX] =  1.5;
        m_bounds[Bounds::MinY] = -2.0;
        m_bounds[Bounds::MaxY] =  2.0;

        // scale
        m_scale.x = (m_bounds[Bounds::MaxX] - m_bounds[Bounds::MinX]) / m_size.x;
        m_scale.y = (m_bounds[Bounds::MaxY] - m_bounds[Bounds::MinY]) / m_size.y;

        m_implementations[ImplementationType::Pseudocode] = {
            "Pseudo code",
            [&](std::size_t start, std::size_t end, bool loadFromImage) {
                for (std::size_t i = start; i < end; ++i) {
                    for (std::size_t j = 0; j < m_image.getSize().y; ++j) {
                        std::complex<PointType> z(0.0, 0.0);
                        std::complex<PointType> c(
                            m_bounds[Bounds::MinX] + i / m_size.x * (m_bounds[Bounds::MaxX] - m_bounds[Bounds::MinX]),
                            m_bounds[Bounds::MinY] + j / m_size.y * (m_bounds[Bounds::MaxY] - m_bounds[Bounds::MinY])
                        );

                        int iteration;

                        for (iteration = 0; std::abs(z) < 2.0 && iteration < m_iterations; ++iteration)
                            z = z * z + c;

                        m_image.setPixel(i, j, getColor(iteration));
                    }
                }

                if (loadFromImage)
                    m_texture.loadFromImage(m_image);
            }
        };

        m_implementations[ImplementationType::ByHand] = {
            "Own implementation",
            [&](std::size_t start, std::size_t end, bool loadFromImage) {
                for (std::size_t i = start; i < end; i += 1) {
                    for (std::size_t j = 0; j < m_image.getSize().y; j += 1) {
                        PointType realZ = 0.0;
                        PointType imagZ = 0.0;
                        PointType realC = m_bounds[Bounds::MinX] + i / m_size.x * (m_bounds[Bounds::MaxX] - m_bounds[Bounds::MinX]);
                        PointType imagC = m_bounds[Bounds::MinY] + j / m_size.y * (m_bounds[Bounds::MaxY] - m_bounds[Bounds::MinY]);

                        int iteration;

                        for (iteration = 0; iteration < m_iterations && realZ * realZ + imagZ * imagZ < 4.0; ++iteration) {
                            PointType temp = realZ * realZ - imagZ * imagZ + realC;
                            imagZ = 2 * realZ * imagZ + imagC;
                            realZ = temp;
                        }

                        m_image.setPixel(i, j, getColor(iteration));
                    }
                }

                if (loadFromImage)
                    m_texture.loadFromImage(m_image);
            }
        };

        m_implementations[ImplementationType::AVX2] = {
            "AVX2",
            [&](std::size_t start, std::size_t end, bool loadFromImage) {
                PointType y_pos = m_bounds[Bounds::MinY];

                int y_offset = 0;
                int row_size = m_image.getSize().y;

                int x, y;

                __m256d _a, _b, _two, _four, _mask1;
                __m256d _zr, _zi, _zr2, _zi2, _cr, _ci;
                __m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump;
                __m256i _one, _c, _n, _iterations, _mask2;

                _one = _mm256_set1_epi64x(1);
                _two = _mm256_set1_pd(2.0);
                _four = _mm256_set1_pd(4.0);
                _iterations = _mm256_set1_epi64x(m_iterations);

                _x_scale = _mm256_set1_pd(m_scale.x);
                _x_jump = _mm256_set1_pd(m_scale.x * 4);
                _x_pos_offsets = _mm256_set_pd(0, 1, 2, 3);
                _x_pos_offsets = _mm256_mul_pd(_x_pos_offsets, _x_scale);

                for (y = 0; y < m_image.getSize().y; y++)
                {
                    // Reset x_position
                    _a = _mm256_set1_pd(m_bounds[Bounds::MinX] + m_scale.x * start);
                    _x_pos = _mm256_add_pd(_a, _x_pos_offsets);

                    _ci = _mm256_set1_pd(y_pos);

                    for (x = start; x < end; x += 4)
                    {
                        _cr = _x_pos;
                        _zr = _mm256_setzero_pd();
                        _zi = _mm256_setzero_pd();
                        _n = _mm256_setzero_si256();

                        repeat:
                        _zr2 = _mm256_mul_pd(_zr, _zr);
                        _zi2 = _mm256_mul_pd(_zi, _zi);
                        _a = _mm256_sub_pd(_zr2, _zi2);
                        _a = _mm256_add_pd(_a, _cr);
                        _b = _mm256_mul_pd(_zr, _zi);
                        _b = _mm256_fmadd_pd(_b, _two, _ci);
                        _zr = _a;
                        _zi = _b;
                        _a = _mm256_add_pd(_zr2, _zi2);
                        _mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ);
                        _mask2 = _mm256_cmpgt_epi64(_iterations, _n);
                        _mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));
                        _c = _mm256_and_si256(_one, _mask2); // Zero out ones where n < iterations
                        _n = _mm256_add_epi64(_n, _c); // n++ Increase all n
                        if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0)
                            goto repeat;

                        for (int i = 0; i < 4; ++i) {

                            int iteration = _n[3 - i];
                            m_image.setPixel(x + i, y_offset / row_size, getColor(iteration));
                        }

                        _x_pos = _mm256_add_pd(_x_pos, _x_jump);
                    }

                    y_pos += m_scale.y;
                    y_offset += row_size;
                }

                if (loadFromImage)
                    m_texture.loadFromImage(m_image);
            }
        };

        m_implementations[ImplementationType::Threads] = {
            "Threads",
            [&](std::size_t start, std::size_t end, bool loadFromImage) {
                std::vector<std::thread> threads(32);
                size_t offset = m_image.getSize().x / threads.size();

                for (std::size_t i = 0; i < threads.size(); ++i)
                    threads[i] = std::thread(
                        m_implementations[ImplementationType::AVX2],
                        i * offset,
                        (i + 1) * offset,
                        false
                    );

                for (auto&& thread : threads)
                    thread.join();

                m_texture.loadFromImage(m_image);
            }
        };

        m_implementations[ImplementationType::ThreadPool] = {
            "Thread pool",
            [&](std::size_t start, std::size_t end, bool loadFromImage) {
                size_t offset = m_image.getSize().x / m_threadPool.getThreadsCount();

                std::vector<std::future<void>> futures;

                for (std::size_t i = 0; i < m_threadPool.getThreadsCount(); ++i)
                    futures.emplace_back(
                        m_threadPool.enqueue([this, capture1 = i * offset, capture2 = (i + 1) * offset] {
                            m_implementations[ImplementationType::AVX2](capture1, capture2, false);
                        })
                    );

                for (auto&& future : futures)
                    future.get();

                m_texture.loadFromImage(m_image);
            }
        };
    }

    MandelbrotSet::~MandelbrotSet() {
        std::cout << "destructor MandelbrotSet" << std::endl;
    }

    // modifiers
    void MandelbrotSet::setImplementation(const ImplementationType& type) {
        m_implementation = type;
    }

    void MandelbrotSet::setColoring(bool coloring) {
        m_coloring = coloring;
    }

    // public methods
    void MandelbrotSet::move(const sf::Vector2f& offset) {
        m_bounds[Bounds::MinX] -= offset.x * m_scale.x;
        m_bounds[Bounds::MaxX] -= offset.x * m_scale.x;
        m_bounds[Bounds::MinY] -= offset.y * m_scale.y;
        m_bounds[Bounds::MaxY] -= offset.y * m_scale.y;
    }

    void MandelbrotSet::move(float offsetX, float offsetY) {
        m_bounds[Bounds::MinX] -= offsetX * m_scale.x;
        m_bounds[Bounds::MaxX] -= offsetX * m_scale.x;
        m_bounds[Bounds::MinY] += offsetY * m_scale.y;
        m_bounds[Bounds::MaxY] += offsetY * m_scale.y;
    }

    void MandelbrotSet::zoom(float factor) {
        m_bounds[Bounds::MinX] += factor * m_scale.x;
        m_bounds[Bounds::MaxX] -= factor * m_scale.x;
        m_bounds[Bounds::MinY] += factor * m_scale.y;
        m_bounds[Bounds::MaxY] -= factor * m_scale.y;

        m_zoom += factor;

        // update scale
        m_scale.x = (m_bounds[Bounds::MaxX] - m_bounds[Bounds::MinX]) / m_size.x;
        m_scale.y = (m_bounds[Bounds::MaxY] - m_bounds[Bounds::MinY]) / m_size.y;
    }

    void MandelbrotSet::increaseIterations() {
        m_iterations += 4;
    }

    void MandelbrotSet::decreaseIterations() {
        if (m_iterations > 0)
            m_iterations -= 4;
    }

    void MandelbrotSet::update() {
        m_implementations[m_implementation](0, m_image.getSize().x, true);
    }

    // member methods
    sf::Color MandelbrotSet::getColor(int iterations) const {
        if (!m_coloring) {
            if (iterations == m_iterations)
                return sf::Color::Black;

            int factor = 255 * iterations / m_iterations / 2;

            if (factor < 20)
                factor = 20;

            return sf::Color(factor, factor, factor * 2);
        }

        if (iterations == m_iterations)
            return sf::Color::Black;

        double factor1 = (1.0 - std::cos(1.0 * M_PI * (std::log2(iterations) / 10))) / 2.0;
        double factor2 = (1.0 - std::cos(2.0 * M_PI * (std::log2(iterations) / 10))) / 2.0;
        double factor3 = (1.0 - std::cos(3.0 * M_PI * (std::log2(iterations) / 10))) / 2.0;

        return sf::Color(255 * factor1, 255 * factor2, 255 * factor3, 255);
    }

    void MandelbrotSet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_sprite, states);
    }

} // namespace ng