// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream

#include "../Sources/String.hpp"

using mdspp::List;
using mdspp::String;

// constructor destructor size() is_empty()
TEST(String, basics)
{
    // String()
    String str1;
    ASSERT_EQ(str1.size(), 0);
    ASSERT_TRUE(str1.is_empty());

    // String(const char* chars)
    String str2("hello");
    ASSERT_EQ(str2.size(), 5);
    ASSERT_FALSE(str2.is_empty());

    // String(const String& that)
    String str3(str2);
    ASSERT_EQ(str3.size(), 5);
    ASSERT_FALSE(str3.is_empty());

    // String(String&& that)
    String str4(std::move(str3));
    ASSERT_EQ(str4.size(), 5);
    ASSERT_FALSE(str4.is_empty());
    ASSERT_EQ(str3.size(), 0);
    ASSERT_TRUE(str3.is_empty());

    // ~String()
}

// operator[]()
TEST(String, access)
{
    String string("12345");

    // forward
    for (int i = 0; i < string.size(); ++i)
    {
        ASSERT_EQ(string[i], i + '1');
    }

    // backward
    for (int i = -1; i >= -string.size(); --i)
    {
        ASSERT_EQ(string[i], i + '6');
    }

    // assignment
    string[0] = '0';
    ASSERT_EQ(string[0], '0');

    string[-1] = 'Z';
    ASSERT_EQ(string[-1], 'Z');

    // check bounds
    ASSERT_THROW(string[5], std::runtime_error);
    try
    {
        string[5];
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }
}

// operator=()
TEST(String, copy_assignment)
{
    String string1("12345");
    String string2("6789");

    string1 = string2;
    string2 += "10";
    ASSERT_EQ(string1, String("6789"));
    ASSERT_EQ(string2, String("678910"));
}

// operator=()
TEST(String, move_assignment)
{
    String string1("12345");
    String string2("6789");

    string1 = std::move(string2);
    ASSERT_EQ(string1, String("6789"));
    ASSERT_EQ(string2, String());
}

// get() set()
TEST(String, get_set)
{
    String string;
    char* str = String("hello").get();
    string.set(str);
    ASSERT_EQ(string, String("hello"));
    delete[] str;
}

// operator==() operator!=() operator<() operator<=() operator>() operator>=()
TEST(String, compare)
{
    String string("12345");

    // operator==
    String eq_string("12345");
    ASSERT_TRUE(eq_string == string);

    // operator!=
    String ne_string("135");
    ASSERT_TRUE(ne_string != string);

    // operator<
    String lt_string("123");
    ASSERT_TRUE(lt_string < string);

    // operator<=
    ASSERT_TRUE(lt_string <= string);
    ASSERT_TRUE(eq_string <= string);

    // operator>
    String gt_string("123456");
    ASSERT_TRUE(gt_string > string);

    // operator>=
    ASSERT_TRUE(eq_string >= string);
    ASSERT_TRUE(gt_string >= string);
}

// find()
TEST(String, find)
{
    const String s1("");
    const String s2("a");
    const String s3("g");
    const String s4("cde");
    const String s5("abcdefg");

    ASSERT_EQ(s1.find(s1), 0);
    ASSERT_EQ(s5.find(s1), 0);
    ASSERT_EQ(s5.find(s2), 0);
    ASSERT_EQ(s5.find(s3), 6);
    ASSERT_EQ(s5.find(s4), 2);
    ASSERT_EQ(s5.find(s5), 0);

    ASSERT_EQ(s1.find(s1, 3, 99), -1);
    ASSERT_EQ(s5.find(s1, 3, 99), 3);
    ASSERT_EQ(s5.find(s2, 3, 99), -1);
    ASSERT_EQ(s5.find(s3, 3, 99), 6);
    ASSERT_EQ(s5.find(s4, 3, 99), -1);
    ASSERT_EQ(s5.find(s5, 3, 99), -1);
}

