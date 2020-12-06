#pragma once

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cuda_runtime_api.h>
#include <cudart_platform.h>

#include <cstdint>

#include <SFML/System/Vector2.hpp>

namespace ng { namespace kernel {

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Vector2f
    ////////////////////////////////////////////////////////////////////////////////////////////
    struct Vector2f {
        // data
        float x = 0.0f;
        float y = 0.0f;

        // operators
        __device__ Vector2f operator-(const Vector2f& other) const;
        __device__ Vector2f operator+(const Vector2f& other) const;
        __device__ Vector2f operator*(float d) const;

    }; // struct Vector2f

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Color3f
    ////////////////////////////////////////////////////////////////////////////////////////////
    struct Color3f {
        // data
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;

        // operators
        __host__ __device__ Color3f operator+(const Color3f& other) const;
        __host__ __device__ Color3f operator*(float d) const;

    }; // struct Color3f

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Particle
    ////////////////////////////////////////////////////////////////////////////////////////////
    struct Particle {
        // data
        kernel::Vector2f velocity;
        kernel::Color3f color;

    }; // struct Particle

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Kernel Functions
    ////////////////////////////////////////////////////////////////////////////////////////////
    __device__ float curl(const Particle* field, std::size_t width, std::size_t height, int x, int y);
    __device__ Vector2f absGradient(const float* field, std::size_t width, std::size_t height, int x, int y);
    __device__ Vector2f gradient(const float* field, std::size_t width, std::size_t height, int x, int y);
    __device__ Particle interpolate(Vector2f velocity, const Particle* field, std::size_t width, std::size_t height);
    __device__ Vector2f jacobiVelocity(const Particle* field,
                                       std::size_t width, std::size_t height,
                                       Vector2f v, Vector2f B,
                                       float alpha, float beta);
    __device__ Color3f jacobiColor(const Particle* field,
                                   std::size_t width, std::size_t height,
                                   Vector2f pos, Color3f B,
                                   float alpha, float beta);
    __device__ float jacobiPressure(const float* field,
                                    std::size_t width, std::size_t height, int x, int y,
                                    float B, float alpha, float beta);
    __device__ float divergence(const Particle* field, std::size_t width, std::size_t height, int x, int y);

    __global__ void computeVorticity(float* vorticityField, Particle* field, size_t xSize, size_t ySize);
    __global__ void applyVorticity(Particle* newField, Particle* oldField,
                                   std::size_t width, std::size_t height,
                                   float* vorticityField, float vorticity, float dt);
    __global__ void applyForce(Particle* field,
                               std::size_t width, std::size_t height,
                               Color3f color, Vector2f force, Vector2f pos, int radius, float dt);
    __global__ void project(Particle* newField, std::size_t width, std::size_t height, const float* pressureField);
    __global__ void advect(Particle* newField, Particle* oldField,
                           std::size_t width, std::size_t height,
                           float densityDiffusion, float dt);
    __global__ void diffuse(Particle* newField, Particle* oldField,
                            std::size_t width, std::size_t height,
                            float velocityDiffusion, float dt);
    __global__ void computeColor(Particle* newField, Particle* oldField,
                                 std::size_t width, std::size_t height,
                                 float colorDiffusion, float dt);
    __global__ void computePressureImpl(Particle* field,
                                        std::size_t width, std::size_t height,
                                        float* newPressure, float* oldPressure,
                                        float pressure);
    __global__ void paint(uint8_t* colorField, const Particle* field, std::size_t width, std::size_t height);
    __global__ void applyBloom(uint8_t* colorField,
                               std::size_t width, std::size_t height,
                               int positionX, int positionY,
                               float radius, float bloomIntense);

} } // namespace ng::kernel