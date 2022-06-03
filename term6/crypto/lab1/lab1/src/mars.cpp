#include "mars.hpp"

#include <iostream>
#include <cmath>
#include <vector>

namespace crypto_lab1
{

    namespace _detail
    {

        static constexpr std::array<std::int32_t, 512> g_BoxS
        {
            0x09d0c479, 0x28c8ffe0, 0x84aa6c39, 0x9dad7287, 0x7dff9be3, 0xd4268361, 0xc96da1d4, 0x7974cc93, 0x85d0582e, 0x2a4b5705,
            0x1ca16a62, 0xc3bd279d, 0x0f1f25e5, 0x5160372f, 0xc695c1fb, 0x4d7ff1e4, 0xae5f6bf4, 0x0d72ee46, 0xff23de8a, 0xb1cf8e83,
            0xf14902e2, 0x3e981e42, 0x8bf53eb6, 0x7f4bf8ac, 0x83631f83, 0x25970205, 0x76afe784, 0x3a7931d4, 0x4f846450, 0x5c64c3f6,
            0x210a5f18, 0xc6986a26, 0x28f4e826, 0x3a60a81c, 0xd340a664, 0x7ea820c4, 0x526687c5, 0x7eddd12b, 0x32a11d1d, 0x9c9ef086,
            0x80f6e831, 0xab6f04ad, 0x56fb9b53, 0x8b2e095c, 0xb68556ae, 0xd2250b0d, 0x294a7721, 0xe21fb253, 0xae136749, 0xe82aae86,
            0x93365104, 0x99404a66, 0x78a784dc, 0xb69ba84b, 0x04046793, 0x23db5c1e, 0x46cae1d6, 0x2fe28134, 0x5a223942, 0x1863cd5b,
            0xc190c6e3, 0x07dfb846, 0x6eb88816, 0x2d0dcc4a, 0xa4ccae59, 0x3798670d, 0xcbfa9493, 0x4f481d45, 0xeafc8ca8, 0xdb1129d6,
            0xb0449e20, 0x0f5407fb, 0x6167d9a8, 0xd1f45763, 0x4daa96c3, 0x3bec5958, 0xababa014, 0xb6ccd201, 0x38d6279f, 0x02682215,
            0x8f376cd5, 0x092c237e, 0xbfc56593, 0x32889d2c, 0x854b3e95, 0x05bb9b43, 0x7dcd5dcd, 0xa02e926c, 0xfae527e5, 0x36a1c330,
            0x3412e1ae, 0xf257f462, 0x3c4f1d71, 0x30a2e809, 0x68e5f551, 0x9c61ba44, 0x5ded0ab8, 0x75ce09c8, 0x9654f93e, 0x698c0cca,
            0x243cb3e4, 0x2b062b97, 0x0f3b8d9e, 0x00e050df, 0xfc5d6166, 0xe35f9288, 0xc079550d, 0x0591aee8, 0x8e531e74, 0x75fe3578,
            0x2f6d829a, 0xf60b21ae, 0x95e8eb8d, 0x6699486b, 0x901d7d9b, 0xfd6d6e31, 0x1090acef, 0xe0670dd8, 0xdab2e692, 0xcd6d4365,
            0xe5393514, 0x3af345f0, 0x6241fc4d, 0x460da3a3, 0x7bcf3729, 0x8bf1d1e0, 0x14aac070, 0x1587ed55, 0x3afd7d3e, 0xd2f29e01,
            0x29a9d1f6, 0xefb10c53, 0xcf3b870f, 0xb414935c, 0x664465ed, 0x024acac7, 0x59a744c1, 0x1d2936a7, 0xdc580aa6, 0xcf574ca8,
            0x040a7a10, 0x6cd81807, 0x8a98be4c, 0xaccea063, 0xc33e92b5, 0xd1e0e03d, 0xb322517e, 0x2092bd13, 0x386b2c4a, 0x52e8dd58,
            0x58656dfb, 0x50820371, 0x41811896, 0xe337ef7e, 0xd39fb119, 0xc97f0df6, 0x68fea01b, 0xa150a6e5, 0x55258962, 0xeb6ff41b,
            0xd7c9cd7a, 0xa619cd9e, 0xbcf09576, 0x2672c073, 0xf003fb3c, 0x4ab7a50b, 0x1484126a, 0x487ba9b1, 0xa64fc9c6, 0xf6957d49,
            0x38b06a75, 0xdd805fcd, 0x63d094cf, 0xf51c999e, 0x1aa4d343, 0xb8495294, 0xce9f8e99, 0xbffcd770, 0xc7c275cc, 0x378453a7,
            0x7b21be33, 0x397f41bd, 0x4e94d131, 0x92cc1f98, 0x5915ea51, 0x99f861b7, 0xc9980a88, 0x1d74fd5f, 0xb0a495f8, 0x614deed0,
            0xb5778eea, 0x5941792d, 0xfa90c1f8, 0x33f824b4, 0xc4965372, 0x3ff6d550, 0x4ca5fec0, 0x8630e964, 0x5b3fbbd6, 0x7da26a48,
            0xb203231a, 0x04297514, 0x2d639306, 0x2eb13149, 0x16a45272, 0x532459a0, 0x8e5f4872, 0xf966c7d9, 0x07128dc0, 0x0d44db62,
            0xafc8d52d, 0x06316131, 0xd838e7ce, 0x1bc41d00, 0x3a2e8c0f, 0xea83837e, 0xb984737d, 0x13ba4891, 0xc4f8b949, 0xa6d6acb3,
            0xa215cdce, 0x8359838b, 0x6bd1aa31, 0xf579dd52, 0x21b93f93, 0xf5176781, 0x187dfdde, 0xe94aeb76, 0x2b38fd54, 0x431de1da,
            0xab394825, 0x9ad3048f, 0xdfea32aa, 0x659473e3, 0x623f7863, 0xf3346c59, 0xab3ab685, 0x3346a90b, 0x6b56443e, 0xc6de01f8,
            0x8d421fc0, 0x9b0ed10c, 0x88f1a1e9, 0x54c1f029, 0x7dead57b, 0x8d7ba426, 0x4cf5178a, 0x551a7cca, 0x1a9a5f08, 0xfcd651b9,
            0x25605182, 0xe11fc6c3, 0xb6fd9676, 0x337b3027, 0xb7c8eb14, 0x9e5fd030, 0x6b57e354, 0xad913cf7, 0x7e16688d, 0x58872a69,
            0x2c2fc7df, 0xe389ccc6, 0x30738df1, 0x0824a734, 0xe1797a8b, 0xa4a8d57b, 0x5b5d193b, 0xc8a8309b, 0x73f9a978, 0x73398d32,
            0x0f59573e, 0xe9df2b03, 0xe8a5b6c8, 0x848d0704, 0x98df93c2, 0x720a1dc3, 0x684f259a, 0x943ba848, 0xa6370152, 0x863b5ea3,
            0xd17b978b, 0x6d9b58ef, 0x0a700dd4, 0xa73d36bf, 0x8e6a0829, 0x8695bc14, 0xe35b3447, 0x933ac568, 0x8894b022, 0x2f511c27,
            0xddfbcc3c, 0x006662b6, 0x117c83fe, 0x4e12b414, 0xc2bca766, 0x3a2fec10, 0xf4562420, 0x55792e2a, 0x46f5d857, 0xceda25ce,
            0xc3601d3b, 0x6c00ab46, 0xefac9c28, 0xb3c35047, 0x611dfee3, 0x257c3207, 0xfdd58482, 0x3b14d84f, 0x23becb64, 0xa075f3a3,
            0x088f8ead, 0x07adf158, 0x7796943c, 0xfacabf3d, 0xc09730cd, 0xf7679969, 0xda44e9ed, 0x2c854c12, 0x35935fa3, 0x2f057d9f,
            0x690624f8, 0x1cb0bafd, 0x7b0dbdc6, 0x810f23bb, 0xfa929a1a, 0x6d969a17, 0x6742979b, 0x74ac7d05, 0x010e65c4, 0x86a3d963,
            0xf907b5a0, 0xd0042bd3, 0x158d7d03, 0x287a8255, 0xbba8366f, 0x096edc33, 0x21916a7b, 0x77b56b86, 0x951622f9, 0xa6c5e650,
            0x8cea17d1, 0xcd8c62bc, 0xa3d63433, 0x358a68fd, 0x0f9b9d3c, 0xd6aa295b, 0xfe33384a, 0xc000738e, 0xcd67eb2f, 0xe2eb6dc2,
            0x97338b02, 0x06c9f246, 0x419cf1ad, 0x2b83c045, 0x3723f18a, 0xcb5b3089, 0x160bead7, 0x5d494656, 0x35f8a74b, 0x1e4e6c9e,
            0x000399bd, 0x67466880, 0xb4174831, 0xacf423b2, 0xca815ab3, 0x5a6395e7, 0x302a67c5, 0x8bdb446b, 0x108f8fa4, 0x10223eda,
            0x92b8b48b, 0x7f38d0ee, 0xab2701d4, 0x0262d415, 0xaf224a30, 0xb3d88aba, 0xf8b2c3af, 0xdaf7ef70, 0xcc97d3b7, 0xe9614b6c,
            0x2baebff4, 0x70f687cf, 0x386c9156, 0xce092ee5, 0x01e87da6, 0x6ce91e6a, 0xbb7bcc84, 0xc7922c20, 0x9d3b71fd, 0x060e41c6,
            0xd7590f15, 0x4e03bb47, 0x183c198e, 0x63eeb240, 0x2ddbf49a, 0x6d5cba54, 0x923750af, 0xf9e14236, 0x7838162b, 0x59726c72,
            0x81b66760, 0xbb2926c1, 0x48a0ce0d, 0xa6c0496d, 0xad43507b, 0x718d496a, 0x9df057af, 0x44b1bde6, 0x054356dc, 0xde7ced35,
            0xd51a138b, 0x62088cc9, 0x35830311, 0xc96efca2, 0x686f86ec, 0x8e77cb68, 0x63e1d6b8, 0xc80f9778, 0x79c491fd, 0x1b4c67f2,
            0x58656dfb, 0x50820371, 0x41811896, 0xe337ef7e, 0xd39fb119, 0xc97f0df6, 0x68fea01b, 0xa150a6e5, 0x55258962, 0xeb6ff41b,
            0xd7c9cd7a, 0xa619cd9e, 0xbcf09576, 0x2672c073, 0xf003fb3c, 0x4ab7a50b, 0x1484126a, 0x487ba9b1, 0xa64fc9c6, 0xf6957d49,
            0x38b06a75, 0xdd805fcd, 0x63d094cf, 0xf51c999e, 0x1aa4d343, 0xb8495294, 0xce9f8e99, 0xbffcd770, 0xc7c275cc, 0x378453a7,
            0x7b21be33, 0x397f41bd, 0x4e94d131, 0x92cc1f98, 0x5915ea51, 0x99f861b7, 0xc9980a88, 0x1d74fd5f, 0xb0a495f8, 0x614deed0,
            0xb5778eea, 0x5941792d, 0xfa90c1f8, 0x33f824b4, 0xc4965372, 0x3ff6d550, 0x4ca5fec0, 0x8630e964, 0x5b3fbbd6, 0x7da26a48,
            0xb203231a, 0x04297514, 0x2d639306, 0x2eb13149, 0x16a45272, 0x532459a0, 0x8e5f4872, 0xf966c7d9, 0x07128dc0, 0x0d44db62,
            0xafc8d52d, 0x06316131, 0xd838e7ce, 0x1bc41d00, 0x3a2e8c0f, 0xea83837e, 0xb984737d, 0x13ba4891, 0xc4f8b949, 0xa6d6acb3,
            0xa215cdce, 0x8359838b, 0x6bd1aa31, 0xf579dd52, 0x21b93f93, 0xf5176781, 0x187dfdde, 0xe94aeb76, 0x2b38fd54, 0x431de1da,
            0xab394825, 0x9ad3048f, 0xdfea32aa, 0x659473e3, 0x623f7863, 0xf3346c59, 0xab3ab685, 0x3346a90b, 0x6b56443e, 0xc6de01f8,
            0x8d421fc0, 0x9b0ed10c
        };

