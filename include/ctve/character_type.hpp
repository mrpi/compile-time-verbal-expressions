#pragma once

#include "pattern.hpp"

namespace ctve::impl
{
template <size_t Len>
struct character_type : pattern<Len>
{
  template <size_t BufLen>
  constexpr explicit character_type(const char (&buf)[BufLen])
   : pattern<Len>{static_string{buf}}
  {
  }
};

template <size_t BufLen>
character_type(const char (&buf)[BufLen])->character_type<BufLen - 1>;

template <typename T>
struct is_character_type : std::false_type
{
};

template <size_t Len>
struct is_character_type<character_type<Len>> : std::true_type
{
};

} // namespace ctve::impl