// contains() min() max() count()
TEST(String, examination)
{
    String string("12345");

    // contains
    ASSERT_EQ(string.contains("1"), true);
    ASSERT_EQ(string.contains("5"), true);
    ASSERT_EQ(string.contains("0"), false);
    ASSERT_EQ(string.contains("1", 1, 99), false);
    ASSERT_EQ(string.contains("5", 1, 99), true);
    ASSERT_EQ(string.contains("0", 1, 99), false);

    // min
    ASSERT_EQ(string.min(), '1');

    // max
    ASSERT_EQ(string.max(), '5');

    // count
    ASSERT_EQ(string.count('0'), 0);
    ASSERT_EQ(string.count('1'), 1);
}

// to_decimal()
TEST(String, to_decimal)
{
    // example
    ASSERT_DOUBLE_EQ(String("233.33").to_decimal(), 233.33);
    ASSERT_DOUBLE_EQ(String("123.456e-3").to_decimal(), 0.123456);
    ASSERT_DOUBLE_EQ(String("1e+600").to_decimal(), HUGE_VAL);
    ASSERT_TRUE(std::isnan(String("nan").to_decimal()));
    ASSERT_DOUBLE_EQ(String("inf").to_decimal(), INFINITY);

    // 0
    ASSERT_DOUBLE_EQ(String("0").to_decimal(), 0);
    ASSERT_DOUBLE_EQ(String("-0").to_decimal(), 0);
    ASSERT_DOUBLE_EQ(String("+0").to_decimal(), 0);
    ASSERT_DOUBLE_EQ(String(".0").to_decimal(), 0);
    ASSERT_DOUBLE_EQ(String("0.").to_decimal(), 0);

    // 1
    ASSERT_DOUBLE_EQ(String("1").to_decimal(), 1.0);
    ASSERT_DOUBLE_EQ(String("-1").to_decimal(), -1.0);
    ASSERT_DOUBLE_EQ(String("+1").to_decimal(), 1.0);
    ASSERT_DOUBLE_EQ(String(".1").to_decimal(), 0.1);
    ASSERT_DOUBLE_EQ(String("1.").to_decimal(), 1.0);

    // e
    ASSERT_DOUBLE_EQ(String("1e2").to_decimal(), 1e2);
    ASSERT_DOUBLE_EQ(String("-1e2").to_decimal(), -1e2);
    ASSERT_DOUBLE_EQ(String("+1e2").to_decimal(), 1e2);
    ASSERT_DOUBLE_EQ(String(".1e2").to_decimal(), 0.1e2);
    ASSERT_DOUBLE_EQ(String("1.e2").to_decimal(), 1.e2);

    // e+
    ASSERT_DOUBLE_EQ(String("1e+2").to_decimal(), 1e+2);
    ASSERT_DOUBLE_EQ(String("-1e+2").to_decimal(), -1e+2);
    ASSERT_DOUBLE_EQ(String("+1e+2").to_decimal(), 1e+2);
    ASSERT_DOUBLE_EQ(String(".1e+2").to_decimal(), 0.1e+2);
    ASSERT_DOUBLE_EQ(String("1.e+2").to_decimal(), 1.e+2);

    // e-
    ASSERT_DOUBLE_EQ(String("1e-2").to_decimal(), 1e-2);
    ASSERT_DOUBLE_EQ(String("-1e-2").to_decimal(), -1e-2);
    ASSERT_DOUBLE_EQ(String("+1e-2").to_decimal(), 1e-2);
    ASSERT_DOUBLE_EQ(String(".1e-2").to_decimal(), 0.1e-2);
    ASSERT_DOUBLE_EQ(String("1.e-2").to_decimal(), 1.e-2);

    // error
    ASSERT_THROW(String("hello").to_decimal(), std::runtime_error);
    try
    {
        String("hello").to_decimal();
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Invalid literal for to_decimal().");
    }
}

