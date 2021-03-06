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

static inline constexpr auto b_to_f = in(range{'b', 'f'});
static_assert(!CTRE_CREATE(b_to_f).match("a"sv));
static_assert(CTRE_CREATE(b_to_f).match("b"sv));
static_assert(!CTRE_CREATE(b_to_f).match("bc"sv));
static_assert(CTRE_CREATE(b_to_f).match("c"sv));
static_assert(CTRE_CREATE(b_to_f).match("f"sv));
static_assert(!CTRE_CREATE(b_to_f).match("g"sv));

static inline constexpr auto wordChar = in(word_char);
static_assert(CTRE_CREATE(wordChar).match("a"sv));
static_assert(CTRE_CREATE(wordChar).match("Z"sv));
static_assert(CTRE_CREATE(wordChar).match("0"sv));
static_assert(CTRE_CREATE(wordChar).match("9"sv));

static inline constexpr auto hexadecimalNumber =
    maybe("0x") + in(digit, range{'a', 'f'}, range{'A', 'F'}).one_or_more();
static_assert(CTRE_CREATE(hexadecimalNumber).match("0x0"sv));
static_assert(CTRE_CREATE(hexadecimalNumber).match("0xff"sv));
static_assert(CTRE_CREATE(hexadecimalNumber).match("3B"sv));
static_assert(!CTRE_CREATE(hexadecimalNumber).match("0xggg"sv));

static inline constexpr auto nanOrHexadecimalNumber =
    "NaN" || hexadecimalNumber;
static_assert(CTRE_CREATE(nanOrHexadecimalNumber).match("NaN"sv));
static_assert(!CTRE_CREATE(nanOrHexadecimalNumber).match("NaN0x0"sv));
static_assert(CTRE_CREATE(nanOrHexadecimalNumber).match("0x0"sv));
static_assert(CTRE_CREATE(nanOrHexadecimalNumber).match("0xff"sv));
static_assert(CTRE_CREATE(nanOrHexadecimalNumber).match("3B"sv));
static_assert(!CTRE_CREATE(nanOrHexadecimalNumber).match("0xggg"sv));

static inline constexpr auto captureName =
    "Hello " + capture(word) + maybe('!');
static_assert(CTRE_CREATE(captureName).match("Hello you!"sv).get<1>() ==
              "you"sv);

static inline constexpr auto zeroOrOneAnyChar = any_char.at_most(1);
static_assert(CTRE_CREATE(zeroOrOneAnyChar).match(""sv));
static_assert(CTRE_CREATE(zeroOrOneAnyChar).match("."sv));
static_assert(CTRE_CREATE(zeroOrOneAnyChar).match("a"sv));
static_assert(!CTRE_CREATE(zeroOrOneAnyChar).match("ab"sv));

static inline constexpr auto threeToFiveAB = in('a', 'b').count(3, 5);
static_assert(!CTRE_CREATE(threeToFiveAB).match(""sv));
static_assert(!CTRE_CREATE(threeToFiveAB).match("ab"sv));
static_assert(CTRE_CREATE(threeToFiveAB).match("aba"sv));
static_assert(CTRE_CREATE(threeToFiveAB).match("abab"sv));
static_assert(CTRE_CREATE(threeToFiveAB).match("ababb"sv));
static_assert(!CTRE_CREATE(threeToFiveAB).match("ababba"sv));

// would be bad, if the readme example fails unnoticed ;-)
static constexpr auto urlPattern =
    "http"                                       //
    + maybe('s')                                 //
    + "://"                                      //
    + maybe("www.")                              //
    + capture(something_not_in(' ', '/', ':'))   //
    + maybe(':' + capture(digit.one_or_more()))  //
    + maybe(capture('/' + anything_not_in(' ')));

static constexpr auto testUrl =
    "https://github.com:443/mrpi/compile-time-verbal-expressions"sv;

static_assert(CTRE_CREATE(urlPattern).match(testUrl));
static_assert(CTRE_CREATE(urlPattern).match(testUrl).get<1>() ==
              "github.com"sv);
static_assert(CTRE_CREATE(urlPattern).match(testUrl).get<2>() == "443"sv);
static_assert(CTRE_CREATE(urlPattern).match(testUrl).get<3>() ==
              "/mrpi/compile-time-verbal-expressions"sv);

static constexpr auto cOrPosixUpper = in(posix::xdigit, 'x').one_or_more();
static_assert(!CTRE_CREATE(cOrPosixUpper).match(""));
static_assert(CTRE_CREATE(cOrPosixUpper).match("a"));
static_assert(CTRE_CREATE(cOrPosixUpper).match("9"));
static_assert(CTRE_CREATE(cOrPosixUpper).match("A"));
static_assert(CTRE_CREATE(cOrPosixUpper).match("AB"));
static_assert(CTRE_CREATE(cOrPosixUpper).match("ABc"));
static_assert(CTRE_CREATE(cOrPosixUpper).match("ABcx"));
static_assert(!CTRE_CREATE(cOrPosixUpper).match("ABg"));

static constexpr auto upperOrDigit = posix::upper || posix::digit;
static_assert(CTRE_CREATE(upperOrDigit).match("A"));
static_assert(!CTRE_CREATE(upperOrDigit).match("a"));
static_assert(CTRE_CREATE(upperOrDigit).match("0"));

static constexpr auto escaped = (not_in('\\', '"', ']') || "\\\\" || "\\\"" || "\\]").one_or_more();
static_assert(!CTRE_CREATE(escaped).match(R"(ab\c)"));
static_assert(CTRE_CREATE(escaped).match(R"(ab\\c)"));
static_assert(!CTRE_CREATE(escaped).match(R"(ab"c)"));
static_assert(CTRE_CREATE(escaped).match(R"(ab\"c)"));
static_assert(!CTRE_CREATE(escaped).match(R"(ab]c)"));
static_assert(CTRE_CREATE(escaped).match(R"(ab\]c)"));

constexpr auto uuid1 = "3b42ecd2-647d-42b1-9b0c-b0df0f859384"sv;
constexpr auto notUuid1 = "3b42ecd2_647d_42b1_9b0c_b0df0f859384"sv;

static constexpr auto hexDigit = in(digit, range{'a', 'f'});
static constexpr auto isUuid = hexDigit.count(8) + '-' + hexDigit.count(4) + '-' + hexDigit.count(4) + '-' + hexDigit.count(4) + '-' + hexDigit.count(12);

template<auto& in>
constexpr auto ctveShrinkToFit() -> ctve::pattern<in.size()>
{
    return ctve::pattern{ctve::static_string<in.size()>{in.str}};
}

static constexpr auto isUuidShort = ctveShrinkToFit<isUuid>();

static_assert(CTRE_CREATE(isUuid).match(uuid1));
static_assert(!CTRE_CREATE(isUuid).match(notUuid1));

static constexpr auto isLikeUuid = in(digit, range{'a', 'f'}, '-').count(36);
static_assert(CTRE_CREATE(isLikeUuid).match(uuid1));
static_assert(!CTRE_CREATE(isLikeUuid).match(notUuid1));
