#include <ctre.hpp>

namespace ctve
{    
    template<typename T>
    constexpr void enforce(T&& t)
    {
        if (!t)
           throw std::runtime_error("Condition failed");
    }

   template<size_t Len = 0>
   class static_string
   {
       std::array<char, Len+1> data_{};
       size_t len_{};

   public:
      constexpr static_string()
      {
      }    

      template<size_t OtherLen>
      constexpr static_string(const static_string<OtherLen> other)
      {
          enforce(Len >= OtherLen);
          for (size_t i=0; i < other.size(); i++)
             data_[i] = other[i];
          len_ = other.size();
      }

      template<size_t BufLen>
      constexpr static_string(const char (&buf)[BufLen])
      {
          for (size_t i=0; i < BufLen-1; i++)
             data_[i] = buf[i];
          len_ = BufLen-1;
      }

      constexpr const char& operator[](size_t i) const
      {
          enforce(i < size());
          return data_[i];
      }

      constexpr const char* data() const
      {
          return data_.data();
      }

      constexpr const char* c_str() const
      {
          return data();
      }

      constexpr size_t size() const
      {
          return len_;
      }

      constexpr auto begin() const
      {
          return data_.begin();
      }

      constexpr auto end() const
      {
          return begin() + size();
      }

      constexpr operator std::string_view() const
      {
          return {data(), size()};
      }

      constexpr static_string& operator+=(char c)
      {
          enforce(size()+1 <= Len);
          data_[len_++] = c;
          return *this;
      }

      template<size_t BufLen>
      constexpr static_string& operator+=(const char (&buf)[BufLen])
      {
          enforce(size()+BufLen-1 <= Len);
          for (size_t i=0; i < BufLen-1; i++)
             data_[len_+i] = buf[i];
          len_ += BufLen-1;
          return *this;
      }

      template<size_t Len2>
      constexpr static_string& operator+=(const static_string<Len2> other)
      {
          enforce(size()+other.size() <= Len);
          for (size_t i=0; i < other.size(); i++)
             data_[len_+i] = other[i];
          len_ += other.size();
          return *this;
      }
   };

   template<size_t BufLen>
   static_string(const char (&buf)[BufLen]) -> static_string<BufLen-1>;

   template<size_t OtherLen>
   static_string(const static_string<OtherLen> other) -> static_string<OtherLen>;

   template<size_t Len>
   constexpr auto operator+(const static_string<Len>& str, char c)
   {
       static_string<Len+1> res = str;
       res += c;
       return res;
   }

   template<size_t Len, size_t BufLen>
   constexpr auto operator+(const static_string<Len>& str, const char (&buf)[BufLen])
   {
       static_string<Len+BufLen-1> res = str;
       res += buf;
       return res;
   }

   template<size_t Len, size_t BufLen>
   constexpr auto operator+(const char (&buf)[BufLen], const static_string<Len>& str)
   {
       static_string<Len+BufLen-1> res = buf;
       res += str;
       return res;
   }

   template<size_t Len, size_t Len2>
   constexpr auto operator+(const static_string<Len>& str, const static_string<Len2>& str2)
   {
       static_string<Len+Len2> res = str;
       res += str2;
       return res;
   }