        static constexpr std::int32_t LogicalRightShift(std::int32_t x, std::int32_t n)
        {
            return static_cast<std::uint32_t>(x) >> n;
        }

        static constexpr std::int32_t ArithmeticRightShift(std::int32_t x, std::int32_t n)
        {
            return (x < 0 && n > 0) ? (x >> n | ~(~0U >> n)) : (x >> n);
        }

        static constexpr std::int32_t RotateLeft(std::int32_t arg, std::int32_t amount)
        {
            return (arg << amount) | LogicalRightShift(arg, (32 - amount));
        }

        static constexpr std::int32_t RotateRight(std::int32_t arg, std::int32_t amount)
        {
            return LogicalRightShift(arg, amount) | (arg << (32 - amount));
        }

        static constexpr std::int32_t GenerateMask(std::int32_t x)
        {
            std::int32_t m;

            m = (~x ^ _detail::LogicalRightShift(x, 1)) & 0x7fffffff;
            m &= (m >> 1) & (m >> 2);
            m &= (m >> 3) & (m >> 6);

            if (m == 0)
            {
                return 0;
            }

            m <<= 1;
            m |= (m << 1);
            m |= (m << 2);
            m |= (m << 4);

            m |= (m << 1) & ~x & 0x80000000;

            return m & 0xfffffffc;
        }

