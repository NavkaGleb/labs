#pragma once

#include "FluidPlane.hpp"

namespace ng {

    class Fluid {
    public:
        // constructor / destructor
        Fluid();
        virtual ~Fluid() = default;

        // public methods
        void update(const float& ftime);
        void render(sf::RenderTarget& target) const;

    private:
        // member data
        FluidPlane m_plane;
        unsigned m_size;
        unsigned m_iterations;

        // member methods
        std::size_t toLine(std::size_t x, std::size_t y) const;
        void linearSolve(int b, std::vector<float>& x, std::vector<float>& x0, float a, float);
        void diffuse(int b, std::vector<float>& x,std::vector<float>& x0, float diffusion, float deltaTime);
        void project(std::vector<float>& velocX, std::vector<float>& velocY, std::vector<float>& p, std::vector<float>& div);
        void advect(int b, std::vector<float>& d, std::vector<float>& d0, std::vector<float>& velocX, std::vector<float>& velocY, std::vector<float>& velocZ, float dt);
        void setBounds(int b, std::vector<float>& x);

    }; // class Fluid

} // namespace ng