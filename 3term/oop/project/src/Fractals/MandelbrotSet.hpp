#pragma once

// std lib
#include <string>
#include <unordered_map>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>

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
//        todo: implement long arithmetic for doubles
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

    private:
        // inner structs
        struct Implementation {
            // data
            std::string name;
            std::function<void(const float&, std::size_t, std::size_t, std::size_t, std::size_t)> func;

            // operators
            void operator()(const float& ftime,
                            std::size_t startI, std::size_t endI,
                            std::size_t startJ, std::size_t endJ) const { func(ftime, startI, endI, startJ, endJ); };
        };

        class Thread {
        public:
            // static
            static std::atomic<int> complete;

            // constructor / destructor
            Thread() : m_iterations(0), m_alive(true) {}
            ~Thread() {
                m_thread.join();
            };

            // accessors
            [[nodiscard]] inline std::function<void(const float&, std::size_t, std::size_t, std::size_t, std::size_t)>& getFunc() { return m_func; }

            // modifiers
            void setThread(std::thread&& thread) { m_thread = std::move(thread); }
            void setFunc(const std::function<void(const float&, std::size_t, std::size_t, std::size_t, std::size_t)>& func) {
                m_func = func;
            }

            // public methods
            void launch() {
                std::unique_lock<std::mutex> lm(m_mutex);
                m_start.notify_one();
            }

            void create(const float& ftime, std::size_t bi, std::size_t endi, std::size_t bj, std::size_t endj) {
                m_func(ftime, bi, endi, bj, endj);
                ++complete;
            }

        public:
            // member data
            int m_iterations;
            sf::Vector2<PointType> m_tl;
            sf::Vector2<PointType> m_br;
            std::condition_variable m_start;
            bool m_alive;
            std::mutex m_mutex;
            std::thread m_thread;
            std::function<void(const float&, std::size_t, std::size_t, std::size_t, std::size_t)> m_func;

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

        // member methods
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    }; // class MandelbrotSet

} // namespace ng