   constexpr bool needsEscaping(char c)
   {
       switch(c)
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
          return true;
       default:
          return false;
       }       
   }

   template<size_t BufLen>
   [[nodiscard]] constexpr auto sanitize(const char (&buf)[BufLen])
   {
      static_string<(BufLen-1)*2> res;
      for (size_t i=0; i < BufLen-1; i++)
      {
          auto c = buf[i];
          if (needsEscaping(c))
             res += '\\';
          res += c;
      }
      return res;
   }

   template<size_t Len = 0>
   struct pattern
   {
      static_string<Len> str;

      constexpr pattern()
      {}

      template<size_t StrLen>
      constexpr pattern(const static_string<StrLen>& str)
       : str(str)
      {}

   private:
      template<size_t Len2>
      [[nodiscard]] constexpr auto add(static_string<Len2> val) const
      {
          return pattern<Len + Len2>{str + val};
      }

      template<size_t BufLen>
      [[nodiscard]] constexpr auto add(const char (&buf)[BufLen]) const
      {
          return pattern<Len + BufLen-1>{str + buf};
      }

   public:
      constexpr auto begin() const
      {
          return str.begin();
      }

      constexpr auto end() const
      {
          return str.end();
      }

      constexpr size_t size() const
      {
          return str.size();
      }

      constexpr bool empty() const
      {
          return str.empty();
      }

      constexpr decltype(auto) operator[](size_t idx) const
      {
          return str[idx];
      }

      template<size_t BufLen>
      [[nodiscard]] constexpr auto start_of_line() const
      {
          return pattern<Len+1>{"^" + str};
      }

      template<size_t BufLen>
      [[nodiscard]] constexpr auto end_of_line() const
      {
          return add("$");
      }

      [[nodiscard]] constexpr auto one_or_more() const
      {
          return add("+");
      }

      friend inline std::ostream& operator<<(std::ostream& os, const pattern& p)
      {
          os << std::string_view{p.str};
          return os;
      }
   };

   template<size_t StrLen>
   pattern(const static_string<StrLen>& str) -> pattern<StrLen>;

   template<size_t Len>
   [[nodiscard]] constexpr auto sanitize(pattern<Len> p)
   {
       return p.str;
   }

   [[nodiscard]] constexpr auto sanitize(char c)
   {
       static_string<2> res;
       if (needsEscaping(c))
          res += '\\';
       res += c;
       return res;
   }

   struct range
   {
       char start;
       char end;

       constexpr range(char start, char end)
        : start(start), end(end)
       {}
   };

   template<typename T>
   struct is_pattern : std::false_type
   {};

   template<size_t Len>
   struct is_pattern<pattern<Len>> : std::true_type
   {};

   [[nodiscard]] constexpr auto sanitize(range r)
   {
       return sanitize(r.start) + "-" + sanitize(r.end);
   }

   template<size_t Len1, size_t Len2>
   constexpr auto operator+(const pattern<Len1>& p1, const pattern<Len2>& p2)
   {
      return pattern{p1.str + p2.str};
   }

   template<size_t Len1, size_t Len2>
   constexpr auto operator||(const pattern<Len1>& p1, const pattern<Len2>& p2)
   {
      return pattern{"(?:" + p1.str + ")|(?:" + p2.str+ ")"};
   }

   template<size_t BufLen1, size_t BufLen2>
   constexpr auto sanitized_str_or_pattern(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2])
   {
       return [prefix, suffix](auto&& buf){
          static_assert(!std::is_same_v<range, std::decay_t<decltype(buf)>>, "Only string literals allowed!");
          return pattern{static_string{prefix + sanitize(buf) + suffix}};
       };
   }

   template<size_t Len1, size_t Len2>
   struct sanitized_str_or_range
   {
       static_string<Len1> prefix;
       static_string<Len2> suffix;
       
       template<size_t BufLen1, size_t BufLen2>
       constexpr sanitized_str_or_range(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2])
        : prefix{prefix}, suffix{suffix}
       {
       }

       template<typename T>
       static constexpr auto to_str(T&& buf)
       {
          static_assert(!is_pattern<std::decay_t<decltype(buf)>>::value, "Only string literals allowed!");
          return sanitize(buf);
       }

       template<typename T, typename... Args>
       static constexpr auto to_str(T&& buf, Args&&... args)
       {
          return to_str(buf) + to_str(args...);
       }

       template<typename... T>
       constexpr auto operator()(T&&... t) const
       {
          return pattern{static_string{prefix + to_str(t...) + suffix}};
       }
   };

   template<size_t BufLen1, size_t BufLen2>
   sanitized_str_or_range(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2]) -> sanitized_str_or_range<BufLen1-1, BufLen2-1>;

   template<size_t BufLen1, size_t BufLen2>
   constexpr auto sanitized_str(const char (&prefix)[BufLen1], const char (&suffix)[BufLen2])
   {
       return [prefix, suffix](auto&& buf){
          static_assert(!std::is_same_v<range, std::decay_t<decltype(buf)>>, "Only string literals allowed!");
          static_assert(!is_pattern<std::decay_t<decltype(buf)>>::value, "Only string literals allowed!");
          return pattern{static_string{prefix + sanitize(buf) + suffix}};
       };
   }

   static inline constexpr auto find          = sanitized_str("(?:", ")");
   static inline constexpr auto then          = find;
   static inline constexpr auto maybe         = sanitized_str_or_pattern("(?:", ")?");
   static inline constexpr auto not_          = sanitized_str_or_pattern("(?!", ")");
   static inline constexpr auto any_of        = sanitized_str_or_range("[", "]");
   static inline constexpr auto any           = any_of;
   static inline constexpr auto something_but = sanitized_str_or_range("(?:[^", "]+)");
   static inline constexpr auto anything_but  = sanitized_str_or_range("(?:[^", "]*)");
   static inline constexpr auto capture       = sanitized_str_or_pattern("(", ")");

   template<size_t Len>
   struct character_type : pattern<Len>
   {
      template<size_t BufLen>
      constexpr explicit character_type(const char (&buf)[BufLen])
        : pattern<Len>{static_string{buf}}
       {
       }
   };

   template<size_t BufLen>
   character_type(const char (&buf)[BufLen]) -> character_type<BufLen-1>;

   static inline constexpr auto any_char       = character_type{"."};
   static inline constexpr auto whitespace     = character_type{"\\s"};
   static inline constexpr auto space          = whitespace;
   static inline constexpr auto non_space      = character_type{"\\S"};
   static inline constexpr auto word_char      = character_type{"\\w"};
   static inline constexpr auto non_word_char  = character_type{"\\W"};
   static inline constexpr auto word_boundary  = character_type{"\\b"};
   static inline constexpr auto digit          = character_type{"\\d"};
   static inline constexpr auto non_digit      = character_type{"\\D"};
   static inline constexpr auto tab            = character_type{"\\t"};

   static inline constexpr auto word          = pattern{static_string{"\\w+"}};
   static inline constexpr auto line_break    = pattern{static_string{"(?:\\r\\n|\\r|\\n)"}};
   static inline constexpr auto br            = line_break;
   static inline constexpr auto something     = pattern{static_string{"(?:.+)"}};
   static inline constexpr auto anything      = pattern{static_string{"(?:.*)"}};
}
