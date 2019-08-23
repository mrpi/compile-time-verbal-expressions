# Compile-time VerbalExpressions

```c++
#include <iostream>
#include <ctve.hpp>

static constexpr auto urlPattern = find("http")
                                 + maybe('s')
                                 + then("://")
                                 + maybe("www.")
                                 + capture(something_but(" /:"))
                                 + maybe(then(':') + capture(digit.one_or_more()))
                                 + maybe(capture(then('/') + anything_but(' ')));

int main()
{
  if (auto&& [match, host, port, path] 
      = ctre::match<urlPattern>("https://github.com/mrpi/compile-time-verbal-expressions"))
  {
    std::cout << "host: " << std::string_view{host} << std::endl;
    std::cout << "port: " << std::string_view{port} << std::endl;
    std::cout << "path: " << std::string_view{path} << std::endl;
  }
}
```
