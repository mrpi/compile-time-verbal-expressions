#include <ctre.hpp>
#include <ctve.hpp>
#include <string_view>

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

static_assert(CTRE_CREATE(digit).match("1"sv));
static_assert(!CTRE_CREATE(digit).match("12"sv));
static_assert(!CTRE_CREATE(digit).match("a"sv));

static inline constexpr auto digits = digit.one_or_more();
static_assert(!CTRE_CREATE(digits).match(""sv));
static_assert(CTRE_CREATE(digits).match("9"sv));
static_assert(CTRE_CREATE(digits).match("42"sv));

static inline constexpr auto findDot = find(".");
static_assert(!CTRE_CREATE(findDot).match(""sv));
static_assert(CTRE_CREATE(findDot).match("."sv));
static_assert(!CTRE_CREATE(findDot).match("a"sv));

static inline constexpr auto maybeFoo = maybe("foo");
static_assert(CTRE_CREATE(maybeFoo).match(""sv));
static_assert(!CTRE_CREATE(maybeFoo).match("f"sv));
static_assert(CTRE_CREATE(maybeFoo).match("foo"sv));
static_assert(!CTRE_CREATE(maybeFoo).match("bar"sv));
static_assert(!CTRE_CREATE(maybeFoo).match("foobar"sv));

static inline constexpr auto maybe_f_dot_o = maybe("f.o");
static_assert(CTRE_CREATE(maybe_f_dot_o).match("f.o"sv));
static_assert(!CTRE_CREATE(maybe_f_dot_o).match("foo"sv));

static inline constexpr auto maybe_f_any_o =
    maybe(find('f') + any_char + then('o'));
static_assert(CTRE_CREATE(maybe_f_any_o).match("f.o"sv));
static_assert(CTRE_CREATE(maybe_f_any_o).match("foo"sv));
static_assert(!CTRE_CREATE(maybe_f_any_o).match("f..o"sv));

static inline constexpr auto ctve_is_not_not_great =
    find("CTVE is ") + not_("not ") + maybe(word + whitespace) + then("great!");
static_assert(CTRE_CREATE(ctve_is_not_not_great).match("CTVE is great!"sv));
static_assert(
    !CTRE_CREATE(ctve_is_not_not_great).match("CTVE is not great!"sv));
static_assert(
    CTRE_CREATE(ctve_is_not_not_great).match("CTVE is very great!"sv));

static inline constexpr auto b_to_f = any_of(range{'b', 'f'});
static_assert(!CTRE_CREATE(b_to_f).match("a"sv));
static_assert(CTRE_CREATE(b_to_f).match("b"sv));
static_assert(!CTRE_CREATE(b_to_f).match("bc"sv));
static_assert(CTRE_CREATE(b_to_f).match("c"sv));
static_assert(CTRE_CREATE(b_to_f).match("f"sv));
static_assert(!CTRE_CREATE(b_to_f).match("g"sv));

static inline constexpr auto wordChar = any_of(word_char);
static_assert(CTRE_CREATE(wordChar).match("a"sv));
static_assert(CTRE_CREATE(wordChar).match("Z"sv));
static_assert(CTRE_CREATE(wordChar).match("0"sv));
static_assert(CTRE_CREATE(wordChar).match("9"sv));

static inline constexpr auto hexadecimalNumber =
    maybe("0x") + any_of(digit, range{'a', 'f'}, range{'A', 'F'}).one_or_more();
static_assert(CTRE_CREATE(hexadecimalNumber).match("0x0"sv));
static_assert(CTRE_CREATE(hexadecimalNumber).match("0xff"sv));
static_assert(CTRE_CREATE(hexadecimalNumber).match("3B"sv));
static_assert(!CTRE_CREATE(hexadecimalNumber).match("0xggg"sv));

static inline constexpr auto nanOrHexadecimalNumber =
    find("NaN") || hexadecimalNumber;
static_assert(CTRE_CREATE(nanOrHexadecimalNumber).match("NaN"sv));
static_assert(!CTRE_CREATE(nanOrHexadecimalNumber).match("NaN0x0"sv));
static_assert(CTRE_CREATE(nanOrHexadecimalNumber).match("0x0"sv));
static_assert(CTRE_CREATE(nanOrHexadecimalNumber).match("0xff"sv));
static_assert(CTRE_CREATE(nanOrHexadecimalNumber).match("3B"sv));
static_assert(!CTRE_CREATE(nanOrHexadecimalNumber).match("0xggg"sv));

static inline constexpr auto captureName =
    find("Hello ") + capture(word) + maybe('!');
static_assert(CTRE_CREATE(captureName).match("Hello you!"sv).get<1>() ==
              "you"sv);

// would be bad, if the readme example fails unnoticed ;-)
static constexpr auto urlPattern =
    find("http")                                      //
    + maybe('s')                                      //
    + then("://")                                     //
    + maybe("www.")                                   //
    + capture(something_but(" /:"))                   //
    + maybe(then(':') + capture(digit.one_or_more())) //
    + maybe(capture(then('/') + anything_but(' ')));
static constexpr auto testUrl =
    "https://www.github.com:443/mrpi/compile-time-verbal-expressions"sv;
static_assert(CTRE_CREATE(urlPattern).match(testUrl));
static_assert(CTRE_CREATE(urlPattern).match(testUrl).get<1>() ==
              "github.com"sv);
static_assert(CTRE_CREATE(urlPattern).match(testUrl).get<2>() == "443"sv);
static_assert(CTRE_CREATE(urlPattern).match(testUrl).get<3>() ==
              "/mrpi/compile-time-verbal-expressions"sv);
