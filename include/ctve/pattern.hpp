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
  constexpr explicit pattern(const static_string<StrLen>& str) : str(str)
  {
  }

private:
  template <size_t Len2>
  [[nodiscard]] constexpr auto addQuantifier(static_string<Len2> val) const
  {
    return pattern<4 + Len + Len2>{"(?:" + str + ")" + val};
  }

public:
  constexpr auto begin() const { return str.begin(); }

  constexpr auto end() const { return str.end(); }

  constexpr size_t size() const { return str.size(); }

  constexpr bool empty() const { return str.empty(); }

  constexpr decltype(auto) operator[](size_t idx) const { return str[idx]; }

  [[nodiscard]] constexpr auto zero_or_more() const
  {
    // TODO: adding multiple quantificatoins ("a*+") should fail to compile
    // distinguish between qunatified templates and unquantified templates
    return addQuantifier(static_string{"*"});
  }

  [[nodiscard]] constexpr auto one_or_more() const
  {
    // TODO: adding multiple quantificatoins ("a*+") should fail to compile
    // distinguish between qunatified templates and unquantified templates
    return addQuantifier(static_string{"+"});
  }

  [[nodiscard]] constexpr auto at_least(size_t cnt) const
  {
    return addQuantifier("{" + to_string(cnt) + ",}");
  }

  [[nodiscard]] constexpr auto at_most(size_t cnt) const
  {
    return addQuantifier("{0," + to_string(cnt) + "}");
  }

  [[nodiscard]] constexpr auto count(size_t cnt) const
  {
    return addQuantifier("{" + to_string(cnt) + "}");
  }

  [[nodiscard]] constexpr auto count(size_t least, size_t most) const
  {
    return addQuantifier("{" + to_string(least) + "," + to_string(most) + "}");
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

template <size_t BufLen, size_t Len>
constexpr auto operator||(const char (&buf)[BufLen], const pattern<Len>& p)
{
  return pattern{"(?:" + static_string{buf} + ")|(?:" + p.str + ")"};
}

template <size_t BufLen, size_t Len>
constexpr auto operator||(const pattern<Len>& p, const char (&buf)[BufLen])
{
  return pattern{"(?:" + p.str + ")|(?:" + static_string{buf} + ")"};
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