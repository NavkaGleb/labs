#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "Random/Random.hpp"
#include "Fluid.cuh"

namespace ng {

#define CUDA_CALL(x) cudaError_t error = cudaGetLastError();                \
                     if (error != cudaSuccess) {                            \
                         std::cout << cudaGetErrorName(error) << std::endl; \
                         std::abort();                                      \
                     } x

    // constructor / destructor
    __host__ Fluid::Fluid(unsigned int width, unsigned int height, unsigned int scale)
            : m_size(width, height),
              m_scale(scale),
              m_elapsedTime(0.0f),
              m_timeSincePress(0.0f) {

        // init colors
        m_colors.resize(7);
        m_colors[0] = { 1.0f, 0.0f, 0.0f };
        m_colors[1] = { 0.0f, 1.0f, 0.0f };
        m_colors[2] = { 1.0f, 0.0f, 1.0f };
        m_colors[3] = { 1.0f, 1.0f, 0.0f };
        m_colors[4] = { 0.0f, 1.0f, 1.0f };
        m_colors[5] = { 1.0f, 0.0f, 1.0f };
        m_colors[6] = { 1.0f, 0.5f, 0.3f };
        m_color = m_colors[ng::Random::Irand<std::size_t>(0, m_colors.size() - 1)];

        // device memory
        const unsigned int fieldArea = m_size.x * m_size.y;
        CUDA_CALL(cudaSetDevice(0));
        cudaMalloc(&m_newField,    fieldArea * sizeof(kernel::Particle));
        cudaMalloc(&m_oldField,    fieldArea * sizeof(kernel::Particle));
        cudaMalloc(&m_newPressure, fieldArea * sizeof(float));
        cudaMalloc(&m_oldPressure, fieldArea * sizeof(float));
        cudaMalloc(&m_colorField,  fieldArea * 4 * sizeof(uint8_t));
        cudaMalloc(&m_vorticity,   fieldArea * sizeof(float));

        // host memory
        m_pixelBuffer.resize(fieldArea * 4);

        // init SFML stuff
        m_texture.create(m_size.x, m_size.y);
    }

    __host__ Fluid::~Fluid() {
        cudaFree(m_newField);
        cudaFree(m_oldField);
        cudaFree(m_newPressure);
        cudaFree(m_oldPressure);
        cudaFree(m_colorField);
        cudaFree(m_vorticity);
    }

    // public methods
    __host__ void Fluid::update(float dt, const sf::Vector2i& pos1, const sf::Vector2i& pos2, bool isActive) {
        // main function, calls vorticity -> diffusion -> force -> pressure -> project -> advect -> bloom -> paint
        m_threadCount = { m_systemConfig.xThreads, m_systemConfig.yThreads };
        m_blockCount = { m_size.x / m_threadCount.x, m_size.y / m_threadCount.y };

        updateVorticity(dt);
        updateDiffusion(dt);
        updateForce(dt, pos1, pos2, isActive);
        updatePressure(dt);
        updateProjection();
        updateAdvection(dt);
        updateBloom(pos2);
        updateTexture();

        Fluid::errorCheck();
        setTexture();
    }

    // member methods
    __host__ void Fluid::errorCheck() {
        cudaError_t error = cudaGetLastError();

        if (error != cudaSuccess)
            throw std::runtime_error(cudaGetErrorName(error));
    }

    __host__ void Fluid::updateVorticity(float dt) {
        kernel::computeVorticity<<<m_blockCount, m_threadCount>>>(m_vorticity, m_oldField, m_size.x, m_size.y);
        kernel::applyVorticity<<<m_blockCount, m_threadCount>>>(
            m_newField,         m_oldField,
            m_size.x,           m_size.y,
            m_vorticity,
            m_config.vorticity, dt);
        std::swap(m_oldField, m_newField);
    }

