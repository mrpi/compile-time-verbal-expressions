#include <ctre.hpp>

#include "ctve/character_type.hpp"
#include "ctve/function_builder.hpp"
#include "ctve/pattern.hpp"
#include "ctve/static_string.hpp"

namespace ctve
{

static inline constexpr auto find = impl::str_fn("(?:", ")");
static inline constexpr auto then = find;
static inline constexpr auto maybe = impl::str_or_pattern_fn("(?:", ")?");
static inline constexpr auto not_ = impl::str_or_pattern_fn("(?!", ")");
static inline constexpr auto any_of = impl::chrclass_fn("[", "]");
static inline constexpr auto any = any_of;
static inline constexpr auto capture = impl::str_or_pattern_fn("(", ")");
static inline constexpr auto something_but = impl::chrclass_fn("(?:[^", "]+)");
static inline constexpr auto anything_but = impl::chrclass_fn("(?:[^", "]*)");

static inline constexpr auto any_char = impl::character_type{"."};
static inline constexpr auto whitespace = impl::character_type{"\\s"};
static inline constexpr auto space = whitespace;
static inline constexpr auto non_space = impl::character_type{"\\S"};
static inline constexpr auto word_char = impl::character_type{"\\w"};
static inline constexpr auto non_word_char = impl::character_type{"\\W"};
static inline constexpr auto word_boundary = impl::character_type{"\\b"};
static inline constexpr auto digit = impl::character_type{"\\d"};
static inline constexpr auto non_digit = impl::character_type{"\\D"};
static inline constexpr auto tab = impl::character_type{"\\t"};

static inline constexpr auto word = pattern{static_string{"\\w+"}};
static inline constexpr auto line_break =
    pattern{static_string{"(?:\\r\\n|\\r|\\n)"}};
static inline constexpr auto br = line_break;
static inline constexpr auto something = pattern{static_string{"(?:.+)"}};
static inline constexpr auto anything = pattern{static_string{"(?:.*)"}};

} // namespace ctve
