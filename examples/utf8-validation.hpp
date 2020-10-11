#include <ctve.hpp>
#include <ctre.hpp>

namespace ctve::utf8
{
    static inline constexpr auto follow_byte = in(range{'\x80', '\xBF'});
    static inline constexpr auto follow_byte_A0_BF = in(range{'\xA0', '\xBF'});
    static inline constexpr auto follow_byte_80_9F = in(range{'\x80', '\x9F'});
    static inline constexpr auto follow_byte_90_BF = in(range{'\x90', '\xBF'});
    static inline constexpr auto follow_byte_80_8F = in(range{'\x80', '\x8F'});

    static inline constexpr auto u8_char_1byte = in(range{'\x00', '\x7F'});
    static inline constexpr auto u8_char_2byte = in(range{'\xC2', '\xDF'}) + follow_byte;

    static inline constexpr auto u8_char_3byte_range1 = '\xE0' + follow_byte_A0_BF + follow_byte;
    static inline constexpr auto u8_char_3byte_range2 = in(range{'\xE1', '\xEC'}) + follow_byte.count(2);
    static inline constexpr auto u8_char_3byte_range3 = '\xED' + follow_byte_80_9F + follow_byte;
    static inline constexpr auto u8_char_3byte_range4 = in(range{'\xEE', '\xEF'}) + follow_byte.count(2);
    static inline constexpr auto u8_char_3byte = u8_char_3byte_range1 || u8_char_3byte_range2 || u8_char_3byte_range3 || u8_char_3byte_range4;

    static inline constexpr auto u8_char_4byte_range1 = '\xF0' + follow_byte_90_BF + follow_byte.count(2);
    static inline constexpr auto u8_char_4byte_range2 = in(range{'\xF1', '\xF3'}) + follow_byte.count(3);
    static inline constexpr auto u8_char_4byte_range3 = '\xF4' + follow_byte_80_8F + follow_byte.count(2);
    static inline constexpr auto u8_char_4byte = u8_char_4byte_range1 || u8_char_4byte_range2 || u8_char_4byte_range3;

    static inline constexpr auto u8_char = u8_char_1byte || u8_char_2byte || u8_char_3byte || u8_char_4byte;
    static inline constexpr auto u8_chars = u8_char.zero_or_more();

    constexpr bool is_valid(std::string_view data)
    {
        return ctre::match<u8_chars>(data);
    }
}
