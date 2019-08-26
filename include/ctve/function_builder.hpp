#pragma once

#include "pattern.hpp"
#include "range.hpp"

namespace ctve::impl
{
template <size_t Len1, size_t Len2>
struct sanitized_str_or_pattern
{
  static_string<Len1> prefix;
  static_string<Len2> suffix;

  template <size_t BufLen1, size_t BufLen2>
  constexpr sanitized_str_or_pattern(const char (&prefix)[BufLen1],
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
sanitized_str_or_pattern(const char (&prefix)[BufLen1],
                         const char (&suffix)[BufLen2])
    ->sanitized_str_or_pattern<BufLen1 - 1, BufLen2 - 1>;

template <size_t Len1, size_t Len2>
struct sanitized_str_or_range
{
  static_string<Len1> prefix;
  static_string<Len2> suffix;

  template <size_t BufLen1, size_t BufLen2>
  constexpr sanitized_str_or_range(const char (&prefix)[BufLen1],
                                   const char (&suffix)[BufLen2])
   : prefix{prefix}, suffix{suffix}
  {
  }

  template <typename T>
  static constexpr auto to_str(T&& buf)
  {
    static_assert(!is_pattern<std::decay_t<decltype(buf)>>::value,
                  "Only string literals allowed!");
    return sanitize(buf);
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
sanitized_str_or_range(const char (&prefix)[BufLen1],
                       const char (&suffix)[BufLen2])
    ->sanitized_str_or_range<BufLen1 - 1, BufLen2 - 1>;

template <size_t Len1, size_t Len2>
struct sanitized_str
{
  static_string<Len1> prefix;
  static_string<Len2> suffix;

  template <size_t BufLen1, size_t BufLen2>
  constexpr sanitized_str(const char (&prefix)[BufLen1],
                          const char (&suffix)[BufLen2])
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
sanitized_str(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2])
    ->sanitized_str<BufLen1 - 1, BufLen2 - 1>;

} // namespace ctve::impl