        static constexpr std::array<std::int32_t, 3> E(std::int32_t in, std::int32_t k1, std::int32_t k2)
        {
            std::int32_t m;
            std::int32_t l;
            std::int32_t r;

            m = in + k1;
            r = RotateLeft(in, 13) * k2;
            l = g_BoxS[m & 0x000001ff];
            r = RotateLeft(r, 5);
            m = RotateLeft(m, r & 0x0000001f);
            l = l ^ r;
            r = RotateLeft(r, 5);
            l = l ^ r;
            l = RotateLeft(l, r & 0x0000001f);

            return { l, m, r };
        }

        static void CopyFromBlock(const Block16& block, std::vector<int8_t>& result, std::size_t resultBegin)
        {
            std::copy(block.GetData8().begin(), block.GetData8().end(), result.begin() + resultBegin);
        }

        static std::vector<std::int32_t> From8To32(const std::span<const std::int8_t>& data8)
        {
            std::vector<std::int32_t> data32(data8.size() / 4, 0);

            for (std::size_t i = 0; i < data32.size(); ++i)
            {
                data32[i] = static_cast<std::int32_t>(((data8[i * 4 + 0] & 0xff) << 0)) |
                            static_cast<std::int32_t>(((data8[i * 4 + 1] & 0xff) << 8)) |
                            static_cast<std::int32_t>(((data8[i * 4 + 2] & 0xff) << 16)) |
                            static_cast<std::int32_t>(((data8[i * 4 + 3] & 0xff) << 24));
            }

            return data32;
        }

    } // namespace _detail

