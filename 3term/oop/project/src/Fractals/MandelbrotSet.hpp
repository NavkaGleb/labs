#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include <cmath>
#include <vector>
#include <iostream>

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
        // enums
        enum class Bounds : short { MinX = 0, MaxX, MinY, MaxY };
        enum class ImplementationType : short {
            Pseudocode = 0,
            ByHand,
            AVX2,
            Threads,
            ThreadPool
        };

        // aliases
        using PointType       = double;
        using BoundsContainer = std::unordered_map<Bounds, PointType>;

        // constructor / destructor
        explicit MandelbrotSet(const sf::Vector2u& size);
        ~MandelbrotSet() override;

        // accessors
        [[nodiscard]] inline int getIterations() const { return m_iterations; }
        [[nodiscard]] inline BoundsContainer getLocalBounds() const { return m_bounds; }
        [[nodiscard]] inline const sf::Vector2<PointType>& getSize() const { return m_size; }
        [[nodiscard]] inline ImplementationType getImplementation() const { return m_implementation; }
        [[nodiscard]] inline const std::string& getImplementationName() const {
            return m_implementations.at(m_implementation).name;
        }
        [[nodiscard]] inline bool getColoring() const { return m_coloring; }

        // modifiers
        void setImplementation(const ImplementationType& type);
        void setColoring(bool coloring);

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
            std::function<void(std::size_t, std::size_t, bool)> func;

            // operators
            void operator()(std::size_t start, std::size_t end, bool setImage) const {
                return func(start, end, setImage);
            }

        }; // struct Implementation

        // aliases
        using ImplementationContainer = std::unordered_map<ImplementationType, Implementation>;

        // TODO: zoom!
        // member data
        sf::Image               m_image;
        sf::Texture             m_texture;
        sf::Sprite              m_sprite;
        sf::Vector2<PointType>  m_size;
        sf::Vector2<PointType>  m_scale;
        BoundsContainer         m_bounds;
        PointType               m_zoom;
        ImplementationType      m_implementation;
        ImplementationContainer m_implementations;
        ThreadPool              m_threadPool;
        unsigned                m_iterations;
        bool                    m_coloring;

        // member methods
        sf::Color getColor(int iterations);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    }; // class MandelbrotSet

} // namespace ng