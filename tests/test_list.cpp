#include "../sources/List.hpp"

#include "tool.hpp"

using namespace pytype;

// constructor destructor size() is_empty()
TEST_CASE("List: basics")
{
    // List()
    List<int> list1;
    REQUIRE(list1.size() == 0);
    REQUIRE(list1.is_empty());

    // List(const std::initializer_list<T> &il)
    List<int> list2 = {1, 2, 3, 4, 5};
    REQUIRE(list2.size() == 5);
    REQUIRE(!list2.is_empty());

    // List(const List<T> &that)
    List<int> list3(list2);
    REQUIRE(list3.size() == 5);
    REQUIRE(!list3.is_empty());

    // List(List<T> &&that)
    List<int> list4(std::move(list3));
    REQUIRE(list4.size() == 5);
    REQUIRE(!list4.is_empty());
    REQUIRE(list3.size() == 0);
    REQUIRE(list3.is_empty());

    // ~List()
}

// operator==() operator!=() operator<() operator<=() operator>() operator>=()
TEST_CASE("List: compare")
{
    List<int> list({1, 2, 3, 4, 5});

    // operator==
    List<int> eq_list({1, 2, 3, 4, 5});
    REQUIRE(eq_list == list);

    // operator!=
    List<int> ne_list({1, 3, 5});
    REQUIRE(ne_list != list);

    // operator<
    List<int> lt_list({1, 2, 3});
    List<int> lt_list2({0, 9, 9, 9, 9});
    REQUIRE(lt_list < list);
    REQUIRE(lt_list2 < list);

    // operator<=
    REQUIRE(lt_list <= list);
    REQUIRE(lt_list2 <= list);
    REQUIRE(eq_list <= list);

    // operator>
    List<int> gt_list({1, 2, 3, 4, 5, 6});
    List<int> gt_list2({2});
    REQUIRE(gt_list > list);
    REQUIRE(gt_list2 > list);

    // operator>=
    REQUIRE(gt_list >= list);
    REQUIRE(gt_list2 >= list);
    REQUIRE(eq_list >= list);
}

// operator=()
TEST_CASE("List: copy_assignment")
{
    List<int> list1 = {1, 2, 3, 4, 5};
    List<int> list2 = {6, 7, 8, 9};

    list1 = list2;
    REQUIRE(list1 == List<int>({6, 7, 8, 9}));
    REQUIRE(list2 == List<int>({6, 7, 8, 9}));
}

// operator=()
TEST_CASE("List: move_assignment")
{
    List<int> list1 = {1, 2, 3, 4, 5};
    List<int> list2 = {6, 7, 8, 9};

    list1 = std::move(list2);
    REQUIRE(list1 == List<int>({6, 7, 8, 9}));
    REQUIRE(list2 == List<int>());
}

// operator[]()
TEST_CASE("List: access")
{
    List<int> list = {1, 2, 3, 4, 5};

    // forward
    for (int i = 0; i < list.size(); ++i)
    {
        REQUIRE(list[i] == i + 1);
    }

    // backward
    for (int i = -1; i >= -list.size(); --i)
    {
        REQUIRE(list[i] == i + 6);
    }

    // assignment
    list[0] = 0;
    REQUIRE(list[0] == 0);

    list[-1] = 999;
    REQUIRE(list[-1] == 999);

    // check bounds
    MY_ASSERT_THROW_MESSAGE(list[5], std::runtime_error, "Error: Index out of range.");
}

// begin() end()
TEST_CASE("List: iterator")
{
    // empty
    List<int> empty;
    REQUIRE(empty.begin() == empty.end());

    List<int> list = {1, 2, 3, 4, 5};

    // for
    int i = 1;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        REQUIRE(*it == i++);
    }

    // for in
    i = 1;
    for (const auto& e : list)
    {
        REQUIRE(e == i++);
    }
}

