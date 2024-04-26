#include "../sources/String.hpp"

#include "tool.hpp"

using namespace pyincpp;

TEST_CASE("String")
{
    SECTION("basics")
    {
        // String()
        String str1;
        REQUIRE(str1.size() == 0);
        REQUIRE(str1.is_empty());

        // String(const char* chars)
        String str2("hello");
        REQUIRE(str2.size() == 5);
        REQUIRE(!str2.is_empty());

        // String(const String& that)
        String str3(str2);
        REQUIRE(str3.size() == 5);
        REQUIRE(!str3.is_empty());

        // String(String&& that)
        String str4(std::move(str3));
        REQUIRE(str4.size() == 5);
        REQUIRE(!str4.is_empty());
        REQUIRE(str3.size() == 0);
        REQUIRE(str3.is_empty());

        // ~String()
    }

    String empty;
    String one("1");
    String some("12345");

    SECTION("compare")
    {
        // operator==
        String eq_string("12345");
        REQUIRE(eq_string == some);

        // operator!=
        String ne_string("135");
        REQUIRE(ne_string != some);

        // operator<
        String lt_string("123");
        String lt_string2("09999");
        REQUIRE(lt_string < some);
        REQUIRE(lt_string2 < some);

        // operator<=
        REQUIRE(lt_string <= some);
        REQUIRE(lt_string2 <= some);
        REQUIRE(eq_string <= some);

        // operator>
        String gt_string("123456");
        String gt_string2("2");
        REQUIRE(gt_string > some);
        REQUIRE(gt_string2 > some);

        // operator>=
        REQUIRE(gt_string >= some);
        REQUIRE(gt_string2 >= some);
        REQUIRE(eq_string >= some);
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
        String s1("");
        String s2("a");
        String s3("g");
        String s4("cde");
        String s5("abcdefg");

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
        REQUIRE(String("233.33").to_decimal() == Approx(233.33));
        REQUIRE(String("123.456e-3").to_decimal() == Approx(0.123456));
        REQUIRE(String("1e+600").to_decimal() == Approx(HUGE_VAL));
        REQUIRE(std::isnan(String("nan").to_decimal()));
        REQUIRE(String("inf").to_decimal() == Approx(INFINITY));

        // 0
        REQUIRE(String("0").to_decimal() == Approx(0));
        REQUIRE(String("-0").to_decimal() == Approx(0));
        REQUIRE(String("+0").to_decimal() == Approx(0));
        REQUIRE(String(".0").to_decimal() == Approx(0));
        REQUIRE(String("0.").to_decimal() == Approx(0));

        // 1
        REQUIRE(String("1").to_decimal() == Approx(1.0));
        REQUIRE(String("-1").to_decimal() == Approx(-1.0));
        REQUIRE(String("+1").to_decimal() == Approx(1.0));
        REQUIRE(String(".1").to_decimal() == Approx(0.1));
        REQUIRE(String("1.").to_decimal() == Approx(1.0));

        // e
        REQUIRE(String("1e2").to_decimal() == Approx(1e2));
        REQUIRE(String("-1e2").to_decimal() == Approx(-1e2));
        REQUIRE(String("+1e2").to_decimal() == Approx(1e2));
        REQUIRE(String(".1e2").to_decimal() == Approx(0.1e2));
        REQUIRE(String("1.e2").to_decimal() == Approx(1.e2));

        // e+
        REQUIRE(String("1e+2").to_decimal() == Approx(1e+2));
        REQUIRE(String("-1e+2").to_decimal() == Approx(-1e+2));
        REQUIRE(String("+1e+2").to_decimal() == Approx(1e+2));
        REQUIRE(String(".1e+2").to_decimal() == Approx(0.1e+2));
        REQUIRE(String("1.e+2").to_decimal() == Approx(1.e+2));

        // e-
        REQUIRE(String("1e-2").to_decimal() == Approx(1e-2));
        REQUIRE(String("-1e-2").to_decimal() == Approx(-1e-2));
        REQUIRE(String("+1e-2").to_decimal() == Approx(1e-2));
        REQUIRE(String(".1e-2").to_decimal() == Approx(0.1e-2));
        REQUIRE(String("1.e-2").to_decimal() == Approx(1.e-2));

        // error
        REQUIRE_THROWS_MATCHES(String("hello").to_decimal(), std::runtime_error, Message("Error: Invalid literal for to_decimal()."));
    }

    SECTION("to_integer")
    {
        // example
        REQUIRE(String("233").to_integer() == 233);
        REQUIRE(String("cafebabe").to_integer(16) == Integer("3405691582"));
        REQUIRE(String("z").to_integer(36) == 35);

        // other
        REQUIRE(String("0001000").to_integer() == 1000);
        REQUIRE(String("1").to_integer() == 1);
        REQUIRE(String("0").to_integer() == 0);
        REQUIRE(String("f").to_integer(16) == 15);
        REQUIRE(String("11").to_integer(2) == 3);
        REQUIRE(String("zz").to_integer(36) == 35 * 36 + 35);
        REQUIRE(String("-1").to_integer() == -1);
        REQUIRE(String("-0").to_integer() == 0);
        REQUIRE(String("-10").to_integer() == -10);
        REQUIRE(String("-10").to_integer(16) == -16);
        REQUIRE(String("-z").to_integer(36) == -35);
        REQUIRE(String("+1").to_integer() == 1);
        REQUIRE(String("+0").to_integer() == 0);
        REQUIRE(String("+10").to_integer() == 10);
        REQUIRE(String("+10").to_integer(16) == 16);
        REQUIRE(String("+z").to_integer(36) == 35);
        REQUIRE(String("-0101").to_integer(2) == -5);
        REQUIRE(String("-1010").to_integer(2) == -10);
        REQUIRE(String("+0101").to_integer(2) == 5);
        REQUIRE(String("+1010").to_integer(2) == 10);
        REQUIRE(String("\n\r\n\t  233  \t\r\n\r").to_integer() == 233);

        // error
        REQUIRE_THROWS_MATCHES(String("123").to_integer(99), std::runtime_error, Message("Error: Invalid base for to_integer()."));
        REQUIRE_THROWS_MATCHES(String("!!!").to_integer(), std::runtime_error, Message("Error: Invalid literal for to_integer()."));
    }

    SECTION("reverse")
    {
        REQUIRE(empty.reverse() == empty);
        REQUIRE(one.reverse() == one);
        REQUIRE(some.reverse() == "54321");
    }

    SECTION("lower_upper")
    {
        REQUIRE(String("hahaha").upper() == "HAHAHA");

        REQUIRE(String("HAHAHA").lower() == "hahaha");
    }

    SECTION("erase")
    {
        REQUIRE(String("abcdefg").erase(0, 1) == "bcdefg");
        REQUIRE(String("abcdefg").erase(1, 2) == "acdefg");
        REQUIRE(String("abcdefg").erase(1, 6) == "ag");
        REQUIRE(String("abcdefg").erase(0, 7) == "");

        REQUIRE_THROWS_MATCHES(String("abcdefg").erase(-1, 99), std::runtime_error, Message("Error: Index out of range."));
    }

    SECTION("replace")
    {
        REQUIRE(String("abcdefg").replace("a", "g") == "gbcdefg");
        REQUIRE(String("abcdefg").replace("g", "a") == "abcdefa");
        REQUIRE(String("abcdefg").replace("cde", "~~~") == "ab~~~fg");
        REQUIRE(String("abcdefg").replace("abcdefg", "") == "");
        REQUIRE(String("abcdefg").replace("abcdefg", "") == "");
        REQUIRE(String("").replace("abc", "~~~") == "");
        REQUIRE(String("hahaha").replace("h", "l") == "lalala");
        REQUIRE(String("hahaha").replace("a", "ooow~ ") == "hooow~ hooow~ hooow~ ");
        REQUIRE(String("hooow~ hooow~ hooow~ ").replace("ooo", "o") == "how~ how~ how~ ");
    }

    SECTION("strip")
    {
        REQUIRE(String("hello").strip() == "hello");
        REQUIRE(String("\t\nhello\t\n").strip() == "hello");
        REQUIRE(String("           hello           ").strip() == "hello");
        REQUIRE(String("\n\n\n\n \t\n\b\n   hello  \n\n\t\n \r\b\n\r").strip() == "hello");
        REQUIRE(String("'''hello'''").strip('\'') == "hello");
    }

    SECTION("shift")
    {
        REQUIRE((String("ABCDEFGHIJK") >> -1) == "BCDEFGHIJKA");
        REQUIRE((String("ABCDEFGHIJK") >> 0) == "ABCDEFGHIJK");
        REQUIRE((String("ABCDEFGHIJK") >> 1) == "KABCDEFGHIJ");
        REQUIRE((String("ABCDEFGHIJK") >> 3) == "IJKABCDEFGH");
        REQUIRE((String("ABCDEFGHIJK") >> 11) == "ABCDEFGHIJK");

        REQUIRE((String("ABCDEFGHIJK") << -1) == "KABCDEFGHIJ");
        REQUIRE((String("ABCDEFGHIJK") << 0) == "ABCDEFGHIJK");
        REQUIRE((String("ABCDEFGHIJK") << 1) == "BCDEFGHIJKA");
        REQUIRE((String("ABCDEFGHIJK") << 3) == "DEFGHIJKABC");
        REQUIRE((String("ABCDEFGHIJK") << 11) == "ABCDEFGHIJK");
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
        REQUIRE(String("one, two, three").split(", ") == List<String>({"one", "two", "three"}));
        REQUIRE(String("this is my code!").split(" ") == List<String>({"this", "is", "my", "code!"}));
        REQUIRE(String("this is my code!").split() == List<String>({"this", "is", "my", "code!"}));
        REQUIRE(String("this is my code!").split("this is my code!") == List<String>({""}));
        REQUIRE(String(" this is my code! ").split(" ") == List<String>({"", "this", "is", "my", "code!"}));
        REQUIRE(String("aaa").split("a") == List<String>({"", "", ""}));
        REQUIRE(String("192.168.0.1").split(".") == List<String>({"192", "168", "0", "1"}));
    }

    SECTION("join")
    {
        REQUIRE(String(", ").join({}) == "");
        REQUIRE(String(", ").join({"a"}) == "a");
        REQUIRE(String(", ").join({"a", "b"}) == "a, b");
        REQUIRE(String(", ").join({"a", "b", "c"}) == "a, b, c");
        REQUIRE(String(".").join({"192", "168", "0", "1"}) == "192.168.0.1");
    }

    SECTION("format")
    {
        REQUIRE(String("{}, {}, {}, {}.").format(1, 2, 3, 4) == String("1, 2, 3, 4."));
        REQUIRE(String("I'm {}, {} years old.").format("Alice", 18) == String("I'm Alice, 18 years old."));
        REQUIRE(String("{} -> {}").format(List<int>({1, 2, 3}), List<String>({"one", "two", "three"})) == String("[1, 2, 3] -> [\"one\", \"two\", \"three\"]"));
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
        String line1, line2, line3;
        std::istringstream("this is line1\nthis is line2\nhello!") >> line1 >> line2 >> line3;

        REQUIRE(line1 == "this is line1");
        REQUIRE(line2 == "this is line2");
        REQUIRE(line3 == "hello!");
    }
}
