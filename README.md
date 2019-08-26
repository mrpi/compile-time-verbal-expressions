# Compile-time VerbalExpressions

## C++ Regular Expressions made easy

CRVE is a C++17 library that helps construct difficult regular expressions.
The regular expressions are build at compile time and can be used with compile time or runtime regular expression libraries.

It is based on the ideas of https://github.com/VerbalExpressions and powered by the wish to use Hana Dusíkovás fantastic [compile time regular expression library](https://github.com/hanickadot/compile-time-regular-expressions) without having to maintain code sprinkled with dozens of hard to read regular expressions.

Try online: https://godbolt.org/z/JV7XAR

## Example

```c++
#include <iostream>
#include <ctve.hpp>

using namespace ctve;

static constexpr auto urlPattern = find("http")
                                 + maybe('s')
                                 + then("://")
                                 + maybe("www.")
                                 + capture(something_but(" /:"))
                                 + maybe(then(':') + capture(digit.one_or_more()))
                                 + maybe(capture(then('/') + anything_but(' ')));

int main()
{
  auto&& [match, host, port, path] 
      = ctre::match<urlPattern>("https://github.com/mrpi/compile-time-verbal-expressions");
  if (match)
  {
    std::cout << "host: " << std::string_view{host} << std::endl;
    std::cout << "port: " << std::string_view{port} << std::endl;
    std::cout << "path: " << std::string_view{path} << std::endl;
  }
}
```