// find() contains() min() max() count()
TEST_CASE("List: examination")
{
    List<int> list = {1, 2, 3, 4, 5};

    // find
    REQUIRE(list.find(1) == 0);
    REQUIRE(list.find(5) == 4);
    REQUIRE(list.find(0) == -1);
    REQUIRE(list.find(1, 1, 99) == -1);
    REQUIRE(list.find(5, 1, 99) == 4);
    REQUIRE(list.find(0, 1, 99) == -1);

    // contains
    REQUIRE(list.contains(1) == true);
    REQUIRE(list.contains(5) == true);
    REQUIRE(list.contains(0) == false);
    REQUIRE(list.contains(1, 1, 99) == false);
    REQUIRE(list.contains(5, 1, 99) == true);
    REQUIRE(list.contains(0, 1, 99) == false);

    // min
    REQUIRE(list.min() == 1);

    // max
    REQUIRE(list.max() == 5);

    // count
    REQUIRE(list.count(0) == 0);
    REQUIRE(list.count(1) == 1);
}

// insert()
TEST_CASE("List: insert")
{
    List<int> list;

    // check bounds
    MY_ASSERT_THROW_MESSAGE(list.insert(999, 0), std::runtime_error, "Error: Index out of range.");

    // insert
    list.insert(0, 233);
    REQUIRE(list == List<int>({233}));
    list.insert(0, 1);
    REQUIRE(list == List<int>({1, 233}));
    list.insert(2, 999);
    REQUIRE(list == List<int>({1, 233, 999}));
    list.insert(1, 5);
    REQUIRE(list == List<int>({1, 5, 233, 999}));
    list.insert(-1, -1); // expand capacity
    REQUIRE(list == List<int>({1, 5, 233, -1, 999}));

    // check full
    // The test was successful! But the testing time is too long, and comment it out.
    // List<bool> big_list;
    // for (int i = 0; i < List<bool>::MAX_CAPACITY; i++)
    // {
    //     big_list.insert(big_list.size(), true);
    // }
    // REQUIRE(big_list.size() == List<bool>::MAX_CAPACITY);
    // MY_ASSERT_THROW_MESSAGE(big_list.insert(big_list.size(), true), std::runtime_error, "Error: The container has reached the maximum size.");

    // modify after inserted
    List<std::string> str_list;
    std::string str = "test string";
    str_list.insert(0, str);
    str.append(" changed");
    REQUIRE(str == "test string changed");
    REQUIRE(str_list[0] == "test string");
}

// remove()
TEST_CASE("List: remove")
{
    List<int> list = {1, 5, 233, -1, 999};

    // check bounds
    MY_ASSERT_THROW_MESSAGE(list.remove(999), std::runtime_error, "Error: Index out of range.");

    // remove
    REQUIRE(list.remove(-2) == -1);
    REQUIRE(list.remove(1) == 5);
    REQUIRE(list.remove(0) == 1);
    REQUIRE(list.remove(0) == 233);
    REQUIRE(list.remove(0) == 999);

    // check empty
    MY_ASSERT_THROW_MESSAGE(list.remove(0), std::runtime_error, "Error: The container is empty.");
}

// operator+=()
TEST_CASE("List: append")
{
    List<int> list;

    // append element
    REQUIRE((list += 2) == List<int>({2}));
    REQUIRE((list += 3) == List<int>({2, 3}));
    REQUIRE((list += 3) == List<int>({2, 3, 3}));
    REQUIRE((list += 3) == List<int>({2, 3, 3, 3}));
    REQUIRE((list += 3) == List<int>({2, 3, 3, 3, 3}));

    // append list
    REQUIRE((list += list) == List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3}));
    REQUIRE((list += list) == List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3}));
    REQUIRE((list += List<int>({0, 0})) == List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 0, 0}));
}