    Block16::Block16()
    {
        m_Data8.fill(0);
    }

    void Block16::Encrypt(const KeyTable& key)
    {
        const auto cyclicShiftLeft = [](std::int32_t& a, std::int32_t& b, std::int32_t& c, std::int32_t& d)
        {
            const std::int32_t auxiliary{ a };

            a = b;
            b = c;
            c = d;
            d = auxiliary;
        };

        std::vector<std::int32_t> data32{ _detail::From8To32(m_Data8) };

        std::int32_t a{ data32[0] + key[0] };
        std::int32_t b{ data32[1] + key[1] };
        std::int32_t c{ data32[2] + key[2] };
        std::int32_t d{ data32[3] + key[3] };

        // Forward mixing
        {
            for (std::size_t i = 0; i < 8; ++i)
            {
                b = b ^ _detail::g_BoxS[a & 0xff];
                b = b + _detail::g_BoxS[(_detail::RotateRight(a, 8) & 0xff) + 256];
                c = c + _detail::g_BoxS[_detail::RotateRight(a, 16) & 0xff];
                d = d ^ _detail::g_BoxS[(_detail::RotateRight(a, 24) & 0xff) + 256];

                a = _detail::RotateRight(a, 24);

                if (i == 1 || i == 5)
                {
                    a = a + b;
                }

                if (i == 0 || i == 4)
                {
                    a = a + d;
                }

                cyclicShiftLeft(a, b, c, d);
            }
        }

        // Cryptographic core
        {
            std::array<std::int32_t, 3> e;

            for (std::size_t i = 0; i < 16; ++i)
            {
                e = _detail::E(a, key[2 * i + 4], key[2 * i + 5]);

                a = _detail::RotateLeft(a, 13);
                c = c + e[1];

                if (i < 8)
                {
                    b = b + e[0];
                    d = d ^ e[2];
                }
                else
                {
                    d = d + e[0];
                    b = b ^ e[2];
                }

                cyclicShiftLeft(a, b, c, d);
            }
        }

        // Backward mixing
        {
            for (std::size_t i = 0; i < 8; ++i)
            {
                if (i == 3 || i == 7)
                {
                    a = a - b;
                }

                if (i == 2 || i == 6)
                {
                    a = a - d;
                }

                b = b ^ _detail::g_BoxS[(a & 0xff) + 256];
                c = c - _detail::g_BoxS[_detail::RotateRight(a, 24) & 0xff];
                d = d - _detail::g_BoxS[(_detail::RotateRight(a, 16) & 0xff) + 256];
                d = d ^ _detail::g_BoxS[_detail::RotateRight(a, 8) & 0xff];

                a = _detail::RotateLeft(a, 24);

                cyclicShiftLeft(a, b, c, d);
            }
        }

        data32[0] = a - key[36];
        data32[1] = b - key[37];
        data32[2] = c - key[38];
        data32[3] = d - key[39];

        for (std::size_t i = 0; i < m_Data8.size(); ++i)
        {
            m_Data8[i] = static_cast<std::int8_t>(((data32[i / 4] >> (i % 4) * 8) & 0xff));
        }
    }

