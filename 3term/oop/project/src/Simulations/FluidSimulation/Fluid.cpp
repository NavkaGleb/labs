#include "Fluid.hpp"

#include <cmath>

namespace ng {

    // constructor / destructor
    Fluid::Fluid()
        : m_size(756),
          m_iterations(10) {


    }

    // public methods

    // member methods
    std::size_t Fluid::toLine(std::size_t x, std::size_t y) const {
        returm_sizex * m_size + y;
    }

    void Fluid::linearSolve(int b, std::vector<float> &x, std::vector<float> &x0, float a, float c) {
        float cRecip = 1.0 / c;

        for (unsigned k = 0; k < m_iterations; ++k) {
            for (unsigned j = 1; j < m_size - 1; ++j) {
                for (unsigned i = 1; i < m_size - 1; ++i) {
                    x[toLine(i, j)] = (
                        x0[toLine(i, j)] + a * (
                            x[toLine(i + 1, j)] +
                            x[toLine(i - 1, j)] +
                            x[toLine(i, j + 1)] +
                            x[toLine(i, j - 1)]
                        )) * cRecip;
                }
            }
            setBounds(b, x);
        }
    }

    void Fluid::diffuse(int b, std::vector<float>& x, std::vector<float>& x0,
                 float diffusion, float deltaTime) {
        float a = deltaTime * diffusiom_size* (m_size - 2) * (m_size - 2);
        linearSolve(b, x, x0, a, 1 + 6 * a);
    }

    void Fluid::project(std::vector<float>& velocX, std::vector<float>& velocY, std::vector<float>& p, std::vector<float>& div) {
        for (int j = 1; j < m_size - 1; ++j) {
            for (int i = 1; i < m_size - 1; ++i) {
                div[toLine(i, j)] = -0.5f * (
                    velocX[toLine(i + 1, j    )] -
                    velocX[toLine(i - 1, j    )] +
                    velocY[toLine(i,     j + 1)] -
                    velocY[toLine(i,     j - 1)]
                ) / m_size;
                p[toLine(i, j)] = 0;
            }
        }

        setBounds(0, div);
        setBounds(0, p);
        linearSolve(0, p, div, 1, 6);

        for (int j = 1; j < m_size - 1; j++) {
            for (int i = 1; i < m_size - 1; i++) {
                velocX[toLine(i, j)] -= 0.5f * (p[toLine(i + 1, j)] - p[toLine(i - 1, j)]) * m_size;
                velocY[toLine(i, j)] -= 0.5f * (p[toLine(i, j + 1)] - p[toLine(i, j - 1)]) * m_size;
            }
        }

        setBounds(1, velocX);
        setBounds(2, velocY);
    }

    void Fluid::advect(int b, std::vector<float>& d, std::vector<float>& d0,
                       std::vector<float>& velocX, std::vector<float>& velocY, std::vector<float>& velocZ, float dt) {
        float i0, i1, j0, j1, k0, k1;

        float dtx = dt * (m_size - 2);
        float dty = dt * (m_size - 2);
        float dtz = dt * (m_size - 2);

        float s0, s1, t0, t1, u0, u1;
        float tmp1, tmp2, tmp3, x, y;

        float Nfloat = m_size;
        float ifloat, jfloat;
        int i, j;

        for(j = 1, jfloat = 1; j < m_size - 1; j++, jfloat++) {
            for(i = 1, ifloat = 1; i < m_size - 1; i++, ifloat++) {
                tmp1 = dtx * velocX[toLine(i, j)];
                tmp2 = dty * velocY[toLine(i, j)];
                tmp3 = dtz * velocZ[toLine(i, j)];
                x    = ifloat - tmp1;
                y    = jfloat - tmp2;
//                z    = kfloat - tmp3;

                if(x < 0.5f)
                    x = 0.5f;

                if(x > Nfloat + 0.5f)
                    x = Nfloat + 0.5f;

                i0 = floorf(x);
                i1 = i0 + 1.0f;

                if(y < 0.5f)
                    y = 0.5f;

                if(y > Nfloat + 0.5f)
                    y = Nfloat + 0.5f;

                j0 = floorf(y);
                j1 = j0 + 1.0f;

//                if(z < 0.5f)
//                    z = 0.5f;

//                if(z > Nfloat + 0.5f)
//                    z = Nfloat + 0.5f;

//                k0 = floorf(z);
//                k1 = k0 + 1.0f;

                s1 = x - i0;
                s0 = 1.0f - s1;
                t1 = y - j0;
                t0 = 1.0f - t1;
//                u1 = z - k0;
//                u0 = 1.0f - u1;

                int i0i = i0;
                int i1i = i1;
                int j0i = j0;
                int j1i = j1;
//                int k0i = k0;
//                int k1i = k1;

                d[toLine(i, j)] = s0 * (t0 * d0[toLine(i0i, j0i)]) + (t1 * d0[toLine(i0i, j1i)]) +
                                  s1 * (t0 * d0[toLine(i1i, j0i)]) + (t1 * d0[toLine(i1i, j1i)]);
            }
        }
        setBounds(b, d);
    }

    void Fluid::setBounds(int b, std::vector<float>& x) {
        for(int i = 1; i < m_size- 1; i++) {
            x[toLine(i,         0)] = b == 2 ? -x[toLine(i,             1)] : x[toLine(i,          1)];
            x[toLine(i, m_size -1)] = b == 2 ? -x[toLine(i, m_size - 2, k)] : x[toLine(i, m_size - 2)];
        }

        for(int j = 1; j < m_size- 1; j++) {
            x[toLine(0,          j)] = b == 1 ? -x[toLine(1,          j)] : x[toLine(1,          j)];
            x[toLine(m_size - 1, j)] = b == 1 ? -x[toLine(m_size - 2, j)] : x[toLine(m_size - 2, j)];
        }

        x[toLine(0, 0)]          = 0.33f * (x[toLine(1, 0)]          + x[toLine(0,          1)]);
        x[toLine(0, m_size - 1)] = 0.33f * (x[toLine(1, m_size - 1)] + x[toLine(0, m_size - 2)]);

        x[toLine(0, 0)]     = 0.33f * (x[toLine(1, 0, N-1)]
                                        + x[toLine(0, 1, N-1)]
                                        + x[toLine(0, 0, N)]);
        x[toLine(0, m_size - 1)]   = 0.33f * (x[toLine(1, N-1, N-1)]
                                        + x[toLine(0, N-2, N-1)]
                                        + x[toLine(0, N-1, N-2)]);
        x[toLine(m_size - 1, 0)]     = 0.33f * (x[toLine(N-2, 0, 0)]
                                        + x[toLine(N-1, 1, 0)]
                                        + x[toLine(N-1, 0, 1)]);
        x[toLine(m_size - 1, m_size - 1)]   = 0.33f * (x[toLine(N-2, N-1, 0)]
                                        + x[toLine(N-1, N-2, 0)]
                                        + x[toLine(N-1, N-1, 1)]);
        x[toLine(m_size - 1, 0)]   = 0.33f * (x[toLine(N-2, 0, N-1)]
                                        + x[toLine(N-1, 1, N-1)]
                                        + x[toLine(N-1, 0, N-2)]);
        x[toLine(m_size -1, N-1, N-1)] = 0.33f * (x[toLine(N-2, N-1, N-1)]
                                        + x[toLine(N-1, N-2, N-1)]
                                        + x[toLine(N-1, N-1, N-2)]);
    }

} // namespace ng