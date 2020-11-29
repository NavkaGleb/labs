#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace ng {

    class Fluid {
    public:
        // constructor / destructor
        Fluid(int size, float diffusion, float viscosity);
        virtual ~Fluid();

        // public methods
        void addDensity(int x, int y, float amount);
        void addVelocity(int x, int y, float amountX, float amountY);

        void update(const float& ftime);
        void render(sf::RenderTarget& target) const;

    private:
        int size;
        float diff;
        float visc;

        float *s;
        float *density;

        float *Vx;
        float *Vy;

        float *Vx0;
        float *Vy0;

        std::vector<sf::RectangleShape> rect;

    }; // class Fluid

    static void diffuse (int b, float *x, float *x0, float diff, float dt, int iter, int N);
    static void lin_solve(int b, float *x, const float *x0, float a, float c, int iter, int N);
    static void project(float *velocX, float *velocY, float *p, float *div, int iter, int N);
    static void advect(int b, float *d, const float *d0,  const float *velocX, const float *velocY, float dt, int N);
    static void set_bnd(int b, float *x, int N);

} // namespace ng