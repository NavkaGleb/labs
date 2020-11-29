#include "Fluid.hpp"

#include <cmath>
#include <cstdlib>
#include <cmath>
#include <iostream>

//#define IX(x, y) ((x) + (y) * N)

int IX(int x, int y) {
    x = std::min(std::max(x, 0), 48);
    y = std::min(std::max(y, 0), 48);

    return x + y * 48;
}

namespace ng {

    // constructor / destructor
    Fluid::Fluid(int size, float diffusion, float viscosity) {
        int N = size;

        this->size = size;
        this->diff = diffusion;
        this->visc = viscosity;

        this->s = new float[N * N];
        this->density = new float[N * N];

        this->Vx = new float[N * N];
        this->Vy = new float[N * N];

        this->Vx0 = new float[N * N];
        this->Vy0 = new float[N * N];

        this->rect.resize(N * N, sf::RectangleShape());
    }

    Fluid::~Fluid() {
        delete[] this->s;
        delete[] this->density;

        delete[] this->Vx;
        delete[] this->Vy;

        delete[] this->Vx0;
        delete[] this->Vy0;
    }

    // public methods
    void Fluid::addDensity(int x, int y, float amount) {
//        std::cout << "x = " << x << ", y = " << y << std::endl;
//        std::cout << "this->density = " << this->density[IX(x, y)] << std::endl;
        if (this->density[IX(x, y)] < 500)
            this->density[IX(x, y)] += amount;
    }

    void Fluid::addVelocity(int x, int y, float amountX, float amountY) {
        int index = IX(x, y);

        if (this->Vx[index] < 10.0f)
            this->Vx[index] += amountX;

        if (this->Vy[index] < 10.0f)
            this->Vy[index] += amountY;

//        std::cout << "velocity x = " << this->Vx[index] << std::endl;
//        std::cout << "velocity y = " << this->Vy[index] << std::endl;
    }

    void Fluid::update(const float& ftime) {
        int N          = this->size;
        float visc     = this->visc;
        float diff     = this->diff;
        float dt       = ftime;
        float *Vx      = this->Vx;
        float *Vy      = this->Vy;
        float *Vx0     = this->Vx0;
        float *Vy0     = this->Vy0;
        float *s       = this->s;
        float *density = this->density;

        diffuse(1, Vx0, Vx, visc, dt, 4, N);
        diffuse(2, Vy0, Vy, visc, dt, 4, N);

        project(Vx0, Vy0, Vx, Vy, 4, N);

        advect(1, Vx, Vx0, Vx0, Vy0, dt, N);
        advect(2, Vy, Vy0, Vx0, Vy0, dt, N);

        project(Vx, Vy, Vx0, Vy0, 4, N);

        diffuse(0, s, density, diff, dt, 4, N);
        advect(0, density, s, Vx, Vy, dt, N);

        for (int i = 0; i < this->size; ++i) {
            for (int j = 0; j < this->size; ++j) {
                auto x = static_cast<float>(i * 16);
                auto y = static_cast<float>(j * 16);
                int index = IX(i, j);

                this->rect[index].setPosition(x, y);
                this->rect[index].setSize(sf::Vector2f(16, 16));
                this->rect[index].setFillColor(sf::Color(200, 200, 200, std::min<float>(this->density[IX(i, j)], 255)));
            }
        }

    }

    void Fluid::render(sf::RenderTarget &target) const {
        for (const auto& r : rect)
            target.draw(r);
    }

    // member methods

    void diffuse(int b, float *x, float *x0, float diff, float dt, int iter, int N)
    {
        float a = dt * diff * static_cast<float>(N - 2) * static_cast<float>(N - 2);
        lin_solve(b, x, x0, a, 1 + 6 * a, iter, N);
    }

