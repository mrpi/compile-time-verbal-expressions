#pragma once

#include "static_string.hpp"

namespace ctve
{
template <size_t Len = 0>
struct pattern
{
  static_string<Len> str;

  constexpr pattern() {}

  template <size_t StrLen>
  constexpr pattern(const static_string<StrLen>& str) : str(str)
  {
  }

private:
  template <size_t Len2>
  [[nodiscard]] constexpr auto add(static_string<Len2> val) const
  {
    return pattern<Len + Len2>{str + val};
  }

  template <size_t BufLen>
  [[nodiscard]] constexpr auto add(const char (&buf)[BufLen]) const
  {
    return pattern<Len + BufLen - 1>{str + buf};
  }

public:
  constexpr auto begin() const { return str.begin(); }

  constexpr auto end() const { return str.end(); }

  constexpr size_t size() const { return str.size(); }

  constexpr bool empty() const { return str.empty(); }

  constexpr decltype(auto) operator[](size_t idx) const { return str[idx]; }

  template <size_t BufLen>
  [[nodiscard]] constexpr auto start_of_line() const
  {
    return pattern<Len + 1>{"^" + str};
  }

  template <size_t BufLen>
  [[nodiscard]] constexpr auto end_of_line() const
  {
    return add("$");
  }

  [[nodiscard]] constexpr auto one_or_more() const
  {
    // TODO: adding multiple quantificatoins ("a*+") should fail to compile
    // distinguish between qunatified templates and unquantified temmplates
    return add("+");
  }

  friend inline std::ostream& operator<<(std::ostream& os, const pattern& p)
  {
    os << std::string_view{p.str};
    return os;
  }
};

template <size_t StrLen>
pattern(const static_string<StrLen>& str)->pattern<StrLen>;

template <size_t Len1, size_t Len2>
constexpr auto operator+(const pattern<Len1>& p1, const pattern<Len2>& p2)
{
  return pattern{p1.str + p2.str};
}

template <size_t Len1, size_t Len2>
constexpr auto operator||(const pattern<Len1>& p1, const pattern<Len2>& p2)
{
  return pattern{"(?:" + p1.str + ")|(?:" + p2.str + ")"};
}

namespace impl
{
  template <typename T>
  struct is_pattern : std::false_type
  {
  };

  template <size_t Len>
  struct is_pattern<pattern<Len>> : std::true_type
  {
  };

  template <size_t Len>
  [[nodiscard]] constexpr auto sanitize(pattern<Len> p)
  {
    return p.str;
  }
} // namespace impl

} // namespace ctve