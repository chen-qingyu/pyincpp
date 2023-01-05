#include "pch.h"

#include <string>

#include "../Sources/List.hpp"

using std::string;

using mdspp::List;

// constructor, destructor, size(), is_empty()
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
    try
    {
        list[5];
    }
    catch (std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: Out of range.");
    }
}

// begin(), end()
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

// index_of(), find(), contains(), operator==(), operator!=()
TEST(List, examination)
{
    List<int> list = {1, 2, 3, 4, 5};

    // index_of
    ASSERT_EQ(list.index_of(1), 0);
    ASSERT_EQ(list.index_of(5), 4);
    ASSERT_EQ(list.index_of(0), -1);

    // find
    ASSERT_EQ(list.find(1), list.begin());
    ASSERT_EQ(list.find(5), --list.end());
    ASSERT_EQ(list.find(0), list.end());

    // contains
    ASSERT_EQ(list.contains(1), true);
    ASSERT_EQ(list.contains(5), true);
    ASSERT_EQ(list.contains(0), false);

    // operator==
    List<int> eq_list = {1, 2, 3, 4, 5};
    ASSERT_TRUE(list == eq_list);

    // operator!=
    List<int> ne_list = {1, 3, 5};
    ASSERT_TRUE(list != ne_list);
}

// insert()
TEST(List, insert)
{
    List<int> list;

    // check bounds
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
    list.insert(-1, -1);
    ASSERT_EQ(list, List<int>({1, 5, 233, -1, 999}));

    // check full
    // 测试成功了！但是每次测试耗时太长，可以注释掉。
    // List<bool> big_list;
    // for (int i = 0; i < INT_MAX; i++)
    // {
    //     big_list.insert(big_list.size(), true);
    // }
    // ASSERT_EQ(big_list.size(), INT_MAX);
    // try
    // {
    //     big_list.insert(big_list.size(), true);
    // }
    // catch (const std::runtime_error& e)
    // {
    //     ASSERT_STREQ(e.what(), "ERROR: The container has reached the maximum size.");
    // }

    // modify after inserted
    List<string> str_list;
    string str = "test string";
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
    try
    {
        list.remove(0);
    }
    catch (const std::runtime_error& e)
    {
        ASSERT_STREQ(e.what(), "ERROR: The container is empty.");
    }
}
