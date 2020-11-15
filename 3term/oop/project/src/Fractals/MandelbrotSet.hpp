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

// sfml lib
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <ThreadPool/ThreadPool.hpp>

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
        void implementation2(std::size_t si, std::size_t ei, bool setImage = true);
        void implementation3();
        void implementation4();
        void implementation5(std::size_t si, std::size_t ei, bool setImage = true);

    private:
        // inner structs
        struct Implementation {
            // data
            std::string name;
        }; // struct Implementation

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
        ThreadPool m_threadPool;

        // member methods
        sf::Color getColor(int iterations);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    }; // class MandelbrotSet

} // namespace ng