    void Block16::Decrypt(const KeyTable& key)
    {
        const auto cyclicShiftLeft = [](std::int32_t& a, std::int32_t& b, std::int32_t& c, std::int32_t& d)
        {
            const std::int32_t auxiliary{ a };

            a = b;
            b = c;
            c = d;
            d = auxiliary;
        };

        std::vector<std::int32_t> data32{ _detail::From8To32(m_Data8) };

        std::int32_t a{ data32[0] + key[36] };
        std::int32_t b{ data32[1] + key[37] };
        std::int32_t c{ data32[2] + key[38] };
        std::int32_t d{ data32[3] + key[39] };

        // Forward mixing
        {
            for (std::size_t i = 8; i--;)
            {
                cyclicShiftLeft(d, c, b, a);

                a = _detail::RotateRight(a, 24);

                d = d ^ _detail::g_BoxS[(_detail::RotateRight(a, 8) & 0xff)];
                d = d + _detail::g_BoxS[(_detail::RotateRight(a, 16) & 0xff) + 256];
                c = c + _detail::g_BoxS[_detail::RotateRight(a, 24) & 0xff];
                b = b ^ _detail::g_BoxS[(a & 0xff) + 256];

                if (i == 2 || i == 6)
                {
                    a = a + d;
                }

                if (i == 3 || i == 7)
                {
                    a = a + b;
                }
            }
        }

        // Cryptographic core
        {
            std::array<std::int32_t, 3> e;

            for (std::size_t i = 16; i--;)
            {
                cyclicShiftLeft(d, c, b, a);

                a = _detail::RotateRight(a, 13);
                e = _detail::E(a, key[2 * i + 4], key    [2 * i + 5]);

                c = c - e[1];

                if (i < 8)
                {
                    b = b - e[0];
                    d = d ^ e[2];
                }
                else
                {
                    d = d - e[0];
                    b = b ^ e[2];
                }
            }
        }

        // Backward mixing
        {
            for (std::size_t i = 8; i--;)
            {
                cyclicShiftLeft(d, c, b, a);

                if (i == 0 || i == 4)
                {
                    a = a - d;
                }

                if (i == 1 || i == 5)
                {
                    a = a - b;
                }

                a = _detail::RotateLeft(a, 24);

                d = d ^ _detail::g_BoxS[(_detail::RotateRight(a, 24) & 0xff) + 256];
                c = c - _detail::g_BoxS[_detail::RotateRight(a, 16) & 0xff];
                b = b - _detail::g_BoxS[(_detail::RotateRight(a, 8) & 0xff) + 256];
                b = b ^ _detail::g_BoxS[a & 0xff];
            }
        }

        data32[0] = a - key[0];
        data32[1] = b - key[1];
        data32[2] = c - key[2];
        data32[3] = d - key[3];

        for (std::size_t i = 0; i < m_Data8.size(); ++i)
        {
            m_Data8[i] = static_cast<std::int8_t>(((data32[i / 4] >> (i % 4) * 8) & 0xff));
        }
    }