// operator-=()
TEST_CASE("List: remove_element")
{
    List<int> list = {1, 2, 3, 4, 5};

    REQUIRE((list -= 1) == List<int>({2, 3, 4, 5}));
    REQUIRE((list -= 2) == List<int>({3, 4, 5}));
    REQUIRE((list -= 3) == List<int>({4, 5}));
    REQUIRE((list -= 4) == List<int>({5}));
    REQUIRE((list -= 5) == List<int>({}));

    // if the list does not contain the element, it is unchanged.
    REQUIRE((list -= 6) == List<int>({}));
}

// operator*=()
TEST_CASE("List: repeat")
{
    List<int> list = {1, 2};

    MY_ASSERT_THROW_MESSAGE(list *= -1, std::runtime_error, "Error: Times to repeat can not be less than zero.");

    REQUIRE((list *= 1) == List<int>({1, 2}));
    REQUIRE((list *= 2) == List<int>({1, 2, 1, 2}));
    REQUIRE((list *= 0) == List<int>({}));
}

// operator/=()
TEST_CASE("List: remove_all_element")
{
    List<int> list = {1, 2, 3, 4, 5, 4, 3, 2, 1};

    REQUIRE((list /= 1) == List<int>({2, 3, 4, 5, 4, 3, 2}));
    REQUIRE((list /= 2) == List<int>({3, 4, 5, 4, 3}));
    REQUIRE((list /= 3) == List<int>({4, 5, 4}));
    REQUIRE((list /= 4) == List<int>({5}));
    REQUIRE((list /= 5) == List<int>({}));

    // if the list does not contain the element, it is unchanged.
    REQUIRE((list /= 6) == List<int>({}));
}

// clear()
TEST_CASE("List: clear")
{
    List<int> list = {1, 2, 3, 4, 5};

    REQUIRE(list.clear() == List<int>());

    // double clear
    REQUIRE(list.clear() == List<int>());

    // modify after clear
    list += 233;
    REQUIRE(list == List<int>({233}));
}

// map()
TEST_CASE("List: map")
{
    List<int> list = {1, 2, 3, 4, 5};

    list.map([](int& x)
             { x *= 2; });
    REQUIRE(list == List<int>({2, 4, 6, 8, 10}));

    list.map([](int& x)
             { x = 1; });
    REQUIRE(list == List<int>({1, 1, 1, 1, 1}));

    std::string str;
    list.map([&](int& x)
             { str += std::to_string(x) + " "; });
    REQUIRE(str == "1 1 1 1 1 ");
}

// filter()
TEST_CASE("List: filter")
{
    List<int> list = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    list.filter([](int& x)
                { return x % 2 == 0; });
    REQUIRE(list == List<int>({2, 4, 6, 8}));

    list.filter([](int& x)
                { return x % 2 == 1; });
    REQUIRE(list == List<int>());
}

// reverse()
TEST_CASE("List: reverse")
{
    REQUIRE(List<int>().reverse() == List<int>());

    REQUIRE(List<int>({1, 2, 3, 4, 5}).reverse() == List<int>({5, 4, 3, 2, 1}));
}

// uniquify()
TEST_CASE("List: uniquify")
{
    REQUIRE(List<int>({1, 2, 2, 3, 3, 3}).uniquify() == List<int>({1, 2, 3}));

    REQUIRE(List<int>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}).uniquify() == List<int>({0}));

    REQUIRE(List<int>({1, 2, 3, 1, 2, 3, 1, 2, 3}).uniquify() == List<int>({1, 2, 3}));

    List<int> many;
    for (int i = 0; i < 10000; i++)
    {
        many += 0;
    }
    REQUIRE(many.uniquify() == List<int>({0}));
}

// for TEST_CASE("List: sort")
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
TEST_CASE("List: sort")
{
    List<int> list = {1, 3, 5, 7, 9, 8, 6, 4, 2, 0};

    // from small to large
    list.sort();
    REQUIRE(list == List<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

    // from large to small
    list.sort([](const int& e1, const int& e2)
              { return e1 > e2; });
    REQUIRE(list == List<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));

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
    REQUIRE(persons == List<Person>({{"Mei", 17},
                                     {"Alice", 18},
                                     {"Yuzu", 18},
                                     {"Sakura", 19},
                                     {"Homura", 20}}));

    // sort by name
    persons.sort([](const Person& e1, const Person& e2)
                 { return e1.name < e2.name; });
    REQUIRE(persons == List<Person>({{"Alice", 18},
                                     {"Homura", 20},
                                     {"Mei", 17},
                                     {"Sakura", 19},
                                     {"Yuzu", 18}}));
}

