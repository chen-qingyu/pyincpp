// Tested on Microsoft Visual Studio Community 2019: ALL PASSED

#include "pch.h"

#include <sstream> // std::ostringstream
#include <string>  // std::string std::to_string

#include "../Sources/List.hpp"

using mdspp::List;

// constructor destructor size() is_empty()
TEST(List, basics)
{
    // List()
    List<int> list1;
    ASSERT_EQ(list1.size(), 0);
    ASSERT_TRUE(list1.is_empty());

    // List(const std::initializer_list<T> &il)
    List<int> list2 = {1, 2, 3, 4, 5};
    ASSERT_EQ(list2.size(), 5);
    ASSERT_FALSE(list2.is_empty());

    // List(const List<T> &that)
    List<int> list3(list2);
    ASSERT_EQ(list3.size(), 5);
    ASSERT_FALSE(list3.is_empty());

    // List(List<T> &&that)
    List<int> list4(std::move(list3));
    ASSERT_EQ(list4.size(), 5);
    ASSERT_FALSE(list4.is_empty());
    ASSERT_EQ(list3.size(), 0);
    ASSERT_TRUE(list3.is_empty());

    // ~List()
}

// operator[]()
TEST(List, access)
{
    List<int> list = {1, 2, 3, 4, 5};

    // forward
    for (int i = 0; i < list.size(); ++i)
    {
        ASSERT_EQ(list[i], i + 1);
    }

    // backward
    for (int i = -1; i >= -list.size(); --i)
    {
        ASSERT_EQ(list[i], i + 6);
    }

    // assignment
    list[0] = 0;
    ASSERT_EQ(list[0], 0);

    list[-1] = 999;
    ASSERT_EQ(list[-1], 999);

    // check bounds
    ASSERT_THROW(list[5], std::runtime_error);
    try
    {
        list[5];
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }
}

// begin() end()
TEST(List, iterator)
{
    // empty
    List<int> empty;
    ASSERT_EQ(empty.begin(), empty.end());

    List<int> list = {1, 2, 3, 4, 5};

    // for
    int i = 1;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        ASSERT_EQ(*it, i);
        i++;
    }

    // for in
    i = 1;
    for (const auto& e : list)
    {
        ASSERT_EQ(e, i);
        ++i;
    }
}

// operator=()
TEST(List, copy_assignment)
{
    List<int> list1 = {1, 2, 3, 4, 5};
    List<int> list2 = {6, 7, 8, 9};

    list1 = list2;
    list2 += 10;
    ASSERT_EQ(list1, List<int>({6, 7, 8, 9}));
    ASSERT_EQ(list2, List<int>({6, 7, 8, 9, 10}));
}

// operator=()
TEST(List, move_assignment)
{
    List<int> list1 = {1, 2, 3, 4, 5};
    List<int> list2 = {6, 7, 8, 9};

    list1 = std::move(list2);
    ASSERT_EQ(list1, List<int>({6, 7, 8, 9}));
    ASSERT_EQ(list2, List<int>());
}

// operator==() operator!=()
TEST(List, compare)
{
    List<int> list = {1, 2, 3, 4, 5};

    // operator==
    List<int> eq_list = {1, 2, 3, 4, 5};
    ASSERT_TRUE(list == eq_list);

    // operator!=
    List<int> ne_list = {1, 3, 5};
    ASSERT_TRUE(list != ne_list);
}

// find() contains() min() max() count()
TEST(List, examination)
{
    List<int> list = {1, 2, 3, 4, 5};

    // find
    ASSERT_EQ(list.find(1), 0);
    ASSERT_EQ(list.find(5), 4);
    ASSERT_EQ(list.find(0), -1);
    ASSERT_EQ(list.find(1, 1, 99), -1);
    ASSERT_EQ(list.find(5, 1, 99), 4);
    ASSERT_EQ(list.find(0, 1, 99), -1);

    // contains
    ASSERT_EQ(list.contains(1), true);
    ASSERT_EQ(list.contains(5), true);
    ASSERT_EQ(list.contains(0), false);
    ASSERT_EQ(list.contains(1, 1, 99), false);
    ASSERT_EQ(list.contains(5, 1, 99), true);
    ASSERT_EQ(list.contains(0, 1, 99), false);

    // min
    ASSERT_EQ(list.min(), 1);

    // max
    ASSERT_EQ(list.max(), 5);

    // count
    ASSERT_EQ(list.count(0), 0);
    ASSERT_EQ(list.count(1), 1);
}

// insert()
TEST(List, insert)
{
    List<int> list;

    // check bounds
    ASSERT_THROW(list.insert(999, 0), std::runtime_error);
    try
    {
        list.insert(999, 0);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }

    // insert
    list.insert(0, 233);
    ASSERT_EQ(list, List<int>({233}));
    list.insert(0, 1);
    ASSERT_EQ(list, List<int>({1, 233}));
    list.insert(2, 999);
    ASSERT_EQ(list, List<int>({1, 233, 999}));
    list.insert(1, 5);
    ASSERT_EQ(list, List<int>({1, 5, 233, 999}));
    list.insert(-1, -1); // expand capacity
    ASSERT_EQ(list, List<int>({1, 5, 233, -1, 999}));

    // check full
    // 测试成功了！但是每次测试耗时太长，可以注释掉。
    // List<bool> big_list;
    // for (int i = 0; i < INT_MAX; i++)
    // {
    //     big_list.insert(big_list.size(), true);
    // }
    // ASSERT_EQ(big_list.size(), INT_MAX);
    // ASSERT_THROW(big_list.insert(big_list.size(), true), std::runtime_error);
    // try
    // {
    //     big_list.insert(big_list.size(), true);
    // }
    // catch (const std::runtime_error& e)
    // {
    //     ASSERT_STREQ(e.what(), "ERROR: The container has reached the maximum size.");
    // }

    // modify after inserted
    List<std::string> str_list;
    std::string str = "test string";
    str_list.insert(0, str);
    str.append(" changed");
    ASSERT_EQ(str, "test string changed"); // string == char*, use eq
    ASSERT_EQ(str_list[0], "test string");
}

// remove()
TEST(List, remove)
{
    List<int> list = {1, 5, 233, -1, 999};

    // check bounds
    ASSERT_THROW(list.remove(999), std::runtime_error);
    try
    {
        list.remove(999);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }

    // remove
    ASSERT_EQ(list.remove(-2), -1);
    ASSERT_EQ(list.remove(1), 5);
    ASSERT_EQ(list.remove(0), 1);
    ASSERT_EQ(list.remove(0), 233);
    ASSERT_EQ(list.remove(0), 999);

    // check empty
    ASSERT_THROW(list.remove(0), std::runtime_error);
    try
    {
        list.remove(0);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: The container is empty.");
    }
}

// operator+=()
TEST(List, append)
{
    List<int> list;

    // append element
    ASSERT_EQ(list += 2, List<int>({2}));
    ASSERT_EQ(list += 3, List<int>({2, 3}));
    ASSERT_EQ(list += 3, List<int>({2, 3, 3}));
    ASSERT_EQ(list += 3, List<int>({2, 3, 3, 3}));
    ASSERT_EQ(list += 3, List<int>({2, 3, 3, 3, 3})); // expand capacity

    // append list
    ASSERT_EQ(list += list, List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3}));
    ASSERT_EQ(list += list, List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3}));
    ASSERT_EQ(list += List<int>({0, 0}), List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 0, 0}));
}

// operator-=()
TEST(List, remove_element)
{
    List<int> list = {1, 2, 3, 4, 5};

    ASSERT_EQ(list -= 1, List<int>({2, 3, 4, 5}));
    ASSERT_EQ(list -= 2, List<int>({3, 4, 5}));
    ASSERT_EQ(list -= 3, List<int>({4, 5}));
    ASSERT_EQ(list -= 4, List<int>({5}));
    ASSERT_EQ(list -= 5, List<int>({}));

    // if the list does not contain the element, it is unchanged.
    ASSERT_EQ(list -= 6, List<int>({}));
}

// operator*=()
TEST(List, repeat)
{
    List<int> list = {1, 2};

    ASSERT_THROW(list *= -1, std::runtime_error);
    try
    {
        list *= -1;
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Times to repeat cannot be less than zero.");
    }

    ASSERT_EQ(list *= 1, List<int>({1, 2}));
    ASSERT_EQ(list *= 2, List<int>({1, 2, 1, 2}));
    ASSERT_EQ(list *= 0, List<int>({}));
}

// clear()
TEST(List, clear)
{
    List<int> list = {1, 2, 3, 4, 5};

    ASSERT_EQ(list.clear(), List<int>());

    // double clear
    ASSERT_EQ(list.clear(), List<int>());
}

// traverse()
TEST(List, traverse)
{
    List<int> list = {1, 2, 3, 4, 5};

    list.traverse([](int& x)
                  { x *= 2; });
    ASSERT_EQ(list, List<int>({2, 4, 6, 8, 10}));

    list.traverse([](int& x)
                  { x = 1; });
    ASSERT_EQ(list, List<int>({1, 1, 1, 1, 1}));

    std::string str;
    list.traverse([&](int& x)
                  { str += std::to_string(x) + " "; });
    ASSERT_EQ(str, "1 1 1 1 1 ");
}

// reverse()
TEST(List, reverse)
{
    ASSERT_EQ(List<int>().reverse(), List<int>());

    ASSERT_EQ(List<int>({1, 2, 3, 4, 5}).reverse(), List<int>({5, 4, 3, 2, 1}));
}

// uniquify()
TEST(List, uniquify)
{
    ASSERT_EQ(List<int>({1, 2, 2, 3, 3, 3}).uniquify(), List<int>({1, 2, 3}));

    ASSERT_EQ(List<int>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}).uniquify(), List<int>({0}));

    ASSERT_EQ(List<int>({1, 2, 3, 1, 2, 3, 1, 2, 3}).uniquify(), List<int>({1, 2, 3}));

    List<int> many;
    for (int i = 0; i < 10000; i++)
    {
        many += 0;
    }
    ASSERT_EQ(many.uniquify(), List<int>({0}));
}

// for TEST(List, sort)
struct Person
{
    std::string name;
    int age;

    bool operator==(const Person& that)
    {
        return age == that.age && name == that.name;
    }

    bool operator!=(const Person& that)
    {
        return !(*this == that);
    }
};

std::ostream& operator<<(std::ostream& os, const Person& person)
{
    return os << "(" << person.name << ", " << person.age << ")";
}

// sort()
TEST(List, sort)
{
    List<int> list = {1, 3, 5, 7, 9, 8, 6, 4, 2, 0};

    // from small to large
    list.sort();
    ASSERT_EQ(list, List<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

    // from large to small
    list.sort([](const int& e1, const int& e2)
              { return e1 > e2; });
    ASSERT_EQ(list, List<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));

    List<Person> persons = {
        {"Alice", 18},
        {"Sakura", 19},
        {"Homura", 20},
        {"Mei", 17},
        {"Yuzu", 18},
    };

    // sort by age
    persons.sort([](const Person& e1, const Person& e2)
                 { return e1.age < e2.age; });
    ASSERT_EQ(persons, List<Person>({
                           {"Mei", 17},
                           {"Alice", 18},
                           {"Yuzu", 18},
                           {"Sakura", 19},
                           {"Homura", 20},
                       }));

    // sort by name
    persons.sort([](const Person& e1, const Person& e2)
                 { return e1.name < e2.name; });
    ASSERT_EQ(persons, List<Person>({
                           {"Alice", 18},
                           {"Homura", 20},
                           {"Mei", 17},
                           {"Sakura", 19},
                           {"Yuzu", 18},
                       }));
}

// swap()
TEST(List, swap)
{
    List<int> list1({1, 2, 3});
    List<int> list2({4, 5, 6});

    list1.swap(list2);

    ASSERT_EQ(list1, List<int>({4, 5, 6}));
    ASSERT_EQ(list2, List<int>({1, 2, 3}));
}

// slice()
TEST(List, slice)
{
    List<int> list = {1, 2, 3, 4, 5};

    ASSERT_EQ(list.slice(-1, 1), List<int>({}));
    ASSERT_EQ(list.slice(-1, 1, -1), List<int>({5, 4, 3}));
    ASSERT_EQ(list.slice(1, -1), List<int>({2, 3, 4}));
    ASSERT_EQ(list.slice(1, -1, -1), List<int>({}));

    ASSERT_EQ(list.slice(0, 5), List<int>({1, 2, 3, 4, 5}));
    ASSERT_EQ(list.slice(0, 5, 2), List<int>({1, 3, 5}));
    ASSERT_EQ(list.slice(0, 5, -1), List<int>({}));
    ASSERT_EQ(list.slice(0, 5, -2), List<int>({}));

    ASSERT_EQ(list.slice(-1, -6), List<int>({}));
    ASSERT_EQ(list.slice(-1, -6, 2), List<int>({}));
    ASSERT_EQ(list.slice(-1, -6, -1), List<int>({5, 4, 3, 2, 1}));
    ASSERT_EQ(list.slice(-1, -6, -2), List<int>({5, 3, 1}));

    ASSERT_EQ(list.slice(0, 0), List<int>({}));
    ASSERT_EQ(list.slice(1, 1), List<int>({}));
    ASSERT_EQ(list.slice(-1, -1), List<int>({}));
    ASSERT_EQ(list.slice(-1, -1, -1), List<int>({}));

    ASSERT_THROW(list.slice(1, 2, 0), std::runtime_error);
    try
    {
        list.slice(1, 2, 0);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Slice step cannot be zero.");
    }

    ASSERT_THROW(list.slice(-7, -6), std::runtime_error);
    try
    {
        list.slice(-7, -6);
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }
}

// operator+() operator-() operator*()
TEST(List, production)
{
    List<int> list = {1, 2, 3, 4, 5};

    // operator+
    ASSERT_EQ(list + 6, List<int>({1, 2, 3, 4, 5, 6}));
    ASSERT_EQ(list + List<int>({6, 7}), List<int>({1, 2, 3, 4, 5, 6, 7}));

    // operator-
    ASSERT_EQ(list - 5, List<int>({1, 2, 3, 4}));
    ASSERT_EQ(list - 6, List<int>({1, 2, 3, 4, 5}));

    // operator*
    ASSERT_EQ(list * 0, List<int>({}));
    ASSERT_EQ(list * 1, List<int>({1, 2, 3, 4, 5}));
    ASSERT_EQ(list * 2, List<int>({1, 2, 3, 4, 5, 1, 2, 3, 4, 5}));
    ASSERT_EQ(0 * list, List<int>({}));
    ASSERT_EQ(1 * list, List<int>({1, 2, 3, 4, 5}));
    ASSERT_EQ(2 * list, List<int>({1, 2, 3, 4, 5, 1, 2, 3, 4, 5}));
}

// operator<<()
TEST(List, print)
{
    std::ostringstream oss;

    List<int> empty;
    oss << empty;
    ASSERT_EQ(oss.str(), "[]"); // string == char*, use eq
    oss.str("");

    List<int> one = {1};
    oss << one;
    ASSERT_EQ(oss.str(), "[1]");
    oss.str("");

    List<int> many = {1, 2, 3, 4, 5};
    oss << many;
    ASSERT_EQ(oss.str(), "[1, 2, 3, 4, 5]");
    oss.str("");
}
