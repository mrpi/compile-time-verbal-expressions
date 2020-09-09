# Compile-time VerbalExpressions

## C++ Regular Expressions made easy

CTVE is a C++17 library that helps construct difficult regular expressions.
The regular expressions are build at compile time and can be used with compile time or runtime regular expression libraries.

It is based on the ideas of https://github.com/VerbalExpressions and powered by the wish to use Hana Dusíkovás fantastic [compile time regular expression library](https://github.com/hanickadot/compile-time-regular-expressions) without having to maintain code sprinkled with dozens of hard to read regular expressions.

Try online: https://godbolt.org/z/9j1x75

## Example

```c++
#include <ctve.hpp>
#include <ctre.hpp>
#include <iostream>

namespace patterns
{
using namespace ctve;

static constexpr auto url =
    "http"
    + maybe('s')
    + "://"
    + maybe("www.")
    + capture(something_not_in(' ', '/', ':'))
    + maybe(':' + capture(digit.one_or_more()))
    + maybe(capture('/' + anything_not_in(' ')));
}

int main()
{
  auto&& [match, host, port, path] 
      = ctre::match<patterns::url>("https://github.com/mrpi/compile-time-verbal-expressions");
  if (match)
  {
    std::cout << "host: " << std::string_view{host} << '\n';
    std::cout << "port: " << std::string_view{port} << '\n';
    std::cout << "path: " << std::string_view{path} << '\n';
  }
}
```

## Syntax

| Code | Matches | PCRE |  |  |
|-|-|-|-|-|
| 'x' | exactly the given character | (?:x) |  |  |
| "xyz" | exactly the given character sequence | (?:xzy) |  |  |
| find("xyz") | exactly the given character sequence | (?:xzy) |  |  |
| maybe(".") | empty string or a single dot | (?:\\.)? |  |  |
| maybe(any_char) | empty string or one char | (?:.)? |  |  |
| digit | a single digit | [\d] ||
| digit.one_or_more() | one or more digits | (?:[\d])+ ||
| digit.zero_or_more() | zero or more digits | (?:[\d])* ||
| digit.at_least(2) | two or more digits | (?:[\d]){2,} ||
| digit.at_most(3) | up to three digits | (?:[\d]){0,3} ||
| digit.count(2,3) | two or three digits | (?:[\d]){2,3} ||
| anything_not_in(' ', '/') | string but not space or slash | [^ /]* ||
| something_not_in(' ', '/') | non empty string but not space or slash | [^ /]+ ||
| capture(word_char.count(3)) | capture three word char | ((?:[\w]){3}) ||
| "Hello" + maybe(whitespace) | "Hello" optionally followed by a whitespace | (?:Hello)(?:[\s])? ||
| find("Hello") \|\| "Hallo" | "Hello" or "Hallo" | (?:Hello)\|(?:Hallo) ||
