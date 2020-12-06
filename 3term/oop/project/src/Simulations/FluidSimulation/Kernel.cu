#include "Kernel.cuh"

#define SET_TO(p, x, y) if (x < width && x >= 0 && y < height && y >= 0)\
                            p = field[int(x) + int(y) * width]

#define GET_AVG(value, minValue, maxValue) min(maxValue, max(minValue, value))
#define SET_AVG(p, x, y) p = field[int(GET_AVG(x, 0.0f, width - 1.0f)) + int(GET_AVG(y, 0.0f, height - 1.0f)) * width]

namespace ng { namespace kernel {

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Vector2f
    ////////////////////////////////////////////////////////////////////////////////////////////
    // operators
    __device__ Vector2f Vector2f::operator-(const Vector2f& other) const {
        Vector2f res;

        res.x = x - other.x;
        res.y = y - other.y;

        return res;
    }

    __device__ Vector2f Vector2f::operator+(const Vector2f& other) const {
        Vector2f res;

        res.x = x + other.x;
        res.y = y + other.y;

        return res;
    }

    __device__ Vector2f Vector2f::operator*(float d) const {
        Vector2f res;

        res.x = x * d;
        res.y = y * d;

        return res;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Color3f
    ////////////////////////////////////////////////////////////////////////////////////////////
    // operators
    __host__ __device__ Color3f Color3f::operator+(const Color3f& other) const {
        Color3f res;

        res.r = r + other.r;
        res.g = g + other.g;
        res.b = b + other.b;

        return res;
    }

    __host__ __device__ Color3f Color3f::operator*(float d) const {
        Color3f res;

        res.r = r * d;
        res.g = g * d;
        res.b = b * d;

        return res;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    /// Kernel Functions
    ////////////////////////////////////////////////////////////////////////////////////////////
    __device__ float curl(const Particle* field, std::size_t width, std::size_t height, int x, int y) {
        // computes curl of velocity field
        Vector2f defaultValue = field[y * width + x].velocity;
        Vector2f vec1 = { -defaultValue.x, -defaultValue.y };
        Vector2f vec2 = { -defaultValue.x, -defaultValue.y };

        SET_TO(vec1.x, x + 1, y    ).velocity.x;
        SET_TO(vec2.x, x - 1, y    ).velocity.x;
        SET_TO(vec1.y, x,     y + 1).velocity.y;
        SET_TO(vec2.y, x,     y - 1).velocity.y;

        return ((vec1.y - vec2.y) - (vec1.x - vec2.x)) * 0.5f;
    }

    __device__ Vector2f absGradient(const float* field, std::size_t width, std::size_t height, int x, int y) {
        // computes absolute value gradient of vorticity field
        float defaultValue = field[y * width + x];
        Vector2f vec1 = { defaultValue, defaultValue };
        Vector2f vec2 = { defaultValue, defaultValue };

        SET_TO(vec1.x, x + 1, y    );
        SET_TO(vec2.x, x - 1, y    );
        SET_TO(vec1.y, x,     y + 1);
        SET_TO(vec2.y, x,     y - 1);

        return { (abs(vec1.x) - abs(vec2.x)) * 0.5f, (abs(vec1.y) - abs(vec2.y)) * 0.5f };
    }

    __device__ Vector2f gradient(const float* field, std::size_t width, std::size_t height, int x, int y) {
        // computes gradient of pressure field
        float defaultValue = field[y * width + x];
        Vector2f vec1 = { defaultValue, defaultValue };
        Vector2f vec2 = { defaultValue, defaultValue };

        SET_TO(vec1.x, x + 1, y    );
        SET_TO(vec2.x, x - 1, y    );
        SET_TO(vec1.y, x,     y + 1);
        SET_TO(vec2.y, x,     y - 1);

        return { (vec1.x - vec2.x) * 0.5f, (vec1.y - vec2.y) * 0.5f };
    }

    __device__ Particle interpolate(Vector2f velocity, const Particle* field, std::size_t width, std::size_t height) {
        // interpolates quantity of grid cells
        Vector2f vec1 = { floor(velocity.x),        floor(velocity.y)        };
        Vector2f vec2 = { floor(velocity.x + 1.0f), floor(velocity.y + 1.0f) };
        Particle particles[4];

        SET_AVG(particles[0], vec1.x, vec1.y);
        SET_AVG(particles[1], vec1.x, vec2.y);
        SET_AVG(particles[2], vec2.x, vec1.y);
        SET_AVG(particles[3], vec2.x, vec2.y);

        float t1 = (vec2.x - velocity.x) / (vec2.x - vec1.x);
        float t2 = (velocity.x - vec1.x) / (vec2.x - vec1.x);
        float t3 = (vec2.y - velocity.y) / (vec2.y - vec1.y);
        float t4 = (velocity.y - vec1.y) / (vec2.y - vec1.y);

        Vector2f f1 = particles[0].velocity * t1 + particles[2].velocity * t2;
        Vector2f f2 = particles[1].velocity * t1 + particles[3].velocity * t2;

        Color3f color1 = particles[1].color * t1 + particles[3].color * t2;
        Color3f color2 = particles[1].color * t1 + particles[3].color * t2;

        return { f1 * t3 + f2 * t4, color1 * t3 + color2 * t4 };
    }

    __device__ Vector2f jacobiVelocity(const Particle* field,
                                       std::size_t width, std::size_t height,
                                       Vector2f position, Vector2f velocity,
                                       float alpha, float beta) {
        // performs iteration of jacobi method on velocity grid field
        Vector2f velocityLeft;
        Vector2f velocityRight;
        Vector2f velocityUp;
        Vector2f velocityDown;

        SET_TO(velocityLeft,  position.x - 1, position.y    ).velocity;
        SET_TO(velocityRight, position.x + 1, position.y    ).velocity;
        SET_TO(velocityUp,    position.x,     position.y - 1).velocity;
        SET_TO(velocityDown,  position.x,     position.y + 1).velocity;

        return (velocityUp + velocityDown + velocityLeft + velocityRight + velocity * alpha) * (1.0f / beta);
    }

    __device__ Color3f jacobiColor(const Particle* field,
                                   std::size_t width, std::size_t height,
                                   Vector2f pos, Color3f b,
                                   float alpha, float beta) {
        // performs iteration of jacobi method on color grid field
        Color3f colorLeft;
        Color3f colorRight;
        Color3f colorUp;
        Color3f colorDown;
        auto x = static_cast<int>(pos.x);
        auto y = static_cast<int>(pos.y);

        SET_TO(colorLeft,  x - 1, y    ).color;
        SET_TO(colorRight, x + 1, y    ).color;
        SET_TO(colorUp,    x,     y - 1).color;
        SET_TO(colorDown,  x,     y + 1).color;

        return (colorUp + colorDown + colorLeft + colorRight + b * alpha) * (1.0f / beta);
    }

    __device__ float jacobiPressure(const float* field,
                                    std::size_t width, std::size_t height,
                                    int x, int y,
                                    float b, float alpha, float beta) {
        // performs iteration of jacobi method on pressure grid field
        float defaultPressure = field[y * width + x];
        float pressureLeft = defaultPressure;
        float pressureRight = defaultPressure;
        float pressureUp = defaultPressure;
        float pressureDown = defaultPressure;

        SET_TO(pressureLeft,  x - 1, y    );
        SET_TO(pressureRight, x + 1, y    );
        SET_TO(pressureUp,    x,     y - 1);
        SET_TO(pressureDown,  x,     y + 1);

        return (pressureUp + pressureDown + pressureLeft + pressureRight + alpha * b) * (1.0f / beta);
    }

    __device__ float divergence(const Particle* field, std::size_t width, std::size_t height, int x, int y) {
        // computes divergence of velocity field
        const Particle& particle = field[y * width + x];

        Vector2f vec1 = { -particle.velocity.x, -particle.velocity.y };
        Vector2f vec2 = { -particle.velocity.x, -particle.velocity.y };

        SET_TO(vec1.x, x + 1, y    ).velocity.x;
        SET_TO(vec2.x, x - 1, y    ).velocity.x;
        SET_TO(vec1.y, x,     y + 1).velocity.y;
        SET_TO(vec2.y, x,     y - 1).velocity.y;

        return (vec1.x - vec2.x + vec1.y - vec2.y) * 0.5f;
    }

    __global__ void computeVorticity(float* vorticityField, Particle* field,
                                     std::size_t width, std::size_t height) {
        // computes vorticity field which should be passed to applyVorticity function
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        vorticityField[y * width + x] = curl(field, width, height, x, y);
    }

    __global__ void applyVorticity(Particle* newField, Particle* oldField,
                                   std::size_t width, std::size_t height,
                                   float* vorticityField, float vorticity, float dt) {
        // applies vorticity to velocity field
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        Particle& newParticle = newField[y * width + x];
        Particle& oldParticle = oldField[y * width + x];

        Vector2f newVorticity = absGradient(vorticityField, width, height, x, y);
        newVorticity.y *= -1.0f;

        float length = sqrtf(newVorticity.x * newVorticity.x + newVorticity.y * newVorticity.y) + 1e-5f;
        Vector2f vorticityNorm = newVorticity * (1.0f / length);
        Vector2f vorticityF = vorticityNorm * vorticityField[y * width + x] * vorticity;

        newParticle = oldParticle;
        newParticle.velocity = newParticle.velocity + vorticityF * dt;
    }

    __global__ void applyForce(Particle* field,
                               std::size_t width, std::size_t height,
                               Color3f color, Vector2f force, Vector2f pos, int radius, float dt) {
        // applies force and add color dye to the particle field
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        if (x >= width || y >= height)
            return;

        float e = expf(-1.0f * ((x - pos.x) * (x - pos.x) + (y - pos.y) * (y - pos.y)) / static_cast<float>(radius));
        Vector2f forceVelocity = force * dt * e;
        Particle& particle = field[y * width + x];

        particle.velocity = particle.velocity + forceVelocity;
        color = color * e + particle.color;

        particle.color.r = color.r;
        particle.color.g = color.g;
        particle.color.b = color.b;
    }

    __global__ void project(Particle* newField, std::size_t width, std::size_t height, const float* pressureField) {
        // projects pressure field on velocity field
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        Vector2f& u = newField[y * width + x].velocity;
        u = u - gradient(pressureField, width, height, x, y);
    }

    __global__ void advect(Particle* newField, Particle* oldField,
                           std::size_t width, std::size_t height,
                           float densityDiffusion, float dt) {
        // adds quantity to particles using bilinear interpolation
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        float decay = 1.0f / (1.0f + densityDiffusion * dt);
        Particle& oldParticle = oldField[y * width + x];
        Vector2f position = { static_cast<float>(x), static_cast<float>(y) };

        // find new particle tracing where it came from
        Particle newParticle = interpolate(position - oldParticle.velocity * dt, oldField, width, height);
        newParticle.velocity = newParticle.velocity * decay;

        newParticle.color.r = min(1.0f, pow(newParticle.color.r, 1.005f) * decay);
        newParticle.color.g = min(1.0f, pow(newParticle.color.g, 1.005f) * decay);
        newParticle.color.b = min(1.0f, pow(newParticle.color.b, 1.005f) * decay);

        newField[y * width + x] = newParticle;
    }

    __global__ void diffuse(Particle* newField, Particle* oldField,
                            std::size_t width, std::size_t height,
                            float velocityDiffusion, float dt) {
        // calculates nonzero divergence velocity field u
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        Vector2f velocity = oldField[y * width + x].velocity;
        Vector2f position = { static_cast<float>(x), static_cast<float>(y) };

        // perform one iteration of jacobi method (diffuse method should be called 20-50 times per cell)
        float alpha = velocityDiffusion * velocityDiffusion / dt;
        float beta = 4.0f + alpha;
        newField[y * width + x].velocity = jacobiVelocity(oldField, width, height, position, velocity, alpha, beta);
    }

    __global__ void computeColor(Particle* newField, Particle* oldField,
                                 std::size_t width, std::size_t height,
                                 float colorDiffusion, float dt) {
        // calculates color field diffusion
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        Color3f color = oldField[y * width + x].color;
        Vector2f position = { static_cast<float>(x) , static_cast<float>(y) };
        float alpha = colorDiffusion * colorDiffusion / dt;
        float beta = 4.0f + alpha;

        // perform one iteration of jacobi method (diffuse method should be called 20-50 times per cell)
        newField[y * width + x].color = jacobiColor(oldField, width, height, position, color, alpha, beta);
    }

    __global__ void computePressureImpl(Particle* field,
                                        std::size_t width, std::size_t height,
                                        float* newPressure, float* oldPressure,
                                        float pressure) {
        // performs iteration of jacobi method on pressure field
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        float div = divergence(field, width, height, x, y);
        float alpha = -1.0f * pressure * pressure;
        float beta = 4.0;

        newPressure[y * width + x] = jacobiPressure(oldPressure, width, height, x, y, div, alpha, beta);
    }

    __global__ void paint(uint8_t* colorField, const Particle* field, std::size_t width, std::size_t height) {
        // fills output image with corresponding color
        auto x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        auto y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        if (x < width && y < height) {
            float r = field[y * width + x].color.r;
            float g = field[y * width + x].color.g;
            float b = field[y * width + x].color.b;

            colorField[4 * (y * width + x) + 0] = min(255.0f, 255.0f * r);
            colorField[4 * (y * width + x) + 1] = min(255.0f, 255.0f * g);
            colorField[4 * (y * width + x) + 2] = min(255.0f, 255.0f * b);
            colorField[4 * (y * width + x) + 3] = 255;
        }
    }

    __global__ void applyBloom(uint8_t* colorField,
                               std::size_t width, std::size_t height,
                               int positionX, int positionY,
                               float radius, float bloomIntense) {
        // adds flashlight effect near the mouse position
        int x = static_cast<int>(blockIdx.x * blockDim.x + threadIdx.x);
        int y = static_cast<int>(blockIdx.y * blockDim.y + threadIdx.y);

        if (x >= width || y >= height)
            return;

        int pos = static_cast<int>(y * width + x) * 4;
        float factor = bloomIntense * expf(-(
                static_cast<float>((x - positionX) * (x - positionX) +
                                   (y - positionY) * (y - positionY)) + 1.0f
        ) / (radius * radius));

        float r = colorField[pos + 0];
        float g = colorField[pos + 1];
        float b = colorField[pos + 2];

        float maxValue = max(r, max(g, b));

        colorField[pos + 0] = min(255.0f, r + maxValue * factor);
        colorField[pos + 1] = min(255.0f, g + maxValue * factor);
        colorField[pos + 2] = min(255.0f, b + maxValue * factor);
    }

} } // namespace ng::kernel