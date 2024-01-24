#include "../sources/String.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor size() is_empty()
TEST_CASE("String: basics")
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

// operator==() operator!=() operator<() operator<=() operator>() operator>=()
TEST_CASE("String: compare")
{
    String string("12345");

    // operator==
    String eq_string("12345");
    REQUIRE(eq_string == string);

    // operator!=
    String ne_string("135");
    REQUIRE(ne_string != string);

    // operator<
    String lt_string("123");
    String lt_string2("09999");
    REQUIRE(lt_string < string);
    REQUIRE(lt_string2 < string);

    // operator<=
    REQUIRE(lt_string <= string);
    REQUIRE(lt_string2 <= string);
    REQUIRE(eq_string <= string);

    // operator>
    String gt_string("123456");
    String gt_string2("2");
    REQUIRE(gt_string > string);
    REQUIRE(gt_string2 > string);

    // operator>=
    REQUIRE(gt_string >= string);
    REQUIRE(gt_string2 >= string);
    REQUIRE(eq_string >= string);
}

// operator=()
TEST_CASE("String: copy_assignment")
{
    String string1("12345");
    String string2("6789");

    string1 = string2;
    REQUIRE(string1 == "6789");
    REQUIRE(string2 == "6789");
}

// operator=()
TEST_CASE("String: move_assignment")
{
    String string1("12345");
    String string2("6789");

    string1 = std::move(string2);
    REQUIRE(string1 == "6789");
    REQUIRE(string2 == "");
}

// operator[]()
TEST_CASE("String: access")
{
    String string("12345");

    // forward
    for (int i = 0; i < string.size(); ++i)
    {
        REQUIRE(string[i] == i + '1');
    }

    // backward
    for (int i = -1; i >= -string.size(); --i)
    {
        REQUIRE(string[i] == i + '6');
    }

    // assignment
    string[0] = '0';
    REQUIRE(string[0] == '0');

    string[-1] = 'Z';
    REQUIRE(string[-1] == 'Z');

    // check bounds
    MY_ASSERT_THROW_MESSAGE(string[5], std::runtime_error, "Error: Index out of range.");
}

// get() set()
TEST_CASE("String: get_set")
{
    String string;
    char* str = String("hello").get();
    string.set(str);
    REQUIRE(string == "hello");
    delete[] str;
}

