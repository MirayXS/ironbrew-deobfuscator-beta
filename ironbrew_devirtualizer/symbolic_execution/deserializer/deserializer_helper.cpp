#include "deserializer_helper.hpp"
#include "deserializer_context.hpp"

namespace deobf::ironbrew_devirtualizer::symbolic_execution::deserializer {
    const inline bool deserializer_helper::get_bits(std::size_t number, std::size_t i) const {
        const double bit_mask = std::pow(2, i - 1);
        return std::fmodl(number, bit_mask + bit_mask) >= bit_mask;
    }

    const inline std::size_t deserializer_helper::get_bits(std::size_t number, std::size_t i, std::size_t j) const {
        // local l = (number / (2 ^ (i - 1))) % (2 ^ ((j - 1) - (i - 1) + 1))
        const auto lhs = (number / std::pow(2, i - 1));
        const auto rhs = std::pow(2, (j - 1) - (i - 1) + 1);

        return std::fmodf(lhs, rhs);
    }
    
    const std::uint8_t deserializer_helper::get_8_bits() { // could been done with reinterpret_cast on address
        return static_cast<std::uint8_t>(managed_deserializer_string.get() ^ vm_xor_key);
    }

    const std::uint16_t deserializer_helper::get_16_bits() { // could been done with reinterpret_cast on address
        std::unique_ptr<unsigned char[]> data_block{ new unsigned char[2] }; // lazy to make_unique
        managed_deserializer_string.read(reinterpret_cast<char*>(data_block.get()), 2);

        // gonna be loop unrolled by the compiler anyway idc
        for (auto i = 0u; i < 2; ++i)
            data_block[i] ^= vm_xor_key;

        return *reinterpret_cast<std::int16_t*>(data_block.get()); //(data_block[1] * 256) + data_block[0];
    }

    const std::uint32_t deserializer_helper::get_32_bits() { // dword since 4
        std::unique_ptr<unsigned char[]> data_block{ new unsigned char[4] }; // lazy to make_unique
        managed_deserializer_string.read(reinterpret_cast<char*>(data_block.get()), 4);

        for (auto i = 0u; i < 4; ++i) 
            data_block[i] ^= vm_xor_key;


        return *reinterpret_cast<std::int32_t*>(data_block.get()); //(data_block[3] * 16777216) + (data_block[2] * 65536) + (data_block[1] * 256) + data_block[0];
    }

    const double deserializer_helper::get_float() { // update about this: reinterpret_cast'ing into double* and reading wouldn't work due to different IEEE standards, so we have to rely on the impelemented way
        const auto left = get_32_bits();
        const auto right = get_32_bits();

        bool is_normal_flag = true;
        
        const std::uint64_t mantissa = get_bits(right, 1, 20) * 4294967296 + left;
        auto exponent = get_bits(right, 21, 31);
        const auto sign = std::powl(-1, get_bits(right, 32));

        if (exponent == 0) {
            if (mantissa == 0) {
                return 0;
            }
            else {
                exponent = 1;
                is_normal_flag = false;
            }
        }
        else if (exponent == 2047) {
            if (mantissa == 0) {
                return std::numeric_limits<double>::infinity(); // inf
            }
            else {
                return std::numeric_limits<double>::signaling_NaN(); // return signaling nan (accept exceptions from fpu on ops)
            }
        }

        return std::ldexp(sign, exponent - 1023) * (is_normal_flag + (mantissa / 4.50359963e15));
    }

    const std::string deserializer_helper::get_string(std::size_t length) {
        if (length > managed_deserializer_string.rdbuf()->in_avail())
            return { };

        std::unique_ptr<char[]> data_block{ new char[length] };
        managed_deserializer_string.read(data_block.get(), length);

        std::string result{ data_block.get() };
        for (auto& character : result)
            character ^= vm_xor_key;

        return result;
    }

    const std::string deserializer_helper::get_string() {
        const auto length = get_32_bits();
        if (!length)
            return { };
        else if (length > managed_deserializer_string.rdbuf()->in_avail()) { // safe from buffer overflow attacks and stack smashing at emulator
            throw std::runtime_error("not enough memory for the desired string length? [get_string]");
        }

        std::unique_ptr<char[]> data_block{ new char[length] };
        managed_deserializer_string.read(data_block.get(), length);
        
        auto result = std::string{ data_block.get(), length }; // could use a string_view
        for (auto& character : result)
            character ^= vm_xor_key;

        return result;
    }
}