// to_integer()
TEST(String, to_integer)
{
    // example
    ASSERT_EQ(String("233").to_integer(), 233);
    ASSERT_EQ(String("cafebabe").to_integer(16), 3405691582);
    ASSERT_EQ(String("z").to_integer(36), 35);

    // other
    ASSERT_EQ(String("0001000").to_integer(), 1000);
    ASSERT_EQ(String("1").to_integer(), 1);
    ASSERT_EQ(String("0").to_integer(), 0);
    ASSERT_EQ(String("f").to_integer(16), 15);
    ASSERT_EQ(String("11").to_integer(2), 3);
    ASSERT_EQ(String("zz").to_integer(36), 35 * 36 + 35);
    ASSERT_EQ(String("-1").to_integer(), -1);
    ASSERT_EQ(String("-0").to_integer(), 0);
    ASSERT_EQ(String("-10").to_integer(), -10);
    ASSERT_EQ(String("-10").to_integer(16), -16);
    ASSERT_EQ(String("-z").to_integer(36), -35);
    ASSERT_EQ(String("+1").to_integer(), 1);
    ASSERT_EQ(String("+0").to_integer(), 0);
    ASSERT_EQ(String("+10").to_integer(), 10);
    ASSERT_EQ(String("+10").to_integer(16), 16);
    ASSERT_EQ(String("+z").to_integer(36), 35);
    ASSERT_EQ(String("-0101").to_integer(2), -5);
    ASSERT_EQ(String("-1010").to_integer(2), -10);
    ASSERT_EQ(String("+0101").to_integer(2), 5);
    ASSERT_EQ(String("+1010").to_integer(2), 10);
    ASSERT_EQ(String("\n\r\n\t  233  \t\r\n\r").to_integer(), 233);

    // error
    ASSERT_THROW(String("123").to_integer(99), std::runtime_error);
    try
    {
        String("123").to_integer(99);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Invalid base for to_integer().");
    }

    ASSERT_THROW(String("!!!").to_integer(), std::runtime_error);
    try
    {
        String("!!!").to_integer();
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Invalid literal for to_integer().");
    }
}

// insert()
TEST(String, insert)
{
    String string;

    // check bounds
    ASSERT_THROW(string.insert(999, '0'), std::runtime_error);
    try
    {
        string.insert(999, '0');
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }

    // insert
    string.insert(0, 'a');
    ASSERT_EQ(string, String("a"));
    string.insert(0, 'b');
    ASSERT_EQ(string, String("ba"));
    string.insert(2, 'c');
    ASSERT_EQ(string, String("bac"));
    string.insert(1, 'd');
    ASSERT_EQ(string, String("bdac"));
    string.insert(-1, 'z'); // expand capacity
    ASSERT_EQ(string, String("bdazc"));
}

// remove()
TEST(String, remove)
{
    String string("bdazc");

    // check bounds
    ASSERT_THROW(string.remove(999), std::runtime_error);
    try
    {
        string.remove(999);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }

    // remove
    ASSERT_EQ(string.remove(-2), 'z');
    ASSERT_EQ(string.remove(1), 'd');
    ASSERT_EQ(string.remove(0), 'b');
    ASSERT_EQ(string.remove(0), 'a');
    ASSERT_EQ(string.remove(0), 'c');

    // check empty
    ASSERT_THROW(string.remove(0), std::runtime_error);
    try
    {
        string.remove(0);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: The container is empty.");
    }
}

// operator+=()
TEST(String, append)
{
    String string;

    // append element
    ASSERT_EQ(string += '2', String("2"));
    ASSERT_EQ(string += '3', String("23"));
    ASSERT_EQ(string += '3', String("233"));
    ASSERT_EQ(string += '3', String("2333"));
    ASSERT_EQ(string += '3', String("23333")); // expand capacity

    // append string
    ASSERT_EQ(string += string, String("2333323333"));
    ASSERT_EQ(string += string, String("23333233332333323333"));
    ASSERT_EQ(string += String("00"), String("2333323333233332333300"));
}

// operator-=()
TEST(String, remove_element)
{
    String string("12345");

    ASSERT_EQ(string -= '1', String("2345"));
    ASSERT_EQ(string -= '2', String("345"));
    ASSERT_EQ(string -= '3', String("45"));
    ASSERT_EQ(string -= '4', String("5"));
    ASSERT_EQ(string -= '5', String(""));

    // if the string does not contain the element, it is unchanged.
    ASSERT_EQ(string -= '6', String());
}

