#pragma once

#include <cstdint>
#include <cmath>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "Kernel.cuh"

namespace ng {

    class Fluid : public sf::Drawable {
    public:
        // constructor / destructor
        __host__ explicit Fluid(unsigned int width, unsigned int height, unsigned int scale);
        __host__ ~Fluid() override;

        // accessors
        __host__ inline unsigned int getScale() const { return m_scale; }

        // public methods
        __host__ void update(float dt, const sf::Vector2i& pos1, const sf::Vector2i& pos2, bool isPressed);

    private:
        // inner structs
        struct Config {
            float pressure          = 1.5f;
            float vorticity         = 50.0f;
            float velocityDiffusion = 0.3f;
            float colorDiffusion    = 0.8f;
            float densityDiffusion  = 1.2f;
            float forceScale        = 5000.0f;
            float bloomIntense      = 0.1f;
            int   radius            = 400;
            bool  bloomEnabled      = true;
        } m_config; // struct Config

        struct SystemConfig {
            int velocityIterations = 20;
            int pressureIterations = 40;
            unsigned int xThreads  = 32;
            unsigned int yThreads  = 1;
        } m_systemConfig; // struct SystemConfig

        // member data
        sf::Vector2u m_size;
        unsigned int m_scale;

        std::vector<kernel::Color3f> m_colors;
        kernel::Color3f m_color;

        kernel::Particle* m_newField;
        kernel::Particle* m_oldField;
        float* m_newPressure;
        float* m_oldPressure;
        uint8_t* m_colorField;
        float* m_vorticity;

        float  m_elapsedTime;
        float  m_timeSincePress;

        dim3 m_blockCount;
        dim3 m_threadCount;

        std::vector<uint8_t> m_pixelBuffer;
        sf::Texture m_texture;
        sf::Sprite m_sprite;

        // member methods
        __host__ static void errorCheck();
        __host__ void updateVorticity(float dt);
        __host__ void updateDiffusion(float dt);
        __host__ void updateForce(float dt, const sf::Vector2i& pos1, const sf::Vector2i& pos2, bool isActive);
        __host__ void updatePressure(float dt);
        __host__ void updateProjection();
        __host__ void updateAdvection(float dt);
        __host__ void updateBloom(const sf::Vector2i& pos);
        __host__ void updateTexture();
        __host__ void setTexture();
        __host__ void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    }; // class Fluid

} // namespace ng