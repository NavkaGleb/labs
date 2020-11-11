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
        using PointType = long double;

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

                    for (std::size_t i = bi; i < ei; i += 1) {
                        for (std::size_t j = 0; j < m_image.getSize().y; j += 1) {
                            PointType realZ = 0.0;
                            PointType imagineZ = 0.0;
                            PointType realC = m_topLeft.x + i / 800.0 * (m_bottomRight.x - m_topLeft.x);
                            PointType imagineC = m_topLeft.y + j / 800.0 * (m_bottomRight.y - m_topLeft.y);

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