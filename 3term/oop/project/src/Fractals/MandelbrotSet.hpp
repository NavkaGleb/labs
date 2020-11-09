#pragma once

// std lib
#include <string>
#include <unordered_map>
#include <functional>

// sfml lib
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace ng {

    class MandelbrotSet : public sf::Drawable {
    public:
        // constructor / destructor
        MandelbrotSet();
        ~MandelbrotSet() override;

        // accessors
        [[nodiscard]] inline int getIterations() const { return m_iterations; }
        [[nodiscard]] inline double getLeft() const { return m_left; }
        [[nodiscard]] inline double getRight() const { return m_right; }
        [[nodiscard]] inline double getBottom() const { return m_bottom; }
        [[nodiscard]] inline double getTop() const { return m_top; }
        [[nodiscard]] inline const sf::Vector2<double>& getSize() const { return m_size; }
        [[nodiscard]] inline int getImplementation() const { return m_implementation; }
        [[nodiscard]] inline const std::string& getImplementationName() const {
            return m_implementations.at(m_implementation).name;
        }

        // modifiers
        void setSize(const sf::Vector2<double>& size);
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

        // member data
        sf::Image m_image;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2<double> m_size;
        int m_iterations;
        double m_left;
        double m_right;
        double m_bottom;
        double m_top;
        double m_sizeX;
        double m_sizeY;
        int m_implementation;
        std::unordered_map<int, Implementation> m_implementations;

        // member methods
        void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    }; // class MandelbrotSet

} // namespace ng