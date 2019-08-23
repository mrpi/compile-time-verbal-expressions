#include <ctre.hpp>
#include <ctve.hpp>
#include <string_view>

template <auto& input> constexpr auto create() {
	using tmp = typename ctll::parser<ctre::pcre, input, ctre::pcre_actions>::template output<ctre::pcre_context<>>;
	static_assert(tmp(), "Regular Expression contains syntax error.");
	using re = decltype(front(typename tmp::output_type::stack_type()));
	return ctre::regular_expression(re());
}

#define CTRE_CREATE(pattern) create<pattern>()

using namespace std::string_view_literals;

static_assert(CTRE_CREATE(ctve::digit).match("1"sv));
static_assert(!CTRE_CREATE(ctve::digit).match("12"sv));
static_assert(!CTRE_CREATE(ctve::digit).match("a"sv));

static inline constexpr auto digits = ctve::digit.one_or_more();
static_assert(!CTRE_CREATE(digits).match(""sv));
static_assert(CTRE_CREATE(digits).match("9"sv));
static_assert(CTRE_CREATE(digits).match("42"sv));

static inline constexpr auto findDot = ctve::find(".");
static_assert(!CTRE_CREATE(findDot).match(""sv));
static_assert(CTRE_CREATE(findDot).match("."sv));
static_assert(!CTRE_CREATE(findDot).match("a"sv));

static inline constexpr auto maybeFoo = ctve::maybe("foo");
static_assert(CTRE_CREATE(maybeFoo).match(""sv));
static_assert(!CTRE_CREATE(maybeFoo).match("f"sv));
static_assert(CTRE_CREATE(maybeFoo).match("foo"sv));
static_assert(!CTRE_CREATE(maybeFoo).match("bar"sv));
static_assert(!CTRE_CREATE(maybeFoo).match("foobar"sv));

static inline constexpr auto maybeFDoto = ctve::maybe("f.o");
static_assert(CTRE_CREATE(maybeFDoto).match("f.o"sv));
static_assert(!CTRE_CREATE(maybeFDoto).match("foo"sv));
