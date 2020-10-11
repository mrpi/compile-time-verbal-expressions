#include "../examples/utf8-validation.hpp"

using namespace std::literals;

constexpr bool correctForSingeByteChars()
{
    for (int i=0; i < 128; i++)
    {
        char c = (unsigned char)i;
        std::string_view sv{&c, 1};

        if (!ctve::utf8::is_valid(sv))
           return false;
    }

    for (int i=128; i < 256; i++)
    {
        char c = (unsigned char)i;
        std::string_view sv{&c, 1};

        if (ctve::utf8::is_valid(sv))
           return false;
    }

    return true;
}

static_assert(ctve::utf8::is_valid(""), "utf8 detection failed for empty string");

static_assert(correctForSingeByteChars(), "utf8 detection failed for single byte chars");

static_assert(!ctve::utf8::is_valid("\xC0\x80"), "utf8 detection succeeded for two byte char in codepoint 0-127 range");
static_assert(ctve::utf8::is_valid(u8"ü"), "utf8 detection failed for valid two byte char");

static_assert(ctve::utf8::is_valid(u8"€"), "utf8 detection failed for valid three byte char");
static_assert(!ctve::utf8::is_valid("\xE0\x80\x80"), "utf8 detection succeeded for invalid three byte char");
static_assert(!ctve::utf8::is_valid("\xED\xA0\x80"), "utf8 detection succeeded for invalid three byte char");

static_assert(ctve::utf8::is_valid(u8"🦄"), "utf8 detection failed for valid four byte char");

static_assert(ctve::utf8::is_valid(u8"漢語 / 汉语"), "utf8 detection failed for valid chinese sequence");

static_assert(!ctve::utf8::is_valid("\xF0\x80\x80\x80"), "utf8 detection succeeded for invalid four byte char");
static_assert(!ctve::utf8::is_valid("\xF5\x80\x80\x80"), "utf8 detection succeeded for out of range four byte char");
static_assert(!ctve::utf8::is_valid("\xF8\x80\x80\x80\x80"), "utf8 detection succeeded for five byte char");
static_assert(!ctve::utf8::is_valid("\xFC\x80\x80\x80\x80\x80"), "utf8 detection succeeded for five byte char");

static_assert(ctve::utf8::is_valid("\x00 abc"sv), "utf8 detection failed on string with zero byte");
static_assert(!ctve::utf8::is_valid("\x00 \xF5"sv), "utf8 detection succeeded on invalid string with zero byte");
