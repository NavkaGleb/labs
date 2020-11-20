#include "Fluid.hpp"

namespace ng {

    // constructor / destructor
    Fluid::Fluid(int size, float deltaTime, float diffusion, float viscosity)
        : m_size(size),
          m_deltaTime(deltaTime),
          m_diffusion(diffusion),
          m_viscosity(viscosity) {

        int planeSize = size * size;

        m_s.resize(planeSize);
        m_density.resize(planeSize);
        m_velocity[Dimension::X].resize(planeSize);
        m_velocity[Dimension::Y].resize(planeSize);
        m_prevVelocity[Dimension::X].resize(planeSize);
        m_prevVelocity[Dimension::Y].resize(planeSize);
    }

    // public methods
    void Fluid::addDensity(std::size_t x, std::size_t y, float amount) {
        m_density[toLine(x, y)] += amount;
    }

    void Fluid::addVelocity(std::size_t x, std::size_t y, const std::vector<float>& amount) {
        std::size_t index = toLine(x, y);

        m_velocity[Dimension::X][index] += amount[0];
        m_velocity[Dimension::Y][index] += amount[1];
    }

    // member methods
    void Fluid::draw(sf::RenderTarget& target, sf::RenderStates states) const {

    }

    std::size_t Fluid::toLine(std::size_t x, std::size_t y) const {
        return x * m_size + y;
    }

} // namespace ng