#ifndef CTVE_HPP
#define CTVE_HPP

#include "ctve/function_builder.hpp"
#include "ctve/pattern.hpp"
#include "ctve/static_string.hpp"
namespace ctve
{

static inline constexpr auto find = impl::str_fn("(?:", ")");
static inline constexpr auto then = find;
static inline constexpr auto maybe = impl::str_or_pattern_fn("(?:", ")?");
static inline constexpr auto not_ = impl::str_or_pattern_fn("(?!", ")");
static inline constexpr auto in = impl::chrclass_fn("[", "]");
static inline constexpr auto not_in = impl::chrclass_fn("[^", "]");
static inline constexpr auto capture = impl::str_or_pattern_fn("(", ")");
static inline constexpr auto something_not_in = impl::chrclass_fn("[^", "]+");
static inline constexpr auto anything_not_in = impl::chrclass_fn("[^", "]*");

static inline constexpr auto whitespace = impl::character_type{"\\s"};
static inline constexpr auto non_whitespace = impl::character_type{"\\S"};
static inline constexpr auto word_char = impl::character_type{"\\w"};
static inline constexpr auto non_word_char = impl::character_type{"\\W"};
static inline constexpr auto word_boundary = impl::character_type{"\\b"};
static inline constexpr auto digit = impl::character_type{"\\d"};
static inline constexpr auto non_digit = impl::character_type{"\\D"};
static inline constexpr auto tab = impl::character_type{"\\t"};

static inline constexpr auto xdigit = in(digit, range{ 'a', 'f' }, range{ 'A', 'F' });

namespace posix
{
static inline constexpr auto alnum = impl::character_type{"[:alnum:]"};
static inline constexpr auto alpha = impl::character_type{"[:alpha:]"};
static inline constexpr auto ascii = impl::character_type{"[:ascii:]"};
static inline constexpr auto blank = impl::character_type{"[:blank:]"};
static inline constexpr auto cntrl = impl::character_type{"[:cntrl:]"};
static inline constexpr auto digit = impl::character_type{"[:digit:]"};
static inline constexpr auto graph = impl::character_type{"[:graph:]"};
static inline constexpr auto lower = impl::character_type{"[:lower:]"};
static inline constexpr auto print = impl::character_type{"[:print:]"};
static inline constexpr auto punct = impl::character_type{"[:punct:]"};
static inline constexpr auto space = impl::character_type{"[:space:]"};
static inline constexpr auto upper = impl::character_type{"[:upper:]"};
static inline constexpr auto word = impl::character_type{"[:word:]"};
static inline constexpr auto xdigit = impl::character_type{"[:xdigit:]"};
}

static inline constexpr auto any_char = pattern{static_string{"."}};
static inline constexpr auto word = pattern{static_string{"\\w+"}};
static inline constexpr auto line_break =
    pattern{static_string{"(?:\\r\\n|\\r|\\n)"}};
static inline constexpr auto br = line_break;
static inline constexpr auto something = pattern{static_string{"(?:.+)"}};
static inline constexpr auto anything = pattern{static_string{"(?:.*)"}};

template <size_t Len, size_t BufLen>
constexpr auto operator+(const pattern<Len>& pattern, const char (&buf)[BufLen])
{
  return pattern + find(buf);
}

template <size_t BufLen, size_t Len>
constexpr auto operator+(const char (&buf)[BufLen], const pattern<Len>& pattern)
{
  return find(buf) + pattern;
}

template <size_t Len>
constexpr auto operator+(const pattern<Len>& pattern, char c)
{
  return pattern + find(c);
}

template <size_t Len>
constexpr auto operator+(char c, const pattern<Len>& pattern)
{
  return find(c) + pattern;
}

template <size_t Len1, size_t Len2>
constexpr auto operator||(const pattern<Len1>& p1, const pattern<Len2>& p2)
{
  return pattern{"(?:" + p1.str + ")|(?:" + p2.str + ")"};
}

template <size_t BufLen, size_t Len>
constexpr auto operator||(const char (&buf)[BufLen], const pattern<Len>& p)
{
  return find(buf) || p;
}

template <size_t Len>
constexpr auto operator||(char c, const pattern<Len>& p)
{
  return find(c) || p;
}

template <size_t BufLen, size_t Len>
constexpr auto operator||(const pattern<Len>& p, const char (&buf)[BufLen])
{
  return p || find(buf);
}

template <size_t Len>
constexpr auto operator||(const pattern<Len>& p, const char c)
{
  return p || find(c);
}

} // namespace ctve

#endif // CTVE_HPP
