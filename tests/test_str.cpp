#include "../sources/str.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("Str")
{
    SECTION("basics")
    {
        // Str()
        Str str1;
        REQUIRE(str1.size() == 0);
        REQUIRE(str1.is_empty());

        // Str(const char* chars)
        Str str2("hello");
        REQUIRE(str2.size() == 5);
        REQUIRE(!str2.is_empty());

        // Str(const std::string& string)
        Str str3(std::string{"hello"});
        REQUIRE(str3.size() == 5);
        REQUIRE(!str3.is_empty());

        // Str(const Str& that)
        Str str4(str3);
        REQUIRE(str4.size() == 5);
        REQUIRE(!str4.is_empty());

        // Str(Str&& that)
        Str str5(std::move(str4));
        REQUIRE(str5.size() == 5);
        REQUIRE(!str5.is_empty());
        REQUIRE(str4.size() == 0);
        REQUIRE(str4.is_empty());

        // ~Str()
    }

    Str empty;
    Str one("1");
    Str some("12345");

    SECTION("compare")
    {
        // operator==
        Str eq_str("12345");
        REQUIRE(eq_str == some);

        // operator!=
        Str ne_str("135");
        REQUIRE(ne_str != some);

        // operator<
        Str lt_str("123");
        Str lt_str2("09999");
        REQUIRE(lt_str < some);
        REQUIRE(lt_str2 < some);

        // operator<=
        REQUIRE(lt_str <= some);
        REQUIRE(lt_str2 <= some);
        REQUIRE(eq_str <= some);

        // operator>
        Str gt_str("123456");
        Str gt_str2("2");
        REQUIRE(gt_str > some);
        REQUIRE(gt_str2 > some);

        // operator>=
        REQUIRE(gt_str >= some);
        REQUIRE(gt_str2 >= some);
        REQUIRE(eq_str >= some);
    }

    SECTION("assignment")
    {
        some = one; // copy
        REQUIRE(some == "1");
        REQUIRE(one == "1");

        empty = std::move(one); // move
        REQUIRE(empty == "1");
        REQUIRE(one == "");
    }

    SECTION("iterator")
    {
        // empty
        REQUIRE(empty.begin() == empty.end());
        REQUIRE(empty.rbegin() == empty.rend());

        // for in
        for (char i = '1'; const auto& e : some)
        {
            REQUIRE(e == i++);
        }

        // reversed for
        char i = '5';
        for (auto it = some.rbegin(); it != some.rend(); ++it)
        {
            REQUIRE(*it == i--);
        }
    }

    SECTION("access")
    {
        // forward
        for (int i = 0; i < some.size(); ++i)
        {
            REQUIRE(some[i] == i + '1');
        }

        // backward
        for (int i = -1; i >= -some.size(); --i)
        {
            REQUIRE(some[i] == i + '6');
        }

        // check bounds
        REQUIRE_THROWS_MATCHES(some[5], std::runtime_error, Message("Error: Index out of range."));
    }

    SECTION("find")
    {
        Str s1("");
        Str s2("a");
        Str s3("g");
        Str s4("cde");
        Str s5("abcdefg");

        REQUIRE(s1.find(s1) == 0);
        REQUIRE(s5.find(s1) == 0);
        REQUIRE(s5.find(s2) == 0);
        REQUIRE(s5.find(s3) == 6);
        REQUIRE(s5.find(s4) == 2);
        REQUIRE(s5.find(s5) == 0);

        REQUIRE(s1.find(s1, 3, 99) == -1);
        REQUIRE(s5.find(s1, 3, 99) == 3);
        REQUIRE(s5.find(s2, 3, 99) == -1);
        REQUIRE(s5.find(s3, 3, 99) == 6);
        REQUIRE(s5.find(s4, 3, 99) == -1);
        REQUIRE(s5.find(s5, 3, 99) == -1);
    }

    SECTION("examination")
    {
        // contains
        REQUIRE(some.contains("1") == true);
        REQUIRE(some.contains("5") == true);
        REQUIRE(some.contains("0") == false);
        REQUIRE(some.contains("1", 1, 99) == false);
        REQUIRE(some.contains("5", 1, 99) == true);
        REQUIRE(some.contains("0", 1, 99) == false);

        // count
        REQUIRE(some.count('0') == 0);
        REQUIRE(some.count('1') == 1);

        // starts_with
        REQUIRE(some.starts_with("1"));
        REQUIRE(some.starts_with("12345"));
        REQUIRE(!some.starts_with("2"));
        REQUIRE(!some.starts_with("123456"));

        // ends_with
        REQUIRE(some.ends_with("5"));
        REQUIRE(some.ends_with("12345"));
        REQUIRE(!some.ends_with("4"));
        REQUIRE(!some.ends_with("123456"));
    }

    SECTION("to_decimal")
    {
        // example
        REQUIRE(Str("233.33").to_decimal() == Approx(233.33));
        REQUIRE(Str("123.456e-3").to_decimal() == Approx(0.123456));
        REQUIRE(Str("1e+600").to_decimal() == Approx(HUGE_VAL));
        REQUIRE(std::isnan(Str("nan").to_decimal()));
        REQUIRE(Str("inf").to_decimal() == Approx(INFINITY));

        // 0
        REQUIRE(Str("0").to_decimal() == Approx(0));
        REQUIRE(Str("-0").to_decimal() == Approx(0));
        REQUIRE(Str("+0").to_decimal() == Approx(0));
        REQUIRE(Str(".0").to_decimal() == Approx(0));
        REQUIRE(Str("0.").to_decimal() == Approx(0));

        // 1
        REQUIRE(Str("1").to_decimal() == Approx(1.0));
        REQUIRE(Str("-1").to_decimal() == Approx(-1.0));
        REQUIRE(Str("+1").to_decimal() == Approx(1.0));
        REQUIRE(Str(".1").to_decimal() == Approx(0.1));
        REQUIRE(Str("1.").to_decimal() == Approx(1.0));

        // e
        REQUIRE(Str("1e2").to_decimal() == Approx(1e2));
        REQUIRE(Str("-1e2").to_decimal() == Approx(-1e2));
        REQUIRE(Str("+1e2").to_decimal() == Approx(1e2));
        REQUIRE(Str(".1e2").to_decimal() == Approx(0.1e2));
        REQUIRE(Str("1.e2").to_decimal() == Approx(1.e2));

        // e+
        REQUIRE(Str("1e+2").to_decimal() == Approx(1e+2));
        REQUIRE(Str("-1e+2").to_decimal() == Approx(-1e+2));
        REQUIRE(Str("+1e+2").to_decimal() == Approx(1e+2));
        REQUIRE(Str(".1e+2").to_decimal() == Approx(0.1e+2));
        REQUIRE(Str("1.e+2").to_decimal() == Approx(1.e+2));

        // e-
        REQUIRE(Str("1e-2").to_decimal() == Approx(1e-2));
        REQUIRE(Str("-1e-2").to_decimal() == Approx(-1e-2));
        REQUIRE(Str("+1e-2").to_decimal() == Approx(1e-2));
        REQUIRE(Str(".1e-2").to_decimal() == Approx(0.1e-2));
        REQUIRE(Str("1.e-2").to_decimal() == Approx(1.e-2));

        // error
        REQUIRE_THROWS_MATCHES(Str("hello").to_decimal(), std::runtime_error, Message("Error: Invalid literal for to_decimal()."));
    }

    SECTION("to_integer")
    {
        // example
        REQUIRE(Str("233").to_integer() == 233);
        REQUIRE(Str("cafebabe").to_integer(16) == Int("3405691582"));
        REQUIRE(Str("z").to_integer(36) == 35);
        REQUIRE(Str("ffffffffffffffff").to_integer(16) == Int("18446744073709551615"));

        // other
        REQUIRE(Str("0001000").to_integer() == 1000);
        REQUIRE(Str("1").to_integer() == 1);
        REQUIRE(Str("0").to_integer() == 0);
        REQUIRE(Str("f").to_integer(16) == 15);
        REQUIRE(Str("11").to_integer(2) == 3);
        REQUIRE(Str("zz").to_integer(36) == 35 * 36 + 35);
        REQUIRE(Str("-1").to_integer() == -1);
        REQUIRE(Str("-0").to_integer() == 0);
        REQUIRE(Str("-10").to_integer() == -10);
        REQUIRE(Str("-10").to_integer(16) == -16);
        REQUIRE(Str("-z").to_integer(36) == -35);
        REQUIRE(Str("+1").to_integer() == 1);
        REQUIRE(Str("+0").to_integer() == 0);
        REQUIRE(Str("+10").to_integer() == 10);
        REQUIRE(Str("+10").to_integer(16) == 16);
        REQUIRE(Str("+z").to_integer(36) == 35);
        REQUIRE(Str("-0101").to_integer(2) == -5);
        REQUIRE(Str("-1010").to_integer(2) == -10);
        REQUIRE(Str("+0101").to_integer(2) == 5);
        REQUIRE(Str("+1010").to_integer(2) == 10);
        REQUIRE(Str("\n\r\n\t  233  \t\r\n\r").to_integer() == 233);

        // error
        REQUIRE_THROWS_MATCHES(Str("123").to_integer(99), std::runtime_error, Message("Error: Invalid base for to_integer()."));
        REQUIRE_THROWS_MATCHES(Str("!!!").to_integer(), std::runtime_error, Message("Error: Invalid literal for to_integer()."));
    }

    SECTION("reverse")
    {
        REQUIRE(empty.reverse() == empty);
        REQUIRE(one.reverse() == one);
        REQUIRE(some.reverse() == "54321");
    }

    SECTION("lower_upper")
    {
        REQUIRE(Str("hahaha").upper() == "HAHAHA");

        REQUIRE(Str("HAHAHA").lower() == "hahaha");
    }

    SECTION("erase")
    {
        REQUIRE(Str("abcdefg").erase(0, 1) == "bcdefg");
        REQUIRE(Str("abcdefg").erase(1, 2) == "acdefg");
        REQUIRE(Str("abcdefg").erase(1, 6) == "ag");
        REQUIRE(Str("abcdefg").erase(0, 7) == "");

        REQUIRE_THROWS_MATCHES(Str("abcdefg").erase(-1, 99), std::runtime_error, Message("Error: Index out of range."));
    }

    SECTION("replace")
    {
        REQUIRE(Str("abcdefg").replace("a", "g") == "gbcdefg");
        REQUIRE(Str("abcdefg").replace("g", "a") == "abcdefa");
        REQUIRE(Str("abcdefg").replace("cde", "~~~") == "ab~~~fg");
        REQUIRE(Str("abcdefg").replace("abcdefg", "") == "");
        REQUIRE(Str("abcdefg").replace("abcdefg", "") == "");
        REQUIRE(Str("").replace("abc", "~~~") == "");
        REQUIRE(Str("hahaha").replace("h", "l") == "lalala");
        REQUIRE(Str("hahaha").replace("a", "ooow~ ") == "hooow~ hooow~ hooow~ ");
        REQUIRE(Str("hooow~ hooow~ hooow~ ").replace("ooo", "o") == "how~ how~ how~ ");
    }

    SECTION("strip")
    {
        REQUIRE(Str("hello").strip() == "hello");
        REQUIRE(Str("\t\nhello\t\n").strip() == "hello");
        REQUIRE(Str("           hello           ").strip() == "hello");
        REQUIRE(Str("\n\n\n\n \t\n\b\n   hello  \n\n\t\n \r\b\n\r").strip() == "hello");
        REQUIRE(Str("'''hello'''").strip('\'') == "hello");
    }

    SECTION("rotate")
    {
        REQUIRE((empty >> 1) == "");
        REQUIRE((empty >> 2) == "");
        REQUIRE((empty << 1) == "");
        REQUIRE((empty << 2) == "");

        REQUIRE((one >> 1) == "1");
        REQUIRE((one >> 2) == "1");
        REQUIRE((one << 1) == "1");
        REQUIRE((one << 2) == "1");

        REQUIRE((Str("ABCDEFGHIJK") >> -1) == "BCDEFGHIJKA");
        REQUIRE((Str("ABCDEFGHIJK") >> 0) == "ABCDEFGHIJK");
        REQUIRE((Str("ABCDEFGHIJK") >> 1) == "KABCDEFGHIJ");
        REQUIRE((Str("ABCDEFGHIJK") >> 3) == "IJKABCDEFGH");
        REQUIRE((Str("ABCDEFGHIJK") >> 11) == "ABCDEFGHIJK");

        REQUIRE((Str("ABCDEFGHIJK") << -1) == "KABCDEFGHIJ");
        REQUIRE((Str("ABCDEFGHIJK") << 0) == "ABCDEFGHIJK");
        REQUIRE((Str("ABCDEFGHIJK") << 1) == "BCDEFGHIJKA");
        REQUIRE((Str("ABCDEFGHIJK") << 3) == "DEFGHIJKABC");
        REQUIRE((Str("ABCDEFGHIJK") << 11) == "ABCDEFGHIJK");
    }

    SECTION("slice")
    {
        REQUIRE(some.slice(-1, 1) == "");
        REQUIRE(some.slice(-1, 1, -1) == "543");
        REQUIRE(some.slice(1, -1) == "234");
        REQUIRE(some.slice(1, -1, -1) == "");

        REQUIRE(some.slice(0, 5) == "12345");
        REQUIRE(some.slice(0, 5, 2) == "135");
        REQUIRE(some.slice(0, 5, -1) == "");
        REQUIRE(some.slice(0, 5, -2) == "");

        REQUIRE(some.slice(-1, -6) == "");
        REQUIRE(some.slice(-1, -6, 2) == "");
        REQUIRE(some.slice(-1, -6, -1) == "54321");
        REQUIRE(some.slice(-1, -6, -2) == "531");

        REQUIRE(some.slice(0, 0) == "");
        REQUIRE(some.slice(1, 1) == "");
        REQUIRE(some.slice(-1, -1) == "");
        REQUIRE(some.slice(-1, -1, -1) == "");

        REQUIRE_THROWS_MATCHES(some.slice(1, 2, 0), std::runtime_error, Message("Error: Slice step can not be zero."));

        REQUIRE_THROWS_MATCHES(some.slice(-7, -6), std::runtime_error, Message("Error: Index out of range."));
    }

    SECTION("append")
    {
        REQUIRE(some + '6' == "123456");
        REQUIRE(some + "67" == "1234567");
    }

    SECTION("times")
    {
        REQUIRE(some * 0 == "");
        REQUIRE(some * 1 == "12345");
        REQUIRE(some * 2 == "1234512345");
    }

    SECTION("split")
    {
        REQUIRE(Str("one, two, three").split(", ") == List<Str>{"one", "two", "three"});
        REQUIRE(Str("192.168.0.1").split(".") == List<Str>{"192", "168", "0", "1"});
        REQUIRE(Str("   1   2   3   ").split() == List<Str>{"1", "2", "3"});
        REQUIRE(Str("this is my code!").split() == List<Str>{"this", "is", "my", "code!"});

        REQUIRE(Str("this is my code!").split("this is my code!") == List<Str>{});
        REQUIRE(Str("this is my code!").split("!") == List<Str>{"this is my code"});
        REQUIRE(Str("aaa").split("a") == List<Str>{});
        REQUIRE(Str(" ").split(" ") == List<Str>{});

        REQUIRE(Str("this is my code!").split("this is my code!", true) == List<Str>{"", ""});
        REQUIRE(Str("this is my code!").split("!", true) == List<Str>{"this is my code", ""});
        REQUIRE(Str("aaa").split("a", true) == List<Str>{"", "", "", ""});
        REQUIRE(Str(" ").split(" ", true) == List<Str>{"", ""});
    }

    SECTION("join")
    {
        REQUIRE(Str(", ").join({}) == "");
        REQUIRE(Str(", ").join({"a"}) == "a");
        REQUIRE(Str(", ").join({"a", "b"}) == "a, b");
        REQUIRE(Str(", ").join({"a", "b", "c"}) == "a, b, c");
        REQUIRE(Str(".").join({"192", "168", "0", "1"}) == "192.168.0.1");
    }

    SECTION("format")
    {
        REQUIRE(Str("{}, {}, {}, {}.").format(1, 2, 3, 4) == "1, 2, 3, 4.");
        REQUIRE(Str("I'm {}, {} years old.").format("Alice", 18) == "I'm Alice, 18 years old.");
        REQUIRE(Str("{} -> {}").format(List<int>{1, 2, 3}, List<Str>{"one", "two", "three"}) == "[1, 2, 3] -> [\"one\", \"two\", \"three\"]");
    }

    SECTION("print")
    {
        std::ostringstream oss;

        oss << empty;
        REQUIRE(oss.str() == "\"\"");
        oss.str("");

        oss << one;
        REQUIRE(oss.str() == "\"1\"");
        oss.str("");

        oss << some;
        REQUIRE(oss.str() == "\"12345\"");
        oss.str("");
    }

    SECTION("input")
    {
        Str line1, line2, line3;
        std::istringstream("this is line1\nthis is line2\nhello!") >> line1 >> line2 >> line3;

        REQUIRE(line1 == "this is line1");
        REQUIRE(line2 == "this is line2");
        REQUIRE(line3 == "hello!");
    }
}