// swap()
TEST_CASE("List: swap")
{
    List<int> list1({1, 2, 3});
    List<int> list2({4, 5, 6});

    REQUIRE(list1.swap(list2) == List<int>({4, 5, 6}));

    REQUIRE(list1 == List<int>({4, 5, 6}));
    REQUIRE(list2 == List<int>({1, 2, 3}));
}

// adjust_capacity()
TEST_CASE("List: adjust_capacity")
{
    List<int> list;

    REQUIRE(list.capacity() == List<int>::INIT_CAPACITY);
    REQUIRE(list.adjust_capacity(list.capacity() * 2).capacity() == 8);
    REQUIRE(list.adjust_capacity(list.capacity() * 2).capacity() == 16);
    REQUIRE(list.adjust_capacity(1).capacity() == 1);
    REQUIRE(list.adjust_capacity(list.capacity() * 2).capacity() == 2);
    REQUIRE(list.adjust_capacity(list.capacity() * 2).capacity() == 4);

    list += List<int>({1, 2, 3, 4, 5});
    REQUIRE(list.capacity() == 5);

    MY_ASSERT_THROW_MESSAGE(list.adjust_capacity(0), std::runtime_error, "Error: Capacity can not be zero.");

    MY_ASSERT_THROW_MESSAGE(list.adjust_capacity(2), std::runtime_error, "Error: Capacity can not be smaller than the size.");

    MY_ASSERT_THROW_MESSAGE(list.adjust_capacity(INT_MAX), std::runtime_error, "Error: Capacity can not be larger than the maximum capacity.");
}

// operator>>=() operator<<=()
TEST_CASE("List: shift")
{
    REQUIRE((List<int>({1, 2, 3, 4, 5}) >>= -1) == List<int>({2, 3, 4, 5, 1}));
    REQUIRE((List<int>({1, 2, 3, 4, 5}) >>= 0) == List<int>({1, 2, 3, 4, 5}));
    REQUIRE((List<int>({1, 2, 3, 4, 5}) >>= 1) == List<int>({5, 1, 2, 3, 4}));
    REQUIRE((List<int>({1, 2, 3, 4, 5}) >>= 3) == List<int>({3, 4, 5, 1, 2}));
    REQUIRE((List<int>({1, 2, 3, 4, 5}) >>= 5) == List<int>({1, 2, 3, 4, 5}));

    REQUIRE((List<int>({1, 2, 3, 4, 5}) <<= -1) == List<int>({5, 1, 2, 3, 4}));
    REQUIRE((List<int>({1, 2, 3, 4, 5}) <<= 0) == List<int>({1, 2, 3, 4, 5}));
    REQUIRE((List<int>({1, 2, 3, 4, 5}) <<= 1) == List<int>({2, 3, 4, 5, 1}));
    REQUIRE((List<int>({1, 2, 3, 4, 5}) <<= 3) == List<int>({4, 5, 1, 2, 3}));
    REQUIRE((List<int>({1, 2, 3, 4, 5}) <<= 5) == List<int>({1, 2, 3, 4, 5}));
}

// erase()
TEST_CASE("List: erase")
{
    REQUIRE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(0, 1) == List<int>({2, 3, 4, 5, 6, 7}));
    REQUIRE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(1, 2) == List<int>({1, 3, 4, 5, 6, 7}));
    REQUIRE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(1, 6) == List<int>({1, 7}));
    REQUIRE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(0, 7) == List<int>());

    MY_ASSERT_THROW_MESSAGE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(-1, 99), std::runtime_error, "Error: Index out of range.");
}

