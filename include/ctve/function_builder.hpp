#ifndef CTVE_FUNC_BUILDER_HPP
#define CTVE_FUNC_BUILDER_HPP

#include "character_type.hpp"
#include "pattern.hpp"
#include "range.hpp"

namespace ctve::impl
{
template <size_t Len1, size_t Len2>
struct str_or_pattern_fn
{
  static_string<Len1> prefix;
  static_string<Len2> suffix;

  template <size_t BufLen1, size_t BufLen2>
  constexpr str_or_pattern_fn(const char (&prefix)[BufLen1],
                              const char (&suffix)[BufLen2])
   : prefix{prefix}, suffix{suffix}
  {
  }

  template <typename T>
  constexpr auto operator()(T&& buf) const
  {
    static_assert(!std::is_same_v<range, std::decay_t<decltype(buf)>>,
                  "Only string literals allowed!");
    return pattern{static_string{prefix + sanitize(buf) + suffix}};
  }
};

template <size_t BufLen1, size_t BufLen2>
str_or_pattern_fn(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2])
    ->str_or_pattern_fn<BufLen1 - 1, BufLen2 - 1>;

template <size_t Len1, size_t Len2>
struct chrclass_fn
{
  static_string<Len1> prefix;
  static_string<Len2> suffix;

  template <size_t BufLen1, size_t BufLen2>
  constexpr chrclass_fn(const char (&prefix)[BufLen1],
                        const char (&suffix)[BufLen2])
   : prefix{prefix}, suffix{suffix}
  {
  }

  template <typename T>
  static constexpr auto to_str(T&& buf)
  {
    if constexpr (impl::is_character_type<std::decay_t<decltype(buf)>>::value)
      return buf.str.substr(1, buf.str.size() - 2);
    else
    {
      static_assert(std::is_same_v<std::decay_t<decltype(buf)>, char> ||
                        std::is_same_v<std::decay_t<decltype(buf)>, range>,
                    "Only characters, character types and ranges allowed!");
      return sanitize(buf);
    }
  }

  template <typename T, typename... Args>
  static constexpr auto to_str(T&& buf, Args&&... args)
  {
    return to_str(buf) + to_str(args...);
  }

  template <typename... T>
  constexpr auto operator()(T&&... t) const
  {
    return pattern{static_string{prefix + to_str(t...) + suffix}};
  }
};

template <size_t BufLen1, size_t BufLen2>
chrclass_fn(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2])
    ->chrclass_fn<BufLen1 - 1, BufLen2 - 1>;

template <size_t Len1, size_t Len2>
struct str_fn
{
  static_string<Len1> prefix;
  static_string<Len2> suffix;

  template <size_t BufLen1, size_t BufLen2>
  constexpr str_fn(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2])
   : prefix{prefix}, suffix{suffix}
  {
  }

  template <typename T>
  constexpr auto operator()(T&& buf) const
  {
    static_assert(!std::is_same_v<range, std::decay_t<decltype(buf)>>,
                  "Only string literals allowed!");
    static_assert(!is_pattern<std::decay_t<decltype(buf)>>::value,
                  "Only string literals allowed!");
    return pattern{static_string{prefix + sanitize(buf) + suffix}};
  }
};

template <size_t BufLen1, size_t BufLen2>
str_fn(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2])
    ->str_fn<BufLen1 - 1, BufLen2 - 1>;

} // namespace ctve::impl

#endif // CTVE_FUNC_BUILDER_HPP