    __host__ void Fluid::updateDiffusion(float dt) {
        // performs several iterations over velocity and color fields
        // diffuse velocity and color

        for (int i = 0; i < m_systemConfig.velocityIterations; ++i) {
            kernel::diffuse<<<m_blockCount, m_threadCount>>>(
                m_newField,                 m_oldField,
                m_size.x,                   m_size.y,
                m_config.velocityDiffusion, dt
            );
            kernel::computeColor<<<m_blockCount, m_threadCount>>>(
                m_newField,              m_oldField,
                m_size.x,                m_size.y,
                m_config.colorDiffusion, dt
            );
            std::swap(m_newField, m_oldField);
        }
    }

    __host__ void Fluid::updateForce(float dt, const sf::Vector2i& pos1, const sf::Vector2i& pos2, bool isActive) {
        // apply force
        if (!isActive) {
            m_timeSincePress += dt;
        } else {
            m_timeSincePress = 0.0f;
            m_elapsedTime += dt;

            // apply gradient to color
            auto minIndex = static_cast<unsigned int>(m_elapsedTime)     % m_colors.size();
            auto maxIndex = static_cast<unsigned int>(m_elapsedTime + 1) % m_colors.size();
            float w = m_elapsedTime - std::floor(m_elapsedTime); // int(m_elapsedTime)
            m_color = m_colors[minIndex] * (1 - w) + m_colors[maxIndex] * w;

            kernel::Vector2f force {
                static_cast<float>(pos2.x - pos1.x) * m_config.forceScale,
                static_cast<float>(pos2.y - pos1.y) * m_config.forceScale
            };
            kernel::Vector2f pos = {
                static_cast<float>(pos2.x),
                static_cast<float>(pos2.y)
            };

            kernel::applyForce<<<m_blockCount, m_threadCount>>>(
                m_oldField,
                m_size.x,        m_size.y,
                m_color,
                force,           pos,
                m_config.radius, dt
            );
        }
    }

    __host__ void Fluid::updatePressure(float dt) {
        // performs several iterations over pressure field

        for (int i = 0; i < m_systemConfig.pressureIterations; ++i) {
            kernel::computePressureImpl<<<m_blockCount, m_threadCount>>>(
                m_oldField,
                m_size.x,          m_size.y,
                m_newPressure,     m_oldPressure,
                m_config.pressure
            );

            std::swap(m_oldPressure, m_newPressure);
        }
    }

    __host__ void Fluid::updateProjection() {
        kernel::project<<<m_blockCount, m_threadCount>>>(m_oldField, m_size.x, m_size.y, m_oldPressure);
        cudaMemset(m_oldPressure, 0, m_size.x * m_size.y * sizeof(float));
    }

    __host__ void Fluid::updateAdvection(float dt) {
        kernel::advect<<<m_blockCount, m_threadCount>>>(
            m_newField,                m_oldField,
            m_size.x,                  m_size.y,
            m_config.densityDiffusion, dt
        );
        std::swap(m_newField, m_oldField);
    }

    __host__ void Fluid::updateBloom(const sf::Vector2i& pos) {
        // apply bloom in mouse pos
        if (m_config.bloomEnabled && m_timeSincePress < 5.0f)
            kernel::applyBloom<<<m_blockCount, m_threadCount>>>(
                m_colorField,
                m_size.x,
                m_size.y,
                pos.x,
                pos.y,
                m_config.radius,
                m_config.bloomIntense
            );
    }

    __host__ void Fluid::updateTexture() {
        // paint image
        kernel::paint<<<m_blockCount, m_threadCount>>>(m_colorField, m_oldField, m_size.x, m_size.y);

        // copy image to cpu
        cudaMemcpy(
                m_pixelBuffer.data(),
                m_colorField,
                m_size.x * m_size.y * 4 * sizeof(uint8_t),
                cudaMemcpyDeviceToHost
        );
    }

    __host__ void Fluid::setTexture() {
        m_texture.update(m_pixelBuffer.data());
        m_sprite.setTexture(m_texture);
        m_sprite.setScale({ static_cast<float>(m_scale), static_cast<float>(m_scale) });
    }

    __host__ void Fluid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_sprite);
    }

} // namespace ng