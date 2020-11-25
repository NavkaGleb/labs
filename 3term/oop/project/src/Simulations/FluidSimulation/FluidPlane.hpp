#pragma once

#include <vector>
#include <unordered_map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/OpenGl.hpp>

namespace ng {

    class FluidPlane : public sf::Drawable {
    public:
        // constructor / destructor
        FluidPlane() = default;
        FluidPlane(int size, float deltaTime, float diffusion, float viscosity);
        ~FluidPlane() override = default;

        // public methods
        void addDensity(std::size_t x, std::size_t y, float amount);
        void addVelocity(std::size_t x, std::size_t y, const std::vector<float>& amount);

    private:
        // enums
        enum class Dimension { X = 0, Y };

        // aliases
        using VelocityContainer = std::unordered_map<Dimension, std::vector<float>>;

        // data
        int                m_size;
        float              m_deltaTime;
        float              m_diffusion;
        float              m_viscosity;
        std::vector<float> m_s;
        std::vector<float> m_density;
        VelocityContainer  m_velocity;
        VelocityContainer  m_prevVelocity;

        // member methods
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::size_t toLine(std::size_t x, std::size_t y) const;


    }; // class Fluid

} // namespace ng