    std::vector<std::int8_t> MARS::Encrypt(const std::span<const std::int8_t>& in, const std::span<const std::int8_t>& key)
    {
        const KeyTable keyTable{ ExpandKey(key) };
        const std::size_t paddingSize{ 16 - in.size() % 16 };

        std::vector<std::int8_t> result(in.size() + paddingSize);
        std::vector<std::int8_t> padding(paddingSize, 0);
        padding[0] = static_cast<std::int8_t>(0x80);

        Block16 block;

        std::size_t paddingOffset{ 0 };

        for (std::size_t i = 0; i < result.size(); ++i)
        {
            if (i > 0 && i % 16 == 0)
            {
                block.Encrypt(keyTable);
                _detail::CopyFromBlock(block, result, i - 16);
            }

            if (i < in.size())
            {
                block.GetData8()[i % 16] = in[i];
            }
            else
            {
                block.GetData8()[i % 16] = padding[paddingOffset++ % 16];
            }
        }

        block.Encrypt(keyTable);
        _detail::CopyFromBlock(block, result, result.size() - 16);

        return result;
    }

    std::vector<std::int8_t> MARS::Decrypt(const std::span<const std::int8_t>& in, const std::span<const std::int8_t>& key)
    {
        const KeyTable keyTable{ ExpandKey(key) };

        std::vector<std::int8_t> result(in.size(), 0);
        Block16 block;

        for (std::size_t i = 0; i < result.size(); ++i)
        {
            if (i > 0 && i % 16 == 0)
            {
                block.Decrypt(keyTable);
                _detail::CopyFromBlock(block, result, i - 16);
            }

            if (i < in.size())
            {
                block.GetData8()[i % 16] = in[i];
            }
        }

        block.Decrypt(keyTable);
        _detail::CopyFromBlock(block, result, result.size() - 16);

        // Delete padding
        {
            while (result.back() == 0)
            {
                result.pop_back();
            }

            result.pop_back();
        }

        return result;
    }

    KeyTable MARS::ExpandKey(const std::span<const std::int8_t>& key)
    {
        KeyTable keyTable;
        keyTable.fill(0);

        std::vector<std::int32_t> data{ _detail::From8To32(key) };
        std::array<std::int32_t, 15> t;

        // Fill t array
        for (std::size_t i = 0; i < t.size(); ++i)
        {
            if (i < data.size())
            {
                t[i] = data[i];
            }
            else if (i == data.size())
            {
                t[i] = static_cast<std::int32_t>(data.size());
            }
            else
            {
                t[i] = 0;
            }
        }

        for (std::int32_t j = 0; j < 4; ++j)
        {
            for (std::int32_t i = 0; i < static_cast<std::int32_t>(t.size()); ++i)
            {
                t[i] = t[i] ^ (_detail::RotateLeft(t[std::abs(i - 7 % 15)] ^ t[std::abs(i - 2 % 15)], 3) ^ (4 * i + j));
            }

            for (std::int32_t c = 0; c < 4; ++c)
            {
                for (std::int32_t i = 0; i < static_cast<std::int32_t>(t.size()); ++i)
                {
                    t[i] = t[i] + _detail::RotateLeft(_detail::g_BoxS[t[std::abs(i - 1 % 15)] & 0x000001ff], 9);
                }
            }

            for (std::size_t i = 0; i < 10; ++i)
            {
                keyTable[10 * j + i] = t[4 * i % 15];
            }
        }

        {
            static constexpr std::array<std::int32_t, 4> b{ 0xa4a8d57b, 0x5b5d193b, 0xc8a8309b, 0x73f9a978 };

            std::int32_t j{ 0 };
            std::int32_t w{ 0 };
            std::int32_t m{ 0 };
            std::int32_t r{ 0 };
            std::int32_t p{ 0 };

            for (std::size_t i = 5; i < 36; ++i)
            {
                j = keyTable[i] & 0x00000003;
                w = keyTable[i] | 0x00000003;

                m = _detail::GenerateMask(w);
                r = keyTable[i - 1] & 0x0000001f;
                p = _detail::RotateLeft(b[j], r);

                keyTable[i] = w ^ (p & m);
            }
        }

        return keyTable;
    }

} // namespace crypto_lab1