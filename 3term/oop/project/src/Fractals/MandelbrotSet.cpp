#include "MandelbrotSet.hpp"

#include <iostream>
#include <complex>
#include <thread>
#include <vector>
#include <immintrin.h>

namespace ng {

    // constructor / destructor
    MandelbrotSet::MandelbrotSet(const sf::Vector2u& size)
        : m_iterations(40),
          m_topLeft(-2.5, 2.0), m_bottomRight(1.5, -2.0),
          m_implementation(ImplementationType::Pseudocode),
          m_threadPool(32),
          m_coloring(false) {

        // sfml
        m_image.create(size.x, size.y);
        m_texture.loadFromImage(m_image);
        m_sprite.setTexture(m_texture);

        m_size = static_cast<sf::Vector2<PointType>>(size);

        m_scale.x = (m_bottomRight.x - m_topLeft.x) / m_size.x;
        m_scale.y = (m_bottomRight.y - m_topLeft.y) / m_size.y;

        std::cout << "x = " << m_scale.x << std::endl;
        std::cout << "y = " << m_scale.y << std::endl;

        m_implementations[ImplementationType::Pseudocode].name = "Pseudo code";
        m_implementations[ImplementationType::ByHand].name     = "Own implementation";
        m_implementations[ImplementationType::AVX2].name       = "AVX2";
        m_implementations[ImplementationType::Threads].name    = "Threads";
        m_implementations[ImplementationType::ThreadPool].name = "Thread pool";
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
        m_topLeft.x     -= offset.x * m_scale.x;
        m_bottomRight.x -= offset.x * m_scale.x;
        m_topLeft.y     -= offset.y * m_scale.y;
        m_bottomRight.y -= offset.y * m_scale.y;
    }

    void MandelbrotSet::move(float offsetX, float offsetY) {
        m_topLeft.x     -= offsetX * m_scale.x;
        m_bottomRight.x -= offsetX * m_scale.x;
        m_topLeft.y     -= offsetY * m_scale.y;
        m_bottomRight.y -= offsetY * m_scale.y;
    }

    void MandelbrotSet::zoom(float factor) {
        m_topLeft.x     += factor * m_scale.x;
        m_bottomRight.x -= factor * m_scale.x;
        m_topLeft.y     += factor * m_scale.y;
        m_bottomRight.y -= factor * m_scale.y;

        m_scale.x = (m_bottomRight.x - m_topLeft.x) / m_size.x;
        m_scale.y = (m_bottomRight.y - m_topLeft.y) / m_size.y;
    }

    void MandelbrotSet::increaseIterations() { m_iterations += 5; }

    void MandelbrotSet::decreaseIterations() {
        if (m_iterations > 0)
            m_iterations -= 5;
    }

    void MandelbrotSet::update(const float& ftime) {
        switch (m_implementation) {
            case ImplementationType::Pseudocode:
                implementation1();
                break;

            case ImplementationType::ByHand:
                implementation2(0, m_image.getSize().x);
                break;

            case ImplementationType::AVX2:
                implementation5(0, m_image.getSize().x);
                break;

            case ImplementationType::Threads:
                implementation3();
                break;

            case ImplementationType::ThreadPool:
                implementation4();
                break;
        }
    }

    void MandelbrotSet::implementation1() {
        for (std::size_t i = 0; i < m_image.getSize().x; i += 1) {
            for (std::size_t j = 0; j < m_image.getSize().y; j += 1) {
                std::complex<PointType> z = 0;
                std::complex<PointType> c(
                    m_topLeft.x + i / m_size.x * (m_bottomRight.x - m_topLeft.x),
                    m_topLeft.y + j / m_size.y * (m_bottomRight.y - m_topLeft.y)
                );

                int iteration;

                for (iteration = 0; std::abs(z) < 2.0 && iteration < m_iterations; ++iteration)
                    z = z * z + c;

                m_image.setPixel(i, j, getColor(iteration));
            }
        }

        m_texture.loadFromImage(m_image);
    }

    void MandelbrotSet::implementation2(std::size_t si, std::size_t ei, bool setImage) {
        for (std::size_t i = si; i < ei; i += 1) {
            for (std::size_t j = 0; j < m_image.getSize().y; j += 1) {
                PointType realZ = 0.0;
                PointType imagineZ = 0.0;
                PointType realC = m_topLeft.x + i / m_size.x * (m_bottomRight.x - m_topLeft.x);
                PointType imagineC = m_topLeft.y + j / m_size.y * (m_bottomRight.y - m_topLeft.y);

                int iteration;

                for (iteration = 0; iteration < m_iterations && realZ * realZ + imagineZ * imagineZ < 4.0; ++iteration) {
                    PointType temp = realZ * realZ - imagineZ * imagineZ + realC;
                    imagineZ = 2 * realZ * imagineZ + imagineC;
                    realZ = temp;
                }

                m_image.setPixel(i, j, getColor(iteration));
            }
        }

        if (setImage)
            m_texture.loadFromImage(m_image);
    }

    void MandelbrotSet::implementation3() {
        std::vector<std::thread> threads(32);
        size_t offset = m_image.getSize().x / threads.size();

        for (std::size_t i = 0; i < threads.size(); ++i)
            threads[i] = std::thread(&MandelbrotSet::implementation5, this, i * offset, (i + 1) * offset, false);

        for (auto&& thread : threads)
            thread.join();

        m_texture.loadFromImage(m_image);
    }

    void MandelbrotSet::implementation4() {
        size_t offset = m_image.getSize().x / m_threadPool.getThreadsCount();

        std::vector<std::future<void>> futures;

        for (std::size_t i = 0; i < m_threadPool.getThreadsCount(); ++i)
            futures.emplace_back(m_threadPool.enqueue([this, capture1 = i * offset, capture2 = (i + 1) * offset] {
                implementation5(capture1, capture2, false);
            }));

        for (auto&& future : futures)
            future.get();

        m_texture.loadFromImage(m_image);
    }

    void MandelbrotSet::implementation5(std::size_t si, std::size_t ei, bool setImage) {
        double x_scale = (m_bottomRight.x - m_topLeft.x) / 800.0;
        double y_scale = (m_bottomRight.y - m_topLeft.y) / 800.0;

        double y_pos = m_topLeft.y;

        int y_offset = 0;
        int row_size = 800;

        int x, y;

        __m256d _a, _b, _two, _four, _mask1;
        __m256d _zr, _zi, _zr2, _zi2, _cr, _ci;
        __m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump;
        __m256i _one, _c, _n, _iterations, _mask2;

        _one = _mm256_set1_epi64x(1);
        _two = _mm256_set1_pd(2.0);
        _four = _mm256_set1_pd(4.0);
        _iterations = _mm256_set1_epi64x(m_iterations);

        _x_scale = _mm256_set1_pd(x_scale);
        _x_jump = _mm256_set1_pd(x_scale * 4);
        _x_pos_offsets = _mm256_set_pd(0, 1, 2, 3);
        _x_pos_offsets = _mm256_mul_pd(_x_pos_offsets, _x_scale);

        for (y = 0; y < m_image.getSize().y; y++)
        {
            // Reset x_position
            _a = _mm256_set1_pd(m_topLeft.x + x_scale * si);
            _x_pos = _mm256_add_pd(_a, _x_pos_offsets);

            _ci = _mm256_set1_pd(y_pos);

            for (x = si; x < ei; x += 4)
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

            y_pos += y_scale;
            y_offset += row_size;
        }

        if (setImage)
            m_texture.loadFromImage(m_image);
    }

    // member methods
    sf::Color MandelbrotSet::getColor(int iterations) {
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