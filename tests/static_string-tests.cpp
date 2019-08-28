#include "ctve/static_string.hpp"

#include <string_view>

using namespace std::string_view_literals;

// empty string
static inline constexpr auto emptyStr = ctve::static_string{};
static_assert(
    std::is_same_v<ctve::static_string<0>, std::decay_t<decltype(emptyStr)>>);
static_assert(std::is_same_v<ctve::static_string<0>,
                             std::decay_t<decltype(emptyStr + emptyStr)>>);
static_assert(emptyStr.empty());
static_assert(emptyStr.size() == 0);
static_assert(emptyStr == ""sv);
static_assert(emptyStr != "a"sv);
static_assert(emptyStr.data()[0] == '\0');
static_assert(emptyStr.begin() == emptyStr.end());

static inline constexpr auto oneCharStr = ctve::static_string{"a"};
static_assert(
    std::is_same_v<ctve::static_string<1>, std::decay_t<decltype(oneCharStr)>>);
static_assert(std::is_same_v<ctve::static_string<1>,
                             std::decay_t<decltype(emptyStr + oneCharStr)>>);
static_assert(!oneCharStr.empty());
static_assert(oneCharStr.size() == 1);
static_assert(oneCharStr != ""sv);
static_assert(oneCharStr == "a"sv);
static_assert(oneCharStr.data()[0] == 'a');
static_assert(oneCharStr.data()[1] == '\0');
static_assert(oneCharStr.begin() != oneCharStr.end());
static_assert(oneCharStr.begin() + 1 == oneCharStr.end());

static inline constexpr auto twoCharStr = oneCharStr + ctve::static_string{"b"};
static_assert(
    std::is_same_v<ctve::static_string<2>, std::decay_t<decltype(twoCharStr)>>);
static_assert(twoCharStr == "ab"sv);
static_assert(twoCharStr.size() == 2);

static inline constexpr auto threeCharStr = ctve::static_string<3>{"ab"};
static_assert(std::is_same_v<ctve::static_string<3>,
                             std::decay_t<decltype(threeCharStr)>>);
static_assert(threeCharStr == "ab"sv);
static_assert(threeCharStr.size() == 2);

static_assert(ctve::to_string(0) == "0"sv);
static_assert(ctve::to_string(1) == "1"sv);
static_assert(ctve::to_string(42) == "42"sv);
static_assert(ctve::to_string(314) == "314"sv);
static_assert(ctve::to_string(-314) == "-314"sv);
static_assert(ctve::to_string(2147483648) == "2147483648"sv);
static_assert(ctve::to_string(-2147483649) == "-2147483649"sv);