// operator*=()
TEST(String, repeat)
{
    String string("12");

    ASSERT_THROW(string *= -1, std::runtime_error);
    try
    {
        string *= -1;
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Times to repeat cannot be less than zero.");
    }

    ASSERT_EQ(string *= 1, String("12"));
    ASSERT_EQ(string *= 2, String("1212"));
    ASSERT_EQ(string *= 0, String());
}

// clear()
TEST(String, clear)
{
    String string("12345");

    ASSERT_EQ(string.clear(), String());

    // double clear
    ASSERT_EQ(string.clear(), String());
}

// traverse()
TEST(String, traverse)
{
    String string("12345");

    string.traverse([](char& x)
                    { x += 1; });
    ASSERT_EQ(string, String("23456"));

    string.traverse([](char& x)
                    { x = '1'; });
    ASSERT_EQ(string, String("11111"));
}

// reverse()
TEST(String, reverse)
{
    ASSERT_EQ(String().reverse(), String());

    ASSERT_EQ(String("12345").reverse(), String("54321"));
}

// uniquify()
TEST(String, uniquify)
{
    ASSERT_EQ(String("122333").uniquify(), String("123"));

    ASSERT_EQ(String("00000000000000").uniquify(), String("0"));

    ASSERT_EQ(String("123123123").uniquify(), String("123"));

    String many;
    for (int i = 0; i < 10000; i++)
    {
        many += 'A';
    }
    ASSERT_EQ(many.uniquify(), String({"A"}));
}

// sort()
TEST(String, sort)
{
    String string("1357986420");

    // from small to large
    string.sort();
    ASSERT_EQ(string, String("0123456789"));

    // from large to small
    string.sort([](const char& e1, const char& e2)
                { return e1 > e2; });
    ASSERT_EQ(string, String("9876543210"));
}

// swap()
TEST(String, swap)
{
    String s1("first");
    String s2("second");

    s1.swap(s2);

    ASSERT_EQ(s1, String("second"));
    ASSERT_EQ(s2, String("first"));
}

// lower() upper()
TEST(String, lower_upper)
{
    ASSERT_EQ(String("hahaha").upper(), String("HAHAHA"));

    ASSERT_EQ(String("HAHAHA").lower(), String("hahaha"));
}

// erase()
TEST(String, erase)
{
    ASSERT_EQ(String("abcdefg").erase(0, 1), String("bcdefg"));
    ASSERT_EQ(String("abcdefg").erase(1, 2), String("acdefg"));
    ASSERT_EQ(String("abcdefg").erase(1, 6), String("ag"));
    ASSERT_EQ(String("abcdefg").erase(0, 7), String(""));

    ASSERT_THROW(String("abcdefg").erase(-1, 99), std::runtime_error);
    try
    {
        String("abcdefg").erase(-1, 99);
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }
}

// replace()
TEST(String, replace)
{
    ASSERT_EQ(String("abcdefg").replace("a", "g"), String("gbcdefg"));
    ASSERT_EQ(String("abcdefg").replace("g", "a"), String("abcdefa"));
    ASSERT_EQ(String("abcdefg").replace("cde", "~~~"), String("ab~~~fg"));
    ASSERT_EQ(String("abcdefg").replace("abcdefg", ""), String(""));
    ASSERT_EQ(String("abcdefg").replace("abcdefg", ""), String(""));
    ASSERT_EQ(String("").replace("abc", "~~~"), String(""));
    ASSERT_EQ(String("hahaha").replace("h", "l"), String("lalala"));
    ASSERT_EQ(String("hahaha").replace("a", "ooow~ "), String("hooow~ hooow~ hooow~ "));
    ASSERT_EQ(String("hooow~ hooow~ hooow~ ").replace("ooo", "o"), String("how~ how~ how~ "));
}

