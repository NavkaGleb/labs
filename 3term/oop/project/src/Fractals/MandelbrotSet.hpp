#pragma once

// std lib
#include <string>
#include <unordered_map>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <cmath>
#include <vector>
#include <iostream>

#include <immintrin.h>

// sfml lib
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

namespace ng {

    class MandelbrotSet : public sf::Drawable {
    public:
        // usings
        // TODO: implement long arithmetic for doubles
        using PointType = double;

        // constructor / destructor
        MandelbrotSet();
        ~MandelbrotSet() override;

        // accessors
        [[nodiscard]] inline int getIterations() const { return m_iterations; }
        [[nodiscard]] inline PointType getLeft() const { return m_topLeft.x; }
        [[nodiscard]] inline PointType getRight() const { return m_bottomRight.x; }
        [[nodiscard]] inline PointType getBottom() const { return m_bottomRight.y; }
        [[nodiscard]] inline PointType getTop() const { return m_topLeft.y; }
        [[nodiscard]] inline const sf::Vector2<PointType>& getSize() const { return m_size; }
        [[nodiscard]] inline int getImplementation() const { return m_implementation; }
        [[nodiscard]] inline const std::string& getImplementationName() const {
            return m_implementations.at(m_implementation).name;
        }

        // modifiers
        void setSize(const sf::Vector2<PointType>& size);
        void setImplementation(int implementation);

        // public methods
        void move(const sf::Vector2f& offset);
        void move(float offsetX, float offsetY);
        void zoom(float factor);
        void increaseIterations();
        void decreaseIterations();
        void update(const float& ftime);
        void implementation1();
        void implementation2(std::size_t si, std::size_t ei);
        void implementation3();
        void implementation4();
        void implementation5();

    private:
        // inner structs
        struct Implementation {
            // data
            std::string name;
        };

        class Thread {
        public:
            // static
            static std::atomic<int> complete;

            // constructor / destructor
            Thread(sf::Image& image, sf::Texture& texture) : m_image(image), m_texture(texture), m_iterations(0), m_alive(true) {
                bi = 0;
                ei = 0;
            }
            ~Thread() {
                m_thread.join();
            };

            // accessors
            [[nodiscard]] inline std::function<void(std::size_t, std::size_t)>& getFunc() { return m_func; }

            // modifiers
            void setThread(std::thread&& thread) { m_thread = std::move(thread); }
            void setFunc(const std::function<void(std::size_t, std::size_t)>& func) {
                m_func = func;
            }

            // public methods
            void launch(std::size_t bi, std::size_t ei, sf::Vector2<PointType> tl, sf::Vector2<PointType> br, int i) {
                this->bi = bi;
                this->ei = ei;
                this->m_topLeft = tl;
                this->m_bottomRight = br;
                m_iterations = i;

                std::unique_lock<std::mutex> lm(m_mutex);
                m_start.notify_one();
            }

            void create() {
                while (m_alive) {
                    std::unique_lock<std::mutex> lm(m_mutex);
                    m_start.wait(lm);

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

//                    m_texture.loadFromImage(m_image);
                    ++Thread::complete;
                }
            }

        public:
            // member data
            int m_iterations;
            sf::Vector2<PointType> m_topLeft;
            sf::Vector2<PointType> m_bottomRight;
            std::condition_variable m_start;
            bool m_alive;
            std::mutex m_mutex;
            std::thread m_thread;
            std::function<void(std::size_t, std::size_t)> m_func;
            sf::Image& m_image;
            sf::Texture& m_texture;
            std::size_t bi{};
            std::size_t ei{};

        }; // class Thread

        // member data
        sf::Image m_image;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2<PointType> m_size;
        int m_iterations;
        sf::Vector2<PointType> m_topLeft;
        sf::Vector2<PointType> m_bottomRight;
        PointType m_sizeX;
        PointType m_sizeY;
        int m_implementation;
        std::unordered_map<int, Implementation> m_implementations;
        std::vector<Thread*> m_threads;

        // member methods
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    }; // class MandelbrotSet

} // namespace ng