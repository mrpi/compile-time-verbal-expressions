#ifndef CTVE_RANGE_HPP
#define CTVE_RANGE_HPP

#include "regex_utility.hpp"
#include "static_string.hpp"

namespace ctve
{
struct range
{
  char start;
  char end;

  constexpr range(char start, char end) : start(start), end(end) {}
};

namespace impl
{
  [[nodiscard]] constexpr auto sanitize(range r)
  {
    return sanitize(r.start) + "-" + sanitize(r.end);
  }
} // namespace impl
} // namespace ctve

#endif // CTVE_RANGE_HPP