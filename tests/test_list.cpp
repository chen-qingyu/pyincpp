#include "../sources/List.hpp"

#include "tool.hpp"

using namespace pyincpp;

// for SECTION("sort")
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

    friend std::ostream& operator<<(std::ostream& os, const Person& person)
    {
        return os << "(" << person.name << ", " << person.age << ")";
    }
};

TEST_CASE("List")
{
    // constructor destructor size() is_empty()
    SECTION("basics")
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

    List<int> empty;
    List<int> one = {1};
    List<int> some = {1, 2, 3, 4, 5};

    // operator==() operator!=() operator<() operator<=() operator>() operator>=()
    SECTION("compare")
    {
        // operator==
        List<int> eq_list({1, 2, 3, 4, 5});
        REQUIRE(eq_list == some);

        // operator!=
        List<int> ne_list({1, 3, 5});
        REQUIRE(ne_list != some);

        // operator<
        List<int> lt_list({1, 2, 3});
        List<int> lt_list2({0, 9, 9, 9, 9});
        REQUIRE(lt_list < some);
        REQUIRE(lt_list2 < some);

        // operator<=
        REQUIRE(lt_list <= some);
        REQUIRE(lt_list2 <= some);
        REQUIRE(eq_list <= some);

        // operator>
        List<int> gt_list({1, 2, 3, 4, 5, 6});
        List<int> gt_list2({2});
        REQUIRE(gt_list > some);
        REQUIRE(gt_list2 > some);

        // operator>=
        REQUIRE(gt_list >= some);
        REQUIRE(gt_list2 >= some);
        REQUIRE(eq_list >= some);
    }

    // operator=()
    SECTION("copy_assignment")
    {
        some = one;
        REQUIRE(some == List<int>({1}));
        REQUIRE(one == List<int>({1}));
    }

    // operator=()
    SECTION("move_assignment")
    {
        some = std::move(one);
        REQUIRE(some == List<int>({1}));
        REQUIRE(one == List<int>());
    }

    // operator[]()
    SECTION("access")
    {
        // forward
        for (int i = 0; i < some.size(); ++i)
        {
            REQUIRE(some[i] == i + 1);
        }

        // backward
        for (int i = -1; i >= -some.size(); --i)
        {
            REQUIRE(some[i] == i + 6);
        }

        // assignment
        some[0] = 0;
        REQUIRE(some[0] == 0);

        some[-1] = 999;
        REQUIRE(some[-1] == 999);

        // check bounds
        REQUIRE_THROWS_MATCHES(some[5], std::runtime_error, Message("Error: Index out of range."));
    }

    // begin() end()
    SECTION("iterator")
    {
        // empty
        REQUIRE(empty.begin() == empty.end());

        // for
        int i = 1;
        for (auto it = some.begin(); it != some.end(); ++it)
        {
            REQUIRE(*it == i++);
        }

        // for in
        i = 1;
        for (const auto& e : some)
        {
            REQUIRE(e == i++);
        }
    }

    // find() contains() min() max() count()
    SECTION("examination")
    {
        // find
        REQUIRE(some.find(1) == 0);
        REQUIRE(some.find(5) == 4);
        REQUIRE(some.find(0) == -1);
        REQUIRE(some.find(1, 1, 99) == -1);
        REQUIRE(some.find(5, 1, 99) == 4);
        REQUIRE(some.find(0, 1, 99) == -1);

        // contains
        REQUIRE(some.contains(1) == true);
        REQUIRE(some.contains(5) == true);
        REQUIRE(some.contains(0) == false);
        REQUIRE(some.contains(1, 1, 99) == false);
        REQUIRE(some.contains(5, 1, 99) == true);
        REQUIRE(some.contains(0, 1, 99) == false);

        // min
        REQUIRE(some.min() == 1);

        // max
        REQUIRE(some.max() == 5);

        // count
        REQUIRE(some.count(0) == 0);
        REQUIRE(some.count(1) == 1);
    }

    // insert()
    SECTION("insert")
    {
        // check bounds
        REQUIRE_THROWS_MATCHES(empty.insert(999, 0), std::runtime_error, Message("Error: Index out of range."));

        // insert
        empty.insert(0, 233);
        REQUIRE(empty == List<int>({233}));
        empty.insert(0, 1);
        REQUIRE(empty == List<int>({1, 233}));
        empty.insert(2, 999);
        REQUIRE(empty == List<int>({1, 233, 999}));
        empty.insert(1, 5);
        REQUIRE(empty == List<int>({1, 5, 233, 999}));
        empty.insert(-1, -1); // expand capacity
        REQUIRE(empty == List<int>({1, 5, 233, -1, 999}));

        // check full
        // The test was successful! But the testing time is too long, and comment it out.
        // List<bool> big_list;
        // for (int i = 0; i < List<bool>::MAX_CAPACITY; i++)
        // {
        //     big_list.insert(big_list.size(), true);
        // }
        // REQUIRE(big_list.size() == List<bool>::MAX_CAPACITY);
        // REQUIRE_THROWS_MATCHES(big_list.insert(big_list.size(), true), std::runtime_error, Message("Error: The container has reached the maximum size."));

        // modify after inserted
        List<std::string> str_list;
        std::string str = "test string";
        str_list.insert(0, str);
        str.append(" changed");
        REQUIRE(str == "test string changed");
        REQUIRE(str_list[0] == "test string");
    }

    // remove()
    SECTION("remove")
    {
        // check bounds
        REQUIRE_THROWS_MATCHES(some.remove(999), std::runtime_error, Message("Error: Index out of range."));

        // remove
        REQUIRE(some.remove(-2) == 4);
        REQUIRE(some.remove(1) == 2);
        REQUIRE(some.remove(0) == 1);
        REQUIRE(some.remove(0) == 3);
        REQUIRE(some.remove(0) == 5);

        // check empty
        REQUIRE_THROWS_MATCHES(some.remove(0), std::runtime_error, Message("Error: The container is empty."));
    }

    // operator+=()
    SECTION("append")
    {
        // append element
        REQUIRE((empty += 2) == List<int>({2}));
        REQUIRE((empty += 3) == List<int>({2, 3}));
        REQUIRE((empty += 3) == List<int>({2, 3, 3}));
        REQUIRE((empty += 3) == List<int>({2, 3, 3, 3}));
        REQUIRE((empty += 3) == List<int>({2, 3, 3, 3, 3}));

        // append list
        REQUIRE((empty += empty) == List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3}));
        REQUIRE((empty += empty) == List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3}));
        REQUIRE((empty += List<int>({0, 0})) == List<int>({2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 0, 0}));
    }

    // operator-=()
    SECTION("remove_element")
    {
        REQUIRE((some -= 1) == List<int>({2, 3, 4, 5}));
        REQUIRE((some -= 2) == List<int>({3, 4, 5}));
        REQUIRE((some -= 3) == List<int>({4, 5}));
        REQUIRE((some -= 4) == List<int>({5}));
        REQUIRE((some -= 5) == List<int>({}));

        // if the list does not contain the element, it is unchanged.
        REQUIRE((some -= 6) == List<int>({}));
    }

    // operator*=()
    SECTION("repeat")
    {
        REQUIRE_THROWS_MATCHES(some *= -1, std::runtime_error, Message("Error: Times to repeat can not be less than zero."));

        REQUIRE((some *= 1) == List<int>({1, 2, 3, 4, 5}));
        REQUIRE((some *= 2) == List<int>({1, 2, 3, 4, 5, 1, 2, 3, 4, 5}));
        REQUIRE((some *= 0) == List<int>({}));
    }

    // operator/=()
    SECTION("remove_all_element")
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
    SECTION("clear")
    {
        REQUIRE(some.clear() == List<int>());

        // double clear
        REQUIRE(some.clear() == List<int>());

        // modify after clear
        some += 233;
        REQUIRE(some == List<int>({233}));
    }

    // map()
    SECTION("map")
    {
        some.map([](int& x)
                 { x *= 2; });
        REQUIRE(some == List<int>({2, 4, 6, 8, 10}));

        some.map([](int& x)
                 { x = 1; });
        REQUIRE(some == List<int>({1, 1, 1, 1, 1}));

        std::string str;
        some.map([&](int& x)
                 { str += std::to_string(x) + " "; });
        REQUIRE(str == "1 1 1 1 1 ");
    }

    // filter()
    SECTION("filter")
    {
        some.filter([](int& x)
                    { return x % 2 == 0; });
        REQUIRE(some == List<int>({2, 4}));

        some.filter([](int& x)
                    { return x % 2 == 1; });
        REQUIRE(some == List<int>());
    }

    // reverse()
    SECTION("reverse")
    {
        REQUIRE(empty.reverse() == empty);
        REQUIRE(one.reverse() == one);
        REQUIRE(some.reverse() == List<int>({5, 4, 3, 2, 1}));
    }

    // uniquify()
    SECTION("uniquify")
    {
        REQUIRE(List<int>({1, 2, 2, 3, 3, 3}).uniquify() == List<int>({1, 2, 3}));
        REQUIRE(List<int>({1, 2, 3, 1, 2, 3, 1, 2, 3}).uniquify() == List<int>({1, 2, 3}));

        List<int> many;
        for (int i = 0; i < 10000; i++)
        {
            many += 0;
        }
        REQUIRE(many.uniquify() == List<int>({0}));
    }

    // sort()
    SECTION("sort")
    {
        List<int> list = {1, 3, 5, 7, 9, 8, 6, 4, 2, 0};

        // from small to large
        list.sort();
        REQUIRE(list == List<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

        // from large to small
        list.sort([](const int& e1, const int& e2)
                  { return e1 > e2; });
        REQUIRE(list == List<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));

        List<Person> persons = {{"Alice", 18},
                                {"Sakura", 19},
                                {"Homura", 20},
                                {"Mei", 17},
                                {"Yuzu", 18}};

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
    SECTION("swap")
    {
        REQUIRE(one.swap(some) == List<int>({1, 2, 3, 4, 5}));

        REQUIRE(one == List<int>({1, 2, 3, 4, 5}));
        REQUIRE(some == List<int>({1}));
    }

    // adjust_capacity()
    SECTION("adjust_capacity")
    {
        REQUIRE(empty.capacity() == List<int>::INIT_CAPACITY);
        REQUIRE(empty.adjust_capacity(empty.capacity() * 2).capacity() == 8);
        REQUIRE(empty.adjust_capacity(empty.capacity() * 2).capacity() == 16);
        REQUIRE(empty.adjust_capacity(1).capacity() == 1);
        REQUIRE(empty.adjust_capacity(empty.capacity() * 2).capacity() == 2);
        REQUIRE(empty.adjust_capacity(empty.capacity() * 2).capacity() == 4);

        empty += List<int>({1, 2, 3, 4, 5});
        REQUIRE(empty.capacity() == 5);

        REQUIRE_THROWS_MATCHES(empty.adjust_capacity(0), std::runtime_error, Message("Error: Capacity can not be zero."));

        REQUIRE_THROWS_MATCHES(empty.adjust_capacity(2), std::runtime_error, Message("Error: Capacity can not be smaller than the size."));

        REQUIRE_THROWS_MATCHES(empty.adjust_capacity(INT_MAX), std::runtime_error, Message("Error: Capacity can not be larger than the maximum capacity."));
    }

    // operator>>=() operator<<=()
    SECTION("shift")
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
    SECTION("erase")
    {
        REQUIRE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(0, 1) == List<int>({2, 3, 4, 5, 6, 7}));
        REQUIRE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(1, 2) == List<int>({1, 3, 4, 5, 6, 7}));
        REQUIRE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(1, 6) == List<int>({1, 7}));
        REQUIRE(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(0, 7) == List<int>());

        REQUIRE_THROWS_MATCHES(List<int>({1, 2, 3, 4, 5, 6, 7}).erase(-1, 99), std::runtime_error, Message("Error: Index out of range."));
    }

    // slice()
    SECTION("slice")
    {
        REQUIRE(some.slice(-1, 1) == List<int>({}));
        REQUIRE(some.slice(-1, 1, -1) == List<int>({5, 4, 3}));
        REQUIRE(some.slice(1, -1) == List<int>({2, 3, 4}));
        REQUIRE(some.slice(1, -1, -1) == List<int>({}));

        REQUIRE(some.slice(0, 5) == List<int>({1, 2, 3, 4, 5}));
        REQUIRE(some.slice(0, 5, 2) == List<int>({1, 3, 5}));
        REQUIRE(some.slice(0, 5, -1) == List<int>({}));
        REQUIRE(some.slice(0, 5, -2) == List<int>({}));

        REQUIRE(some.slice(-1, -6) == List<int>({}));
        REQUIRE(some.slice(-1, -6, 2) == List<int>({}));
        REQUIRE(some.slice(-1, -6, -1) == List<int>({5, 4, 3, 2, 1}));
        REQUIRE(some.slice(-1, -6, -2) == List<int>({5, 3, 1}));

        REQUIRE(some.slice(0, 0) == List<int>({}));
        REQUIRE(some.slice(1, 1) == List<int>({}));
        REQUIRE(some.slice(-1, -1) == List<int>({}));
        REQUIRE(some.slice(-1, -1, -1) == List<int>({}));

        REQUIRE_THROWS_MATCHES(some.slice(1, 2, 0), std::runtime_error, Message("Error: Slice step can not be zero."));

        REQUIRE_THROWS_MATCHES(some.slice(-7, -6), std::runtime_error, Message("Error: Index out of range."));
    }

    // operator+() operator-() operator*() operator/()
    SECTION("production")
    {
        // operator+
        REQUIRE(some + 6 == List<int>({1, 2, 3, 4, 5, 6}));
        REQUIRE(some + List<int>({6, 7}) == List<int>({1, 2, 3, 4, 5, 6, 7}));

        // operator-
        REQUIRE(some - 5 == List<int>({1, 2, 3, 4}));
        REQUIRE(some - 6 == List<int>({1, 2, 3, 4, 5}));

        // operator*
        REQUIRE(some * 0 == List<int>({}));
        REQUIRE(some * 1 == List<int>({1, 2, 3, 4, 5}));
        REQUIRE(some * 2 == List<int>({1, 2, 3, 4, 5, 1, 2, 3, 4, 5}));
        REQUIRE(0 * some == List<int>({}));
        REQUIRE(1 * some == List<int>({1, 2, 3, 4, 5}));
        REQUIRE(2 * some == List<int>({1, 2, 3, 4, 5, 1, 2, 3, 4, 5}));

        // operator/
        REQUIRE(some / 5 == List<int>({1, 2, 3, 4}));
        REQUIRE(some / 6 == List<int>({1, 2, 3, 4, 5}));
    }

    // to_deque() to_set()
    SECTION("to_deque_set")
    {
        REQUIRE(List<int>({1, 2, 3, 4, 5}).to_deque() == Deque<int>({1, 2, 3, 4, 5}));

        REQUIRE(List<int>({1, 2, 3, 4, 5}).to_set() == Set<int>({1, 2, 3, 4, 5}));
    }

    // operator<<()
    SECTION("print")
    {
        std::ostringstream oss;

        oss << empty;
        REQUIRE(oss.str() == "[]");
        oss.str("");

        oss << one;
        REQUIRE(oss.str() == "[1]");
        oss.str("");

        oss << some;
        REQUIRE(oss.str() == "[1, 2, 3, 4, 5]");
        oss.str("");
    }
}
