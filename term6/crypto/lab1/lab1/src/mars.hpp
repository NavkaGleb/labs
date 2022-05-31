#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <span>

namespace crypto_lab1
{

    using KeyTable = std::array<std::int32_t, 40>;

    class Block16
    {
    public:
        using Data8 = std::array<std::int8_t, 16>;

    public:
        Block16();

    public:
        Data8& GetData8() { return m_Data8; }
        const Data8& GetData8() const { return m_Data8; }

    public:
        void Encrypt(const KeyTable& key);
        void Decrypt(const KeyTable& key);

    private:
        Data8 m_Data8;
    };

    class MARS
    {
    public:
        static std::vector<std::int8_t> Encrypt(const std::span<const std::int8_t>& in, const std::span<const std::int8_t>& key);
        static std::vector<std::int8_t> Decrypt(const std::span<const std::int8_t>& in, const std::span<const std::int8_t>& key);

    private:
        static KeyTable ExpandKey(const std::span<const std::int8_t>& key);

    public:
        MARS() = delete;
        MARS(const MARS&& other) = delete;
        MARS(MARS&& other) = delete;

    public:
        MARS& operator=(const MARS& other) = delete;
        MARS& operator=(MARS&& other) = delete;
    };

} // namespace crypto_lab1