    static void lin_solve(int b, float *x, const float *x0, float a, float c, int iter, int N)
    {
        float cRecip = 1.0 / c;
        for (int k = 0; k < iter; k++) {
            for (int j = 1; j < N - 1; j++) {
                for (int i = 1; i < N - 1; i++) {
                    x[IX(i, j)] =
                        (x0[IX(i, j)]
                         + a*(    x[IX(i+1, j)]
                                  +x[IX(i-1, j)]
                                  +x[IX(i, j+1)]
                                  +x[IX(i, j-1)]
                        )) * cRecip;
                }
            }

            set_bnd(b, x, N);
        }
    }

    static void project(float *velocX, float *velocY, float *p, float *div, int iter, int N)
    {
        for (int j = 1; j < N - 1; j++) {
            for (int i = 1; i < N - 1; i++) {
                div[IX(i, j)] = -0.5f*(
                    velocX[IX(i+1, j)]
                    -velocX[IX(i-1, j)]
                    +velocY[IX(i, j+1)]
                    -velocY[IX(i, j-1)]
                )/N;
                p[IX(i, j)] = 0;
            }
        }

        set_bnd(0, div, N);
        set_bnd(0, p, N);
        lin_solve(0, p, div, 1, 6, iter, N);

        for (int j = 1; j < N - 1; j++) {
            for (int i = 1; i < N - 1; i++) {
                velocX[IX(i, j)] -= 0.5f * (  p[IX(i+1, j)]
                                              -p[IX(i-1, j)]) * N;
                velocY[IX(i, j)] -= 0.5f * (  p[IX(i, j+1)]
                                              -p[IX(i, j-1)]) * N;
            }
        }
        set_bnd(1, velocX, N);
        set_bnd(2, velocY, N);
    }

    static void advect(int b, float *d, const float *d0,  const float *velocX, const float *velocY, float dt, int N)
    {
        float i0, i1, j0, j1;

        float dtx = dt * (N - 2);
        float dty = dt * (N - 2);

        float s0, s1, t0, t1;
        float tmp1, tmp2, x, y;

        float Nfloat = N;
        float ifloat, jfloat;
        int i, j;

        for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
            for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++) {
                tmp1 = dtx * velocX[IX(i, j)];
                tmp2 = dty * velocY[IX(i, j)];
                x    = ifloat - tmp1;
                y    = jfloat - tmp2;

                if (x < 0.5f) x = 0.5f;
                if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
                i0 = floor(x);
                i1 = i0 + 1.0f;
                if (y < 0.5f) y = 0.5f;
                if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
                j0 = floor(y);
                j1 = j0 + 1.0f;

                s1 = x - i0;
                s0 = 1.0f - s1;
                t1 = y - j0;
                t0 = 1.0f - t1;

                int i0i = int(i0);
                int i1i = int(i1);
                int j0i = int(j0);
                int j1i = int(j1);

                // DOUBLE CHECK THIS!!!
                d[IX(i, j)] =
                    s0 * (t0 * d0[IX(i0i, j0i)] + t1 * d0[IX(i0i, j1i)]) +
                    s1 * (t0 * d0[IX(i1i, j0i)] + t1 * d0[IX(i1i, j1i)]);
            }
        }

        set_bnd(b, d, N);
    }

    static void set_bnd(int b, float *x, int N)
    {
        for (int i = 1; i < N - 1; i++) {
            x[IX(i, 0  )] = b == 2 ? -x[IX(i, 1  )] : x[IX(i, 1 )];
            x[IX(i, N-1)] = b == 2 ? -x[IX(i, N-2)] : x[IX(i, N-2)];
        }
        for (int j = 1; j < N - 1; j++) {
            x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
            x[IX(N-1, j)] = b == 1 ? -x[IX(N-2, j)] : x[IX(N-2, j)];
        }

        x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
        x[IX(0, N-1)] = 0.5f * (x[IX(1, N-1)] + x[IX(0, N-2)]);
        x[IX(N-1, 0)] = 0.5f * (x[IX(N-2, 0)] + x[IX(N-1, 1)]);
        x[IX(N-1, N-1)] = 0.5f * (x[IX(N-2, N-1)] + x[IX(N-1, N-2)]);
    }

} // namespace ng