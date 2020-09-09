#pragma once

#include <array>
#include <stdexcept>
#include <string_view>
#include <limits>

namespace ctve
{
namespace impl
{
  template <typename T>
  constexpr void enforce(T&& t)
  {
    if (!t)
      throw std::runtime_error("Condition failed");
  }
} // namespace impl

template <size_t Len = 0>
class static_string
{
  // TODO: maybe use langeuage array char[] instead of std::array
  // TODO: Unicode/char32_t instead of char (like ctll::fixed_string uses)
  std::array<char, Len + 1> data_{};
  size_t len_{};

public:
  constexpr static_string() {}

  template <size_t OtherLen>
  constexpr static_string(const static_string<OtherLen> other)
  {
    impl::enforce(Len >= OtherLen);
    for (size_t i = 0; i < other.size(); i++)
      data_[i] = other[i];
    len_ = other.size();
  }

  template <size_t BufLen>
  constexpr static_string(const char (&buf)[BufLen])
  {
    for (size_t i = 0; i < BufLen - 1; i++)
      data_[i] = buf[i];
    len_ = BufLen - 1;
  }

  constexpr const char& operator[](size_t i) const
  {
    impl::enforce(i < size());
    return data_[i];
  }

  constexpr const char* data() const { return data_.data(); }

  constexpr const char* c_str() const { return data(); }

  constexpr bool empty() const { return !len_; }

  constexpr size_t size() const { return len_; }

  constexpr auto begin() const { return data_.begin(); }

  constexpr auto end() const { return begin() + size(); }

  constexpr operator std::string_view() const { return {data(), size()}; }

  [[nodiscard]] constexpr auto
  substr(size_t pos = 0, size_t count = std::string_view::npos) const
  {
    impl::enforce(pos <= size());
    static_string<Len> res;
    auto itr = begin() + pos;
    while (itr != end())
    {
      if (res.size() == count)
        break;
      res += *(itr++);
    }
    return res;
  }

  constexpr static_string& operator+=(char c)
  {
    impl::enforce(size() + 1 <= Len);
    data_[len_++] = c;
    return *this;
  }

  template <size_t BufLen>
  constexpr static_string& operator+=(const char (&buf)[BufLen])
  {
    impl::enforce(size() + BufLen - 1 <= Len);
    for (size_t i = 0; i < BufLen - 1; i++)
      data_[len_ + i] = buf[i];
    len_ += BufLen - 1;
    return *this;
  }

  template <size_t Len2>
  constexpr static_string& operator+=(const static_string<Len2> other)
  {
    impl::enforce(size() + other.size() <= Len);
    for (size_t i = 0; i < other.size(); i++)
      data_[len_ + i] = other[i];
    len_ += other.size();
    return *this;
  }
};

template <size_t BufLen>
static_string(const char (&buf)[BufLen])->static_string<BufLen - 1>;

template <size_t OtherLen>
static_string(const static_string<OtherLen> other)->static_string<OtherLen>;

template <size_t Len>
constexpr auto operator+(const static_string<Len>& str, char c)
{
  static_string<Len + 1> res = str;
  res += c;
  return res;
}

template <size_t Len>
constexpr auto operator+(char c, const static_string<Len>& str)
{
  static_string<Len + 1> res;
  res += c;
  res += str;
  return res;
}

template <size_t Len, size_t BufLen>
constexpr auto operator+(const static_string<Len>& str,
                         const char (&buf)[BufLen])
{
  static_string<Len + BufLen - 1> res = str;
  res += buf;
  return res;
}

template <size_t Len, size_t BufLen>
constexpr auto operator+(const char (&buf)[BufLen],
                         const static_string<Len>& str)
{
  static_string<Len + BufLen - 1> res = buf;
  res += str;
  return res;
}

template <size_t Len, size_t Len2>
constexpr auto operator+(const static_string<Len>& str,
                         const static_string<Len2>& str2)
{
  static_string<Len + Len2> res = str;
  res += str2;
  return res;
}

template <typename Int>
constexpr auto to_string(Int num)
{
  static_string<std::numeric_limits<Int>::digits + 1> reverseRes;
  static_string<std::numeric_limits<Int>::digits + 1> res;

  if constexpr (std::numeric_limits<Int>::is_signed)
  {
    if (num < 0)
    {
      res += '-';
      num = -num;
    }
  }

  if (!num)
    reverseRes += '0';
  else
  {
    while (num)
    {
      reverseRes += '0' + (num % 10);
      num /= 10;
    }
  }

  for (size_t i = 0; i < reverseRes.size(); i++)
    res += reverseRes[reverseRes.size() - 1 - i];

  return res;
}

} // namespace ctve
