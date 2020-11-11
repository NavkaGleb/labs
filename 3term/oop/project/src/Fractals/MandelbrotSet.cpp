#include "MandelbrotSet.hpp"

#include <iostream>
#include <complex>
#include <thread>
#include <vector>
#include <immintrin.h>

namespace ng {

    std::atomic<int> MandelbrotSet::Thread::complete = 0;

    // constructor / destructor
    MandelbrotSet::MandelbrotSet()
        : m_iterations(40),
          m_topLeft(-2.5, 2.0), m_bottomRight(1.5, -2.0),
          m_sizeX(0.0), m_sizeY(0.0),
          m_implementation(4) {

        std::cout << "threads = " << std::thread::hardware_concurrency() << std::endl;
        std::cout << "long double = " << sizeof(long double) << std::endl;
        std::cout << "double = " << sizeof(double) << std::endl;

        // sfml
        m_image.create(800.0, 800.0);
        m_texture.loadFromImage(m_image);
        m_sprite.setTexture(m_texture);

        m_implementations[0].name = "Pseudo code";
        m_implementations[1].name = "Own implementation";
        m_implementations[2].name = "Threads";
        m_implementations[3].name = "Thread pool";
        m_implementations[4].name = "AVX2";

        m_threads.resize(32);

        // init thread
        for (std::size_t i = 0; i < m_threads.size(); ++i) {
            m_threads[i] = new Thread(m_image, m_texture);
            m_threads[i]->m_thread = std::thread(&Thread::create, m_threads[i]);
        }
    }

    MandelbrotSet::~MandelbrotSet() {
        for (int i = 0; i < 4; i++)
        {
            m_threads[i]->m_alive = false;		 // Allow thread exit
            m_threads[i]->m_start.notify_one(); // Fake starting gun
        }

        // Clean up worker threads
        for (auto & thread : m_threads) {
            delete thread;
        }
    }

    // modifiers
    void MandelbrotSet::setSize(const sf::Vector2<PointType>& size) {
        m_size = size;
        m_image.create(m_size.x, m_size.y);
        m_sizeX = (m_bottomRight.x - m_topLeft.x) / m_size.x;
        m_sizeY = (m_bottomRight.y - m_topLeft.y) / m_size.y;
    }

    void MandelbrotSet::setImplementation(int implementation) { m_implementation = implementation; }

    // public methods
    void MandelbrotSet::move(const sf::Vector2f& offset) {
        m_topLeft.x -= offset.x * m_sizeX;
        m_bottomRight.x -= offset.x * m_sizeX;
        m_topLeft.y -= offset.y * m_sizeY;
        m_bottomRight.y -= offset.y * m_sizeY;
    }

    void MandelbrotSet::move(float offsetX, float offsetY) {
        m_topLeft.x -= offsetX * m_sizeX;
        m_bottomRight.x -= offsetX * m_sizeX;
        m_topLeft.y -= offsetY * m_sizeY;
        m_bottomRight.y -= offsetY * m_sizeY;
    }

    void MandelbrotSet::zoom(float factor) {
        m_topLeft.x += m_sizeX * factor;
        m_bottomRight.x -= m_sizeX * factor;
        m_topLeft.y += m_sizeY * factor;
        m_bottomRight.y -= m_sizeY * factor;

        m_sizeX = (m_bottomRight.x - m_topLeft.x) / m_size.x;
        m_sizeY = (m_bottomRight.y - m_topLeft.y) / m_size.y;
    }

    void MandelbrotSet::increaseIterations() { m_iterations += 5; }

    void MandelbrotSet::decreaseIterations() {
        if (m_iterations > 0)
            m_iterations -= 5;
    }

    void MandelbrotSet::update(const float& ftime) {
        switch (m_implementation) {
            case 0:
                implementation1();
                break;

            case 1:
                implementation2(0, m_image.getSize().x);
                break;

            case 2:
                implementation3();
                break;

            case 3:
                implementation4();
                break;

            case 4:
                implementation5();
                break;

            default:
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


                sf::Color color;

                if (iteration == m_iterations) {
                    color = sf::Color(255, 0, 0, 255);
                } else {
                    int factor = int(std::sqrt(double(iteration) / double(m_iterations)) * double(m_iterations));
                    color = sf::Color(factor, factor, factor);
                }

                m_image.setPixel(i, j, color);
            }
        }

        m_texture.loadFromImage(m_image);
    }

    void MandelbrotSet::implementation2(std::size_t si, std::size_t ei) {
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

                sf::Color color;

                if (iteration == m_iterations) {
                    color = sf::Color(255, 0, 0, 255);
                } else {
                    int factor = int(std::sqrt(double(iteration) / double(m_iterations)) * double(m_iterations));
                    color = sf::Color(factor, factor, factor);
                }

                m_image.setPixel(i, j, color);
            }
        }

        m_texture.loadFromImage(m_image);
    }

    void MandelbrotSet::implementation3() {
        std::vector<std::thread> threads(32);
        size_t offset = m_image.getSize().x / threads.size();

        for (std::size_t i = 0; i < threads.size(); ++i)
            threads[i] = std::thread(&MandelbrotSet::implementation2, this, i * offset, (i + 1) * offset - 1);

        for (auto& thread : threads)
            thread.join();
    }

    void MandelbrotSet::implementation4() {
        size_t offset = m_image.getSize().x / m_threads.size();

        Thread::complete = 0;

        // start thread
        for (std::size_t i = 0; i < 32; ++i)
            m_threads[i]->launch(i * offset, (i + 1) * offset - 1, m_topLeft, m_bottomRight, m_iterations);

        // while loop
        while (Thread::complete < 32) {}

        m_texture.loadFromImage(m_image);
    }

    void MandelbrotSet::implementation5() {
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
            _a = _mm256_set1_pd(m_topLeft.x);
            _x_pos = _mm256_add_pd(_a, _x_pos_offsets);

            _ci = _mm256_set1_pd(y_pos);

            for (x = 0; x < m_image.getSize().x; x += 4)
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

                    int iteration = _n[i];
                    sf::Color color;

                    if (iteration == m_iterations) {
                        color = sf::Color(255, 0, 0, 255);
                    } else {
                        int factor = int(std::sqrt(double(iteration) / double(m_iterations)) * double(m_iterations));
                        color = sf::Color(factor, factor, factor);
                    }

                    m_image.setPixel(x + i, y_offset / row_size, color);
                }

                _x_pos = _mm256_add_pd(_x_pos, _x_jump);
            }

            y_pos += y_scale;
            y_offset += row_size;
        }

        m_texture.loadFromImage(m_image);
    }

    // member methods
    void MandelbrotSet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_sprite, states);
    }

} // namespace ng