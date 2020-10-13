#include <ctve.hpp>

#include <ctre.hpp>

template <auto& input>
constexpr auto create()
{
  using tmp = typename ctll::parser<ctre::pcre, input, ctre::pcre_actions>::
      template output<ctre::pcre_context<>>;
  static_assert(tmp(), "Regular Expression contains syntax error.");
  using re = decltype(front(typename tmp::output_type::stack_type()));
  return ctre::regular_expression(re());
}

#define CTRE_CREATE(pattern) create<pattern>()

using namespace std::string_view_literals;
using namespace ctve;

#define CHECK(hexVal, hexStr) \
    namespace { \
       static constexpr auto hexVal = find(hexStr); \
       static_assert(CTRE_CREATE(hexVal).match(std::string_view{hexStr, 1})); \
       static constexpr auto hexVal ## In = in(std::string_view{hexStr, 1}[0]); \
       static_assert(CTRE_CREATE(hexVal ## In).match(std::string_view{hexStr, 1})); \
    }

CHECK(x00, "\x00");
CHECK(x01, "\x01");
CHECK(x02, "\x02");
CHECK(x03, "\x03");
CHECK(x04, "\x04");
CHECK(x05, "\x05");
CHECK(x06, "\x06");
CHECK(x07, "\x07");
CHECK(x08, "\x08");
CHECK(x09, "\x09");
CHECK(x0A, "\x0A");
CHECK(x0B, "\x0B");
CHECK(x0C, "\x0C");
CHECK(x0D, "\x0D");
CHECK(x0E, "\x0E");
CHECK(x0F, "\x0F");

CHECK(x10, "\x10");
CHECK(x11, "\x11");
CHECK(x12, "\x12");
CHECK(x13, "\x13");
CHECK(x14, "\x14");
CHECK(x15, "\x15");
CHECK(x16, "\x16");
CHECK(x17, "\x17");
CHECK(x18, "\x18");
CHECK(x19, "\x19");
CHECK(x1A, "\x1A");
CHECK(x1B, "\x1B");
CHECK(x1C, "\x1C");
CHECK(x1D, "\x1D");
CHECK(x1E, "\x1E");
CHECK(x1F, "\x1F");

CHECK(x20, "\x20");
CHECK(x21, "\x21");
CHECK(x22, "\x22");
CHECK(x23, "\x23");
CHECK(x24, "\x24");
CHECK(x25, "\x25");
CHECK(x26, "\x26");
CHECK(x27, "\x27");
CHECK(x28, "\x28");
CHECK(x29, "\x29");
CHECK(x2A, "\x2A");
CHECK(x2B, "\x2B");
CHECK(x2C, "\x2C");
CHECK(x2D, "\x2D");
CHECK(x2E, "\x2E");
CHECK(x2F, "\x2F");

CHECK(x30, "\x30");
CHECK(x31, "\x31");
CHECK(x32, "\x32");
CHECK(x33, "\x33");
CHECK(x34, "\x34");
CHECK(x35, "\x35");
CHECK(x36, "\x36");
CHECK(x37, "\x37");
CHECK(x38, "\x38");
CHECK(x39, "\x39");
CHECK(x3A, "\x3A");
CHECK(x3B, "\x3B");
CHECK(x3C, "\x3C");
CHECK(x3D, "\x3D");
CHECK(x3E, "\x3E");
CHECK(x3F, "\x3F");

CHECK(x40, "\x40");
CHECK(x41, "\x41");
CHECK(x42, "\x42");
CHECK(x43, "\x43");
CHECK(x44, "\x44");
CHECK(x45, "\x45");
CHECK(x46, "\x46");
CHECK(x47, "\x47");
CHECK(x48, "\x48");
CHECK(x49, "\x49");
CHECK(x4A, "\x4A");
CHECK(x4B, "\x4B");
CHECK(x4C, "\x4C");
CHECK(x4D, "\x4D");
CHECK(x4E, "\x4E");
CHECK(x4F, "\x4F");

CHECK(x50, "\x50");
CHECK(x51, "\x51");
CHECK(x52, "\x52");
CHECK(x53, "\x53");
CHECK(x54, "\x54");
CHECK(x55, "\x55");
CHECK(x56, "\x56");
CHECK(x57, "\x57");
CHECK(x58, "\x58");
CHECK(x59, "\x59");
CHECK(x5A, "\x5A");
CHECK(x5B, "\x5B");
CHECK(x5C, "\x5C");
CHECK(x5D, "\x5D");
CHECK(x5E, "\x5E");
CHECK(x5F, "\x5F");

CHECK(x60, "\x60");
CHECK(x61, "\x61");
CHECK(x62, "\x62");
CHECK(x63, "\x63");
CHECK(x64, "\x64");
CHECK(x65, "\x65");
CHECK(x66, "\x66");
CHECK(x67, "\x67");
CHECK(x68, "\x68");
CHECK(x69, "\x69");
CHECK(x6A, "\x6A");
CHECK(x6B, "\x6B");
CHECK(x6C, "\x6C");
CHECK(x6D, "\x6D");
CHECK(x6E, "\x6E");
CHECK(x6F, "\x6F");

CHECK(x70, "\x70");
CHECK(x71, "\x71");
CHECK(x72, "\x72");
CHECK(x73, "\x73");
CHECK(x74, "\x74");
CHECK(x75, "\x75");
CHECK(x76, "\x76");
CHECK(x77, "\x77");
CHECK(x78, "\x78");
CHECK(x79, "\x79");
CHECK(x7A, "\x7A");
CHECK(x7B, "\x7B");
CHECK(x7C, "\x7C");
CHECK(x7D, "\x7D");
CHECK(x7E, "\x7E");
CHECK(x7F, "\x7F");

CHECK(x80, "\x80");
CHECK(x81, "\x81");
CHECK(x82, "\x82");
CHECK(x83, "\x83");
CHECK(x84, "\x84");
CHECK(x85, "\x85");
CHECK(x86, "\x86");
CHECK(x87, "\x87");
CHECK(x88, "\x88");
CHECK(x89, "\x89");
CHECK(x8A, "\x8A");
CHECK(x8B, "\x8B");
CHECK(x8C, "\x8C");
CHECK(x8D, "\x8D");
CHECK(x8E, "\x8E");
CHECK(x8F, "\x8F");

CHECK(x90, "\x90");
CHECK(x91, "\x91");
CHECK(x92, "\x92");
CHECK(x93, "\x93");
CHECK(x94, "\x94");
CHECK(x95, "\x95");
CHECK(x96, "\x96");
CHECK(x97, "\x97");
CHECK(x98, "\x98");
CHECK(x99, "\x99");
CHECK(x9A, "\x9A");
CHECK(x9B, "\x9B");
CHECK(x9C, "\x9C");
CHECK(x9D, "\x9D");
CHECK(x9E, "\x9E");
CHECK(x9F, "\x9F");

CHECK(xA0, "\xA0");
CHECK(xA1, "\xA1");
CHECK(xA2, "\xA2");
CHECK(xA3, "\xA3");
CHECK(xA4, "\xA4");
CHECK(xA5, "\xA5");
CHECK(xA6, "\xA6");
CHECK(xA7, "\xA7");
CHECK(xA8, "\xA8");
CHECK(xA9, "\xA9");
CHECK(xAA, "\xAA");
CHECK(xAB, "\xAB");
CHECK(xAC, "\xAC");
CHECK(xAD, "\xAD");
CHECK(xAE, "\xAE");
CHECK(xAF, "\xAF");

CHECK(xB0, "\xB0");
CHECK(xB1, "\xB1");
CHECK(xB2, "\xB2");
CHECK(xB3, "\xB3");
CHECK(xB4, "\xB4");
CHECK(xB5, "\xB5");
CHECK(xB6, "\xB6");
CHECK(xB7, "\xB7");
CHECK(xB8, "\xB8");
CHECK(xB9, "\xB9");
CHECK(xBA, "\xBA");
CHECK(xBB, "\xBB");
CHECK(xBC, "\xBC");
CHECK(xBD, "\xBD");
CHECK(xBE, "\xBE");
CHECK(xBF, "\xBF");

CHECK(xC0, "\xC0");
CHECK(xC1, "\xC1");
CHECK(xC2, "\xC2");
CHECK(xC3, "\xC3");
CHECK(xC4, "\xC4");
CHECK(xC5, "\xC5");
CHECK(xC6, "\xC6");
CHECK(xC7, "\xC7");
CHECK(xC8, "\xC8");
CHECK(xC9, "\xC9");
CHECK(xCA, "\xCA");
CHECK(xCB, "\xCB");
CHECK(xCC, "\xCC");
CHECK(xCD, "\xCD");
CHECK(xCE, "\xCE");
CHECK(xCF, "\xCF");

CHECK(xD0, "\xD0");
CHECK(xD1, "\xD1");
CHECK(xD2, "\xD2");
CHECK(xD3, "\xD3");
CHECK(xD4, "\xD4");
CHECK(xD5, "\xD5");
CHECK(xD6, "\xD6");
CHECK(xD7, "\xD7");
CHECK(xD8, "\xD8");
CHECK(xD9, "\xD9");
CHECK(xDA, "\xDA");
CHECK(xDB, "\xDB");
CHECK(xDC, "\xDC");
CHECK(xDD, "\xDD");
CHECK(xDE, "\xDE");
CHECK(xDF, "\xDF");

CHECK(xE0, "\xE0");
CHECK(xE1, "\xE1");
CHECK(xE2, "\xE2");
CHECK(xE3, "\xE3");
CHECK(xE4, "\xE4");
CHECK(xE5, "\xE5");
CHECK(xE6, "\xE6");
CHECK(xE7, "\xE7");
CHECK(xE8, "\xE8");
CHECK(xE9, "\xE9");
CHECK(xEA, "\xEA");
CHECK(xEB, "\xEB");
CHECK(xEC, "\xEC");
CHECK(xED, "\xED");
CHECK(xEE, "\xEE");
CHECK(xEF, "\xEF");

CHECK(xF0, "\xF0");
CHECK(xF1, "\xF1");
CHECK(xF2, "\xF2");
CHECK(xF3, "\xF3");
CHECK(xF4, "\xF4");
CHECK(xF5, "\xF5");
CHECK(xF6, "\xF6");
CHECK(xF7, "\xF7");
CHECK(xF8, "\xF8");
CHECK(xF9, "\xF9");
CHECK(xFA, "\xFA");
CHECK(xFB, "\xFB");
CHECK(xFC, "\xFC");
CHECK(xFD, "\xFD");
CHECK(xFE, "\xFE");
CHECK(xFF, "\xFF");