// strip()
TEST(String, strip)
{
    ASSERT_EQ(String("\t\nhello\t\n").strip(), String("hello"));
    ASSERT_EQ(String("           hello           ").strip(), String("hello"));
    ASSERT_EQ(String("\n\n\n\n \t\n\b\n   hello  \n\n\t\n \r\b\n\r").strip(), String("hello"));
}

// slice()
TEST(String, slice)
{
    String string("12345");

    ASSERT_EQ(string.slice(-1, 1), String());
    ASSERT_EQ(string.slice(-1, 1, -1), String("543"));
    ASSERT_EQ(string.slice(1, -1), String("234"));
    ASSERT_EQ(string.slice(1, -1, -1), String());

    ASSERT_EQ(string.slice(0, 5), String("12345"));
    ASSERT_EQ(string.slice(0, 5, 2), String("135"));
    ASSERT_EQ(string.slice(0, 5, -1), String());
    ASSERT_EQ(string.slice(0, 5, -2), String());

    ASSERT_EQ(string.slice(-1, -6), String());
    ASSERT_EQ(string.slice(-1, -6, 2), String());
    ASSERT_EQ(string.slice(-1, -6, -1), String("54321"));
    ASSERT_EQ(string.slice(-1, -6, -2), String("531"));

    ASSERT_EQ(string.slice(0, 0), String());
    ASSERT_EQ(string.slice(1, 1), String());
    ASSERT_EQ(string.slice(-1, -1), String());
    ASSERT_EQ(string.slice(-1, -1, -1), String());

    ASSERT_THROW(string.slice(1, 2, 0), std::runtime_error);
    try
    {
        string.slice(1, 2, 0);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Slice step cannot be zero.");
    }

    ASSERT_THROW(string.slice(-7, -6), std::runtime_error);
    try
    {
        string.slice(-7, -6);
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }
}

// operator+() operator-() operator*()
TEST(String, production)
{
    String string("12345");

    // operator+
    ASSERT_EQ(string + '6', String("123456"));
    ASSERT_EQ(string + String("67"), String("1234567"));

    // operator-
    ASSERT_EQ(string - '5', String("1234"));
    ASSERT_EQ(string - '6', String("12345"));

    // operator*
    ASSERT_EQ(string * 0, String());
    ASSERT_EQ(string * 1, String("12345"));
    ASSERT_EQ(string * 2, String("1234512345"));
    ASSERT_EQ(0 * string, String());
    ASSERT_EQ(1 * string, String("12345"));
    ASSERT_EQ(2 * string, String("1234512345"));
}

// split()
TEST(String, split)
{
    ASSERT_EQ(String("one, two, three").split(", "), List<String>({"one", "two", "three"}));
    ASSERT_EQ(String("this is my code!").split(" "), List<String>({"this", "is", "my", "code!"}));
    ASSERT_EQ(String("this is my code!").split("this is my code!"), List<String>({""}));
    ASSERT_EQ(String(" this is my code! ").split(" "), List<String>({"", "this", "is", "my", "code!"}));
    ASSERT_EQ(String("aaa").split("a"), List<String>({"", "", ""}));
    ASSERT_EQ(String("192.168.0.1").split("."), List<String>({"192", "168", "0", "1"}));
}

// join()
TEST(String, join)
{
    ASSERT_EQ(String(", ").join(List<String>()), String());
    ASSERT_EQ(String(", ").join(List<String>({"a"})), String("a"));
    ASSERT_EQ(String(", ").join(List<String>({"a", "b"})), String("a, b"));
    ASSERT_EQ(String(", ").join(List<String>({"a", "b", "c"})), String("a, b, c"));
    ASSERT_EQ(String(".").join(List<String>({"192", "168", "0", "1"})), String("192.168.0.1"));
}

// operator<<()
TEST(String, print)
{
    std::ostringstream oss;

    String empty;
    oss << empty;
    ASSERT_EQ(oss.str(), "\"\""); // string == char*, use eq
    oss.str("");

    String one("1");
    oss << one;
    ASSERT_EQ(oss.str(), "\"1\"");
    oss.str("");

    String many("hello world");
    oss << many;
    ASSERT_EQ(oss.str(), "\"hello world\"");
    oss.str("");
}
