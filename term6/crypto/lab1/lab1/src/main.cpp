#include <iostream>

#include "mars.hpp"

namespace
{

    void PrintByteStream(const std::string_view& name, const std::span<const std::int8_t>& data)
    {
        std::cout << name << ": ";

        for (const auto& e : data)
        {
            std::cout << +e << " ";
        }

        std::cout << std::endl;
    }

}

int main()
{
    // http://reto.orgfree.com/us/projectlinks/MARSReport.html#Top
    // 128 - 1248 bits

    const std::array<std::int8_t, 4> in{ 12, 85, 32, 42 };
    const std::array<std::int8_t, 4> key{ 12, 85, 32, 42 };

    const std::vector<std::int8_t> out{ crypto_lab1::MARS::Encrypt(in, key) };
    const std::vector<std::int8_t> decryptedIn{ crypto_lab1::MARS::Decrypt(out, key) };

    PrintByteStream("In", in);
    PrintByteStream("Key", key);

    PrintByteStream("Out", out);
    PrintByteStream("DecrtyptedIn", decryptedIn);
}