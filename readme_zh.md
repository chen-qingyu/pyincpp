# PyInCpp

_像 Python 的内置类型一样好用的 C++ 库_

[English](./readme.md)

### 1. 属性

- 名称：PyInCpp。
- 语言：C++ ，要求 C++20 。
- 目标：实现一个像 Python 的内置类型一样好用的 C++ 库。
- 模块：List, Set, Dict, Int, Str, Tuple, Deque, Fraction.
- 风格：大部分遵循 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) ，小部分基于项目规模和源码简洁性的考虑采用自己的风格。
- 测试：使用 [Catch2](https://github.com/catchorg/Catch2) 进行了单元测试和基准测试，确保测试全部通过。
- 安全：使用 [Dr. Memory](https://drmemory.org/) 进行了检查，确保没有安全问题。
- 文档：使用 [Doxygen](https://www.doxygen.nl/) 生成文档。
- 构建：使用 [XMake](https://xmake.io/) 进行构建。

### 2. 特点

- 简洁：Stay simple, stay young. 在保证好用和健壮的前提下，尽量简洁，便于维护和阅读。
- 好用：经过我的精心设计，用起来可以像 Python 的内置类型一样方便。很 Pythonic。
- 健壮：对容器的增删改查都有相应的检查。
- 高效：[基准测试结果](./benches/std_vs_pyincpp.cpp)表明功能与标准库相同的部分的性能几乎一样。

### 3. 用法

因为用的是 C++ 模板，所以全部以头文件的形式（.hpp）给出，header-only。

使用非常方便：

- PyInCpp 已经进入 XMake 官方仓库，所以只需要在 xmake.lua 中加上 `add_requires("pyincpp")` 然后源码中就可以 `#include <pyincpp.hpp>`。
- 或者，直接从 releases 里面下载合并后的头文件然后 `#include "pyincpp_amalgamated.hpp"`。

一共九个类，对标 Python 里面常用的类：

| Type in PyInCpp | Type in Python       |
| --------------- | -------------------- |
| `List<T>`       | `list`               |
| `Set<T>`        | `set`                |
| `Dict<K, V>`    | `dict`               |
| `Int`           | `int`                |
| `Str`           | `str`                |
| `Tuple<Ts...>`  | `tuple`              |
| `Complex`       | `complex`            |
| `Deque<T>`      | `collections.deque`  |
| `Fraction`      | `fractions.Fraction` |

一些简单的例子：

```cpp
using namespace pyincpp;

// List support negative index
List<int>{1, 2, 3, 4, 5}[-1]; // 5
// List traversal
List<int>{1, 2, 3, 4, 5}.map([](int& x) { x *= 2; }); // [2, 4, 6, 8, 10]
// List uniquify
List<int>{1, 2, 3, 1, 2, 3, 1, 2, 3}.uniquify(); // [1, 2, 3]
// List sort, stable sort, default from small to large, customizable comparator
List<int>{1, 2, 3, 4, 5}.sort([](const int& e1, const int& e2) { return e1 > e2; }); // [5, 4, 3, 2, 1]

// test whether a Set is proper subset of another Set
Set<int>{5, 1} < Set<int>{1, 2, 3, 4, 5}; // true
// intersection of Sets, support intersection, union, difference, and symmetric difference
Set<int>{1, 2, 3, 4, 5} & Set<int>{1, 3, 5, 7, 9}; // {1, 3, 5}

// Dict access
Dict<Str, int>{{"one", 1}, {"two", 2}, {"three", 3}}["one"]; // 1
// Dict get values
Dict<Str, int>{{"one", 1}, {"two", 2}, {"three", 3}}.values(); // {1, 2, 3}

// Int basic operation, support +, -, *, /, % and compare
Int("18446744073709551617") + Int("18446744073709551617"); // 36893488147419103234
// Int increment, after my optimization, much faster than `+= 1`
++Int("99999999999999"); // 100000000000000
// Int modular power, very fast
Int::pow("1024", "1024", "100"); // 76
// Int factorial
Int("5").factorial().factorial(); // 668950291344912705758811805409037258675274633313802981029567135...
// get random Int, using hardware device to generate true random integer if possible
Int::random(); // 2379575921434838751469952249632783251093957333629022509960142131113090672699644921...
// calculate the next prime that greater than this
Int(0).next_prime(); // 2

// convert Str to floating-point number, support inf and nan
Str(".1e-2").to_decimal(); // 0.1e-2
// convert Str to integer, support base 2-36
Str("-ffffffffffffffff").to_integer(16); // -18446744073709551615
// Str repeat
Str("hello! ") * 2; // "hello! hello! "
// Str replace
Str("hahaha").replace("a", "ooow~ "); // "hooow~ hooow~ hooow~ "
// Str slice
Str("12345").slice(0, 5, 2); // "135"
// Str split
Str("one, two, three").split(", "); // ["one", "two", "three"]
// Str join
Str(".").join({"192", "168", "0", "1"}); // "192.168.0.1"
// Str format
Str("I'm {}, {} years old.").format("Alice", 18); // "I'm Alice, 18 years old."

// Tuple index, return type different, so template function is used
Tuple<int, double, char>(1, 2.5, 'A').get<2>(); // 'A'
// take the remaining part of the Tuple, the underlying layer is pointer conversion, which is very fast
Tuple<int, double, char>(1, 2.5, 'A').rest(); // (2.5, 'A')

// Complex addition
Complex(1, 2) + Complex(1, 3); // (2+5j)
// Complex power
Complex::pow(Complex(1, 2), Complex(-1, 2)); // (0.04281551979798478+0.023517649351954585j)

// Deque element reference
Deque<int>{1, 2, 3, 4, 5}.front(); // 1
// Deque rotate to right (or left), very vivid!
Deque<int>{1, 2, 3, 4, 5} >>= 1; // <5, 1, 2, 3, 4>

// Fraction addition
Fraction(1, 2) + Fraction(1, 3); // 5/6
// Fraction modulo
Fraction(1, 2) % Fraction(1, 3); // 1/6
```

### 4. 优势

PyInCpp 的优势在于把 C++ 的高性能和 Python 的易用性结合起来了，还可以方便地与其他库结合使用，比如：

```cpp
// 1. All types can be printed and easily combined:
Dict<Str, List<Int>> dict = {{"first", {"123", "456"}}, {"second", {"789"}}, {"third", {"12345678987654321", "5"}}};
std::cout << dict; // {"first": [123, 456], "second": [789], "third": [12345678987654321, 5]}
dict.keys(); // {"first", "second", "third"}
dict["third"][-1].factorial(); // 120

// 2. All container types are iterable:
for (const auto& [k, v] : Dict<int, int>{{1, 1}, {2, 4}, {3, 9}})
{
    assert(k * k == v);
}

// 3. All immutable types are hashable:
std::unordered_set<Int> set1 = {"1", "2", "3", "18446744073709551617"};
std::unordered_set<Str> set2 = {"hello", "pyincpp"};
std::unordered_set<Fraction> set3 = {{1, 2}, {3, 4}};

// 4. Combining pyincpp::Fraction with Eigen library to display accurate matrix.
using Matrix = Eigen::Matrix<pyincpp::Fraction, 2, 2>; // compiling with boost::rational will fail

Matrix A = Matrix{{1, 2}, {3, 4}};
Matrix B = Matrix::Zero();
Matrix C = Matrix::Ones();
Matrix D = Matrix::Identity();

std::cout << ((A + B) * (C + D)).inverse() << std::endl;
/*
-11/6     5/6
  5/3    -2/3
*/

// 5. boost::rational vs pyincpp::Fraction
boost::rational<int> r1(1, 2), r2(1, 3), r3(1, 4), r4(1, 5);
pyincpp::Fraction f1(1, 2), f2(1, 3), f3(1, 4), f4(1, 5);

std::cout << ((r1 + r2) * r3 / r4) << std::endl; // 25/24
// std::cout << ((r1 + r2) * r3 % r4) << std::endl; // boost::rational does not support operator%
std::cout << ((f1 + f2) * f3 / f4) << std::endl; // 25/24
std::cout << ((f1 + f2) * f3 % f4) << std::endl; // 1/120

// 6. std::tuple vs boost::tuple vs pyincpp::Tuple
auto t1 = std::make_tuple(1, 1.5, 'A', "hello", std::tuple<std::tuple<>, std::tuple<>>({}, {}));
auto t2 = boost::make_tuple(1, 1.5, 'A', "hello", boost::tuple<boost::tuple<>, boost::tuple<>>({}, {}));
auto t3 = pyincpp::make_tuple(1, 1.5, 'A', "hello", pyincpp::Tuple<pyincpp::Tuple<>, pyincpp::Tuple<>>({}, {}));

// std::cout << t1 << std::endl; // std::tuple does not support operator<<
std::cout << t2 << std::endl; // (1 1.5 A hello (() ()))
std::cout << t3 << std::endl; // (1, 1.5, A, hello, ((), ()))
```

如果您想在 Rust 中使用类似的库，请参阅：[PyInRs](https://github.com/chen-qingyu/pyinrs).

### 5. 历史

最开始是用的 C 语言开发，目的是学习数据结构。然后在 2021 年开始尝试用 C++ 实现一些方便的容器类，只当是练手。后来经过了几次重构，确定了目标。将最初的版本迁移至了[HelloDS](https://github.com/chen-qingyu/hellods)，目标是在不使用任何标准库容器的情况下实现完整、通用的基础容器类。而这个项目的目标是实现与 Python 类似的基本类型，并尽可能方便和高效。这个项目于 2023 年 1 月 1 日开始正式开发。

说明一下关于 inline：为了源码简洁性，我最后决定一律采用 inline 的风格。一般不会有问题，除非对程序体积有很高的要求。刚开始我是把声明和定义分开写的，但这是模板，没法分成两个文件，所以我在一个文件里分开写，一部分函数定义前面加上 inline，但是这样最后写完了看起来非常冗长，一大堆的 "template typename inline"，在看了 Java 源码后考虑再三决定全部写在类里面，也就是默认 inline 的形式。inline 只是对编译器的请求而非要求，不能 inline 的函数（比如有递归的函数）编译器是不会执行 inline 的。

每行代码都经过了我的精心优化。拿 pyincpp::Int 和 GitHub 上有三百多 star 的 [BigInt](https://github.com/faheel/BigInt) 做个[比较](./benches/pyincpp_int_vs_other_int.cpp)，结论是 pyincpp::Int 的速度快得多，并且功能更丰富，而源码行数不到 BigInt 的一半，代码也更加整洁。

我这个项目用到了 FPGA 里面的独热码思想结合有限状态机，还用到了模板元编程在编译期递归实现任意可变模板参数，听着很厉害，但是不赚钱，也没多少人真的会用，属于自娱自乐，可我创造就是快乐，创造就是意义（反正我不缺钱——饿不死）。