// find()
TEST_CASE("String: find")
{
    const String s1("");
    const String s2("a");
    const String s3("g");
    const String s4("cde");
    const String s5("abcdefg");

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

// contains() min() max() count() begin_with() end_with()
TEST_CASE("String: examination")
{
    String string("12345");

    // contains
    REQUIRE(string.contains("1") == true);
    REQUIRE(string.contains("5") == true);
    REQUIRE(string.contains("0") == false);
    REQUIRE(string.contains("1", 1, 99) == false);
    REQUIRE(string.contains("5", 1, 99) == true);
    REQUIRE(string.contains("0", 1, 99) == false);

    // min
    REQUIRE(string.min() == '1');

    // max
    REQUIRE(string.max() == '5');

    // count
    REQUIRE(string.count('0') == 0);
    REQUIRE(string.count('1') == 1);

    // begin_with
    REQUIRE(string.begin_with("1"));
    REQUIRE(string.begin_with("12345"));
    REQUIRE(!string.begin_with("2"));
    REQUIRE(!string.begin_with("123456"));

    // end_with
    REQUIRE(string.end_with("5"));
    REQUIRE(string.end_with("12345"));
    REQUIRE(!string.end_with("4"));
    REQUIRE(!string.end_with("123456"));
}

// to_decimal()
TEST_CASE("String: to_decimal")
{
    // example
    REQUIRE(String("233.33").to_decimal() == Catch::Approx(233.33));
    REQUIRE(String("123.456e-3").to_decimal() == Catch::Approx(0.123456));
    REQUIRE(String("1e+600").to_decimal() == Catch::Approx(HUGE_VAL));
    REQUIRE(std::isnan(String("nan").to_decimal()));
    REQUIRE(String("inf").to_decimal() == Catch::Approx(INFINITY));

    // 0
    REQUIRE(String("0").to_decimal() == Catch::Approx(0));
    REQUIRE(String("-0").to_decimal() == Catch::Approx(0));
    REQUIRE(String("+0").to_decimal() == Catch::Approx(0));
    REQUIRE(String(".0").to_decimal() == Catch::Approx(0));
    REQUIRE(String("0.").to_decimal() == Catch::Approx(0));

    // 1
    REQUIRE(String("1").to_decimal() == Catch::Approx(1.0));
    REQUIRE(String("-1").to_decimal() == Catch::Approx(-1.0));
    REQUIRE(String("+1").to_decimal() == Catch::Approx(1.0));
    REQUIRE(String(".1").to_decimal() == Catch::Approx(0.1));
    REQUIRE(String("1.").to_decimal() == Catch::Approx(1.0));

    // e
    REQUIRE(String("1e2").to_decimal() == Catch::Approx(1e2));
    REQUIRE(String("-1e2").to_decimal() == Catch::Approx(-1e2));
    REQUIRE(String("+1e2").to_decimal() == Catch::Approx(1e2));
    REQUIRE(String(".1e2").to_decimal() == Catch::Approx(0.1e2));
    REQUIRE(String("1.e2").to_decimal() == Catch::Approx(1.e2));

    // e+
    REQUIRE(String("1e+2").to_decimal() == Catch::Approx(1e+2));
    REQUIRE(String("-1e+2").to_decimal() == Catch::Approx(-1e+2));
    REQUIRE(String("+1e+2").to_decimal() == Catch::Approx(1e+2));
    REQUIRE(String(".1e+2").to_decimal() == Catch::Approx(0.1e+2));
    REQUIRE(String("1.e+2").to_decimal() == Catch::Approx(1.e+2));

    // e-
    REQUIRE(String("1e-2").to_decimal() == Catch::Approx(1e-2));
    REQUIRE(String("-1e-2").to_decimal() == Catch::Approx(-1e-2));
    REQUIRE(String("+1e-2").to_decimal() == Catch::Approx(1e-2));
    REQUIRE(String(".1e-2").to_decimal() == Catch::Approx(0.1e-2));
    REQUIRE(String("1.e-2").to_decimal() == Catch::Approx(1.e-2));

    // error
    MY_ASSERT_THROW_MESSAGE(String("hello").to_decimal(), std::runtime_error, "Error: Invalid literal for to_decimal().");
}

// to_integer()
TEST_CASE("String: to_integer")
{
    // example
    REQUIRE(String("233").to_integer() == 233);
    REQUIRE(String("cafebabe").to_integer(16) == 3405691582);
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
    MY_ASSERT_THROW_MESSAGE(String("123").to_integer(99), std::runtime_error, "Error: Invalid base for to_integer().");

    MY_ASSERT_THROW_MESSAGE(String("!!!").to_integer(), std::runtime_error, "Error: Invalid literal for to_integer().");
}

// insert()
TEST_CASE("String: insert")
{
    String string;

    // check bounds
    MY_ASSERT_THROW_MESSAGE(string.insert(999, '0'), std::runtime_error, "Error: Index out of range.");

    // insert
    string.insert(0, 'a');
    REQUIRE(string == "a");
    string.insert(0, 'b');
    REQUIRE(string == "ba");
    string.insert(2, 'c');
    REQUIRE(string == "bac");
    string.insert(1, 'd');
    REQUIRE(string == "bdac");
    string.insert(-1, 'z'); // expand capacity
    REQUIRE(string == "bdazc");
}

// remove()
TEST_CASE("String: remove")
{
    String string("bdazc");

    // check bounds
    MY_ASSERT_THROW_MESSAGE(string.remove(999), std::runtime_error, "Error: Index out of range.");

    // remove
    REQUIRE(string.remove(-2) == 'z');
    REQUIRE(string.remove(1) == 'd');
    REQUIRE(string.remove(0) == 'b');
    REQUIRE(string.remove(0) == 'a');
    REQUIRE(string.remove(0) == 'c');

    // check empty
    MY_ASSERT_THROW_MESSAGE(string.remove(0), std::runtime_error, "Error: The container is empty.");
}

// operator+=()
TEST_CASE("String: append")
{
    String string;

    // append element
    REQUIRE((string += '2') == "2");
    REQUIRE((string += '3') == "23");
    REQUIRE((string += '3') == "233");
    REQUIRE((string += '3') == "2333");
    REQUIRE((string += '3') == "23333");

    // append string
    REQUIRE((string += string) == "2333323333");
    REQUIRE((string += string) == "23333233332333323333");
    REQUIRE((string += "00") == "2333323333233332333300");
}

// operator-=()
TEST_CASE("String: remove_element")
{
    String string("12345");

    REQUIRE((string -= '1') == "2345");
    REQUIRE((string -= '2') == "345");
    REQUIRE((string -= '3') == "45");
    REQUIRE((string -= '4') == "5");
    REQUIRE((string -= '5') == "");

    // if the string does not contain the element, it is unchanged.
    REQUIRE((string -= '6') == "");
}

// operator*=()
TEST_CASE("String: repeat")
{
    String string("12");

    MY_ASSERT_THROW_MESSAGE(string *= -1, std::runtime_error, "Error: Times to repeat can not be less than zero.");

    REQUIRE((string *= 1) == "12");
    REQUIRE((string *= 2) == "1212");
    REQUIRE((string *= 0) == "");
}

// clear()
TEST_CASE("String: clear")
{
    String string("12345");

    REQUIRE(string.clear() == "");

    // double clear
    REQUIRE(string.clear() == "");

    // modify after clear
    string += "233";
    REQUIRE(string == "233");
}

// map()
TEST_CASE("String: map")
{
    String string("12345");

    string.map([](char& x)
               { x += 1; });
    REQUIRE(string == "23456");

    string.map([](char& x)
               { x = '1'; });
    REQUIRE(string == "11111");
}

// filter()
TEST_CASE("String: filter")
{
    String string("123456789");

    string.filter([](char& x)
                  { return x % 2 == 0; });
    REQUIRE(string == "2468");

    string.filter([](char& x)
                  { return x % 2 == 1; });
    REQUIRE(string == "");
}

// reverse()
TEST_CASE("String: reverse")
{
    REQUIRE(String().reverse() == "");

    REQUIRE(String("12345").reverse() == "54321");
}

// uniquify()
TEST_CASE("String: uniquify")
{
    REQUIRE(String("122333").uniquify() == "123");

    REQUIRE(String("00000000000000").uniquify() == "0");

    REQUIRE(String("123123123").uniquify() == "123");

    String many;
    for (int i = 0; i < 10000; i++)
    {
        many += 'A';
    }
    REQUIRE(many.uniquify() == "A");
}

// sort()
TEST_CASE("String: sort")
{
    String string("1357986420");

    // from small to large
    string.sort();
    REQUIRE(string == "0123456789");

    // from large to small
    string.sort([](const char& e1, const char& e2)
                { return e1 > e2; });
    REQUIRE(string == "9876543210");
}

// swap()
TEST_CASE("String: swap")
{
    String s1("first");
    String s2("second");

    REQUIRE(s1.swap(s2) == "second");

    REQUIRE(s1 == "second");
    REQUIRE(s2 == "first");
}

// lower() upper()
TEST_CASE("String: lower_upper")
{
    REQUIRE(String("hahaha").upper() == "HAHAHA");

    REQUIRE(String("HAHAHA").lower() == "hahaha");
}

// erase()
TEST_CASE("String: erase")
{
    REQUIRE(String("abcdefg").erase(0, 1) == "bcdefg");
    REQUIRE(String("abcdefg").erase(1, 2) == "acdefg");
    REQUIRE(String("abcdefg").erase(1, 6) == "ag");
    REQUIRE(String("abcdefg").erase(0, 7) == "");

    MY_ASSERT_THROW_MESSAGE(String("abcdefg").erase(-1, 99), std::runtime_error, "Error: Index out of range.");
}

// replace()
TEST_CASE("String: replace")
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

// strip()
TEST_CASE("String: strip")
{
    REQUIRE(String("hello").strip() == "hello");
    REQUIRE(String("\t\nhello\t\n").strip() == "hello");
    REQUIRE(String("           hello           ").strip() == "hello");
    REQUIRE(String("\n\n\n\n \t\n\b\n   hello  \n\n\t\n \r\b\n\r").strip() == "hello");
    REQUIRE(String("'''hello'''").strip('\'') == "hello");
}

// operator>>=() operator<<=()
TEST_CASE("String: shift")
{
    REQUIRE((String("ABCDEFGHIJK") >>= -1) == "BCDEFGHIJKA");
    REQUIRE((String("ABCDEFGHIJK") >>= 0) == "ABCDEFGHIJK");
    REQUIRE((String("ABCDEFGHIJK") >>= 1) == "KABCDEFGHIJ");
    REQUIRE((String("ABCDEFGHIJK") >>= 3) == "IJKABCDEFGH");
    REQUIRE((String("ABCDEFGHIJK") >>= 11) == "ABCDEFGHIJK");

    REQUIRE((String("ABCDEFGHIJK") <<= -1) == "KABCDEFGHIJ");
    REQUIRE((String("ABCDEFGHIJK") <<= 0) == "ABCDEFGHIJK");
    REQUIRE((String("ABCDEFGHIJK") <<= 1) == "BCDEFGHIJKA");
    REQUIRE((String("ABCDEFGHIJK") <<= 3) == "DEFGHIJKABC");
    REQUIRE((String("ABCDEFGHIJK") <<= 11) == "ABCDEFGHIJK");
}

// slice()
TEST_CASE("String: slice")
{
    String string("12345");

    REQUIRE(string.slice(-1, 1) == "");
    REQUIRE(string.slice(-1, 1, -1) == "543");
    REQUIRE(string.slice(1, -1) == "234");
    REQUIRE(string.slice(1, -1, -1) == "");

    REQUIRE(string.slice(0, 5) == "12345");
    REQUIRE(string.slice(0, 5, 2) == "135");
    REQUIRE(string.slice(0, 5, -1) == "");
    REQUIRE(string.slice(0, 5, -2) == "");

    REQUIRE(string.slice(-1, -6) == "");
    REQUIRE(string.slice(-1, -6, 2) == "");
    REQUIRE(string.slice(-1, -6, -1) == "54321");
    REQUIRE(string.slice(-1, -6, -2) == "531");

    REQUIRE(string.slice(0, 0) == "");
    REQUIRE(string.slice(1, 1) == "");
    REQUIRE(string.slice(-1, -1) == "");
    REQUIRE(string.slice(-1, -1, -1) == "");

    MY_ASSERT_THROW_MESSAGE(string.slice(1, 2, 0), std::runtime_error, "Error: Slice step can not be zero.");

    MY_ASSERT_THROW_MESSAGE(string.slice(-7, -6), std::runtime_error, "Error: Index out of range.");
}

// operator+() operator-() operator*()
TEST_CASE("String: production")
{
    String string("12345");

    // operator+
    REQUIRE(string + '6' == "123456");
    REQUIRE(string + "67" == "1234567");
    REQUIRE('0' + string == "012345");
    REQUIRE("000" + string == "00012345");

    // operator-
    REQUIRE(string - '5' == "1234");
    REQUIRE(string - '6' == "12345");

    // operator*
    REQUIRE(string * 0 == "");
    REQUIRE(string * 1 == "12345");
    REQUIRE(string * 2 == "1234512345");
    REQUIRE(0 * string == "");
    REQUIRE(1 * string == "12345");
    REQUIRE(2 * string == "1234512345");
}

// split()
TEST_CASE("String: split")
{
    REQUIRE(String("one, two, three").split(", ") == List<String>({"one", "two", "three"}));
    REQUIRE(String("this is my code!").split(" ") == List<String>({"this", "is", "my", "code!"}));
    REQUIRE(String("this is my code!").split() == List<String>({"this", "is", "my", "code!"}));
    REQUIRE(String("this is my code!").split("this is my code!") == List<String>({""}));
    REQUIRE(String(" this is my code! ").split(" ") == List<String>({"", "this", "is", "my", "code!"}));
    REQUIRE(String("aaa").split("a") == List<String>({"", "", ""}));
    REQUIRE(String("192.168.0.1").split(".") == List<String>({"192", "168", "0", "1"}));
}

// join()
TEST_CASE("String: join")
{
    REQUIRE(String(", ").join({}) == "");
    REQUIRE(String(", ").join({"a"}) == "a");
    REQUIRE(String(", ").join({"a", "b"}) == "a, b");
    REQUIRE(String(", ").join({"a", "b", "c"}) == "a, b, c");
    REQUIRE(String(".").join({"192", "168", "0", "1"}) == "192.168.0.1");
}

// format()
TEST_CASE("String: format")
{
    REQUIRE(String("{}, {}, {}, {}.").format(1, 2, 3, 4) == String("1, 2, 3, 4."));
    REQUIRE(String("I'm {}, {} years old.").format("Alice", 18) == String("I'm Alice, 18 years old."));
    REQUIRE(String("{} -> {}").format(List<int>({1, 2, 3}), List<String>({"one", "two", "three"})) == String("[1, 2, 3] -> [\"one\", \"two\", \"three\"]"));
}

// operator<<()
TEST_CASE("String: print")
{
    std::ostringstream oss;

    String empty;
    oss << empty;
    REQUIRE(oss.str() == "\"\"");
    oss.str("");

    String one("1");
    oss << one;
    REQUIRE(oss.str() == "\"1\"");
    oss.str("");

    String many("hello world");
    oss << many;
    REQUIRE(oss.str() == "\"hello world\"");
    oss.str("");
}

// operator>>()
TEST_CASE("String: input")
{
    String empty;
    std::istringstream("") >> empty;
    REQUIRE(empty == "");

    String one;
    std::istringstream("1") >> one;
    REQUIRE(one == "1");

    String line1, line2, line3;
    std::istringstream("this is line1\nthis is line2\nhello!") >> line1 >> line2 >> line3;
    REQUIRE(line1 == "this is line1");
    REQUIRE(line2 == "this is line2");
    REQUIRE(line3 == "hello!");
}