// slice()
TEST_CASE("List: slice")
{
    List<int> list = {1, 2, 3, 4, 5};

    REQUIRE(list.slice(-1, 1) == List<int>({}));
    REQUIRE(list.slice(-1, 1, -1) == List<int>({5, 4, 3}));
    REQUIRE(list.slice(1, -1) == List<int>({2, 3, 4}));
    REQUIRE(list.slice(1, -1, -1) == List<int>({}));

    REQUIRE(list.slice(0, 5) == List<int>({1, 2, 3, 4, 5}));
    REQUIRE(list.slice(0, 5, 2) == List<int>({1, 3, 5}));
    REQUIRE(list.slice(0, 5, -1) == List<int>({}));
    REQUIRE(list.slice(0, 5, -2) == List<int>({}));

    REQUIRE(list.slice(-1, -6) == List<int>({}));
    REQUIRE(list.slice(-1, -6, 2) == List<int>({}));
    REQUIRE(list.slice(-1, -6, -1) == List<int>({5, 4, 3, 2, 1}));
    REQUIRE(list.slice(-1, -6, -2) == List<int>({5, 3, 1}));

    REQUIRE(list.slice(0, 0) == List<int>({}));
    REQUIRE(list.slice(1, 1) == List<int>({}));
    REQUIRE(list.slice(-1, -1) == List<int>({}));
    REQUIRE(list.slice(-1, -1, -1) == List<int>({}));

    MY_ASSERT_THROW_MESSAGE(list.slice(1, 2, 0), std::runtime_error, "Error: Slice step can not be zero.");

    MY_ASSERT_THROW_MESSAGE(list.slice(-7, -6), std::runtime_error, "Error: Index out of range.");
}

// operator+() operator-() operator*() operator/()
TEST_CASE("List: production")
{
    List<int> list = {1, 2, 3, 4, 5};

    // operator+
    REQUIRE(list + 6 == List<int>({1, 2, 3, 4, 5, 6}));
    REQUIRE(list + List<int>({6, 7}) == List<int>({1, 2, 3, 4, 5, 6, 7}));

    // operator-
    REQUIRE(list - 5 == List<int>({1, 2, 3, 4}));
    REQUIRE(list - 6 == List<int>({1, 2, 3, 4, 5}));

    // operator*
    REQUIRE(list * 0 == List<int>({}));
    REQUIRE(list * 1 == List<int>({1, 2, 3, 4, 5}));
    REQUIRE(list * 2 == List<int>({1, 2, 3, 4, 5, 1, 2, 3, 4, 5}));
    REQUIRE(0 * list == List<int>({}));
    REQUIRE(1 * list == List<int>({1, 2, 3, 4, 5}));
    REQUIRE(2 * list == List<int>({1, 2, 3, 4, 5, 1, 2, 3, 4, 5}));

    // operator/
    REQUIRE(list / 5 == List<int>({1, 2, 3, 4}));
    REQUIRE(list / 6 == List<int>({1, 2, 3, 4, 5}));
}

// to_deque() to_set()
TEST_CASE("List: to_deque_set")
{
    REQUIRE(List<int>({1, 2, 3, 4, 5}).to_deque() == Deque<int>({1, 2, 3, 4, 5}));

    REQUIRE(List<int>({1, 2, 3, 4, 5}).to_set() == Set<int>({1, 2, 3, 4, 5}));
}

// operator<<()
TEST_CASE("List: print")
{
    std::ostringstream oss;

    List<int> empty;
    oss << empty;
    REQUIRE(oss.str() == "[]");
    oss.str("");

    List<int> one = {1};
    oss << one;
    REQUIRE(oss.str() == "[1]");
    oss.str("");

    List<int> many = {1, 2, 3, 4, 5};
    oss << many;
    REQUIRE(oss.str() == "[1, 2, 3, 4, 5]");
    oss.str("");
}
