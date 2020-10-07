#ifndef CTVE_REGEX_UTIL_HPP
#define CTVE_REGEX_UTIL_HPP

namespace ctve::impl
{

constexpr bool needsEscaping(char c)
{
  switch (c)
  {
  case '\\':
  case '^':
  case '$':
  case '*':
  case '+':
  case '?':
  case '.':
  case '(':
  case ')':
  case '|':
  case '{':
  case '}':
  case '[':
  case ']':
  case '-':
    return true;
  default:
    return false;
  }
}

template <size_t BufLen>
[[nodiscard]] constexpr auto sanitize(const char (&buf)[BufLen])
{
  static_string<(BufLen - 1) * 2> res;
  for (size_t i = 0; i < BufLen - 1; i++)
  {
    auto c = buf[i];
    if (needsEscaping(c))
      res += '\\';
    res += c;
  }
  return res;
}

[[nodiscard]] constexpr auto sanitize(char c)
{
  static_string<2> res;
  if (needsEscaping(c))
    res += '\\';
  res += c;
  return res;
}

} // namespace ctve::impl

#endif // CTVE_REGEX_UTIL_HPP
