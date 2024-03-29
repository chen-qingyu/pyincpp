# PyInCpp

_像 Python 的内置类型一样好用的 C++ 库_

[English](./readme.md)

### 1. 属性

- 名称：PyInCpp。
- 语言：C++ ，要求 C++17 。
- 目标：实现一个像 Python 的内置类型一样好用的 C++ 库。
- 模块：List, Set, Map, Integer, String, Tuple, Deque, Fraction.
- 风格：大部分遵循 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) ，小部分基于项目规模和源码简洁性的考虑采用自己的风格。
- 测试：使用 [Catch2](https://github.com/catchorg/Catch2) 进行了单元测试和基准测试，确保测试全部通过。
- 安全：使用 [Dr. Memory](https://drmemory.org/) 进行了检查，确保没有安全问题。
- 文档：使用 [Doxygen](https://www.doxygen.nl/) 生成文档。
- 构建：使用 [XMake](https://xmake.io/) 进行构建。

### 2. 特点

- 简洁：Stay simple, stay young. 在保证好用和健壮的前提下，尽量简洁，便于维护和阅读。
- 好用：提供了许多方便的函数，比如 String 类提供了像 Python 的 str 那样的替换、分割、查找等操作，比如 List 类和 String 类都支持像 Python 那样的负数下标等等。
- 健壮：安全的扩容机制，防止溢出。对容器的增删改查都有相应的检查。检查会对性能有影响，但是这个库追求的并不是性能，而是简洁，好用和健壮。
- 优雅：经过我的精心设计，用起来可以像 Python 的内置类型一样方便。很 Pythonic。
- 高效：和标准库重合的部分进行了性能比较，[基准测试结果](./tests/benchmark.cpp)表明性能并不差。

### 3. 用法

因为用的是 C++ 模板，所以全部以头文件的形式（.hpp）给出，header-only。

使用非常方便：

- PyInCpp 已经进入 XMake 官方仓库，所以只需要在 xmake.lua 中加上 `add_requires("pyincpp")` 然后源码中就可以 `#include <pyincpp/pyincpp.hpp>`。
- 或者，简单粗暴地拷贝整个 sources 目录到项目源码目录下然后 `#include "pyincpp.hpp"`。

目前一共八个类，对标 Python 里面的八个常用的类：

| Type in PyInCpp | Type in Python       |
| --------------- | -------------------- |
| `List<T>`       | `list`               |
| `Set<T>`        | `set`                |
| `Map<K, V>`     | `dict`               |
| `Integer`       | `int`                |
| `String`        | `str`                |
| `Tuple<Ts...>`  | `tuple`              |
| `Deque<T>`      | `collections.deque`  |
| `Fraction`      | `fractions.Fraction` |

一些简单的例子：

```cpp
using namespace pyincpp;

// 列表索引，支持负数下标
List<int>({1, 2, 3, 4, 5})[-1] // 5
// 列表遍历
List<int>({1, 2, 3, 4, 5}).map([](int& x) { x *= 2; }) // [2, 4, 6, 8, 10]
// 列表去重
List<int>({1, 2, 3, 1, 2, 3, 1, 2, 3}).uniquify() // [1, 2, 3]
// 列表排序，稳定排序，默认从小到大，可自定义比较器
List<int>({1, 2, 3, 4, 5, 6, 7, 8, 9}).sort([](const int& e1, const int& e2) { return e1 > e2; }) // [9, 8, 7, 6, 5, 4, 3, 2, 1]

// 集合添加元素
Set<int>({1, 2, 3, 4}) += 5 // {1, 2, 3, 4, 5}
// 集合求交集，交、并、差、对称差（异或）都支持
Set<int>({1, 2, 3, 4, 5}) & Set<int>({1, 3, 5, 7, 9}) // {1, 3, 5}

// 字典根据键赋值
Map<String, int>({{"one", 1}, {"two", 2}, {"three", 3}})["one"] = 1111 // {"one": 1111, "two": 2, "three": 3}
// 字典取值集合
Map<String, int>({{"one", 1}, {"two", 2}, {"three", 3}}).values() // {1, 2, 3}

// 整数幂取模，非常快
Integer("1024").pow("1024", "100") // 76
// 整数求阶乘
Integer("5").factorial().factorial() // 668950291344912705758811805409037258675274633313802981029567135230163355...

// 字符串转浮点数，支持 inf 和 nan
String(".1e-2").to_decimal() // 0.1e-2
// 字符串转整数，支持 2-36 进制
String("-0101").to_integer(2) // -5
// 字符串追加
String("hello") += " world!" // "hello world!"
// 字符串重复
String("hello! ") *= 2 // "hello! hello! "
// 字符串替换
String("hahaha").replace("a", "ooow~ ") // "hooow~ hooow~ hooow~ "
// 字符串切片
String("12345").slice(0, 5, 2) // "135"
// 字符串分割
String("one, two, three").split(", ") // ["one", "two", "three"]
// 字符串合并
String(".").join({"192", "168", "0", "1"}) // "192.168.0.1"
// 字符串格式化
String("I'm {}, {} years old.").format("Alice", 18) // "I'm Alice, 18 years old."

// 元组索引，返回类型不同所以使用模板函数
Tuple<int, double, char>(1, 2.5, 'A').get<2>() // 'A'
// 元组取剩余部分，底层是指针转换，非常快
Tuple<int, double, char>(1, 2.5, 'A').rest() // (2.5, 'A')

// 双端队列尾部入栈，头部、尾部入栈、出栈、引用都支持
Deque<int>({1, 2, 3, 4}).push_back(5) // <1, 2, 3, 4, 5>
// 双端队列向右移位，很生动形象有木有！
Deque<int>({1, 2, 3, 4, 5}) >>= 1 // <5, 1, 2, 3, 4>

// 分数相加
Fraction(1, 2) + Fraction(1, 3) // 5/6
// 分数取模
Fraction(1, 2) % Fraction(1, 3) // 1/6

// 任意嵌套多层类型
Map<String, List<Integer>> map = {{"first", {123, 456}}, {"second", {789}}, {"second", {0}}, {"third", {"12345678987654321", 5}}}
    // {"first": [123, 456], "second": [789], "third": [12345678987654321, 5]}
map.size() // 3
map.keys() // {"first", "second", "third"}
map["third"][-1].factorial() // 120
```

### 4. 优势

PyInCpp 的优势在于把 C++ 的高性能和 Python 的易用性结合起来了，还可以方便地与其他库结合使用，比如：

```cpp
/*
Combining pyincpp::Fraction with Eigen library to display accurate matrix operation results.
*/
using Matrix = Eigen::Matrix<pyincpp::Fraction, 2, 2>; // compiling with boost::rational will fail

Matrix A;
A << 1, 2, 3, 4;
Matrix B = Matrix::Zero();
Matrix C = Matrix::Ones();
Matrix D = Matrix::Identity();

std::cout << (((A + B) * (C + D)).inverse()) << std::endl;
/*
-11/6     5/6
  5/3    -2/3
*/

/*
boost::rational vs pyincpp::Fraction
*/
boost::rational<int> r1(1, 2), r2(1, 3), r3(1, 4), r4(1, 5);
pyincpp::Fraction f1(1, 2), f2(1, 3), f3(1, 4), f4(1, 5);

std::cout << ((r1 + r2) * r3 / r4) << std::endl; // 25/24
// std::cout << ((r1 + r2) * r3 % r4) << std::endl; // boost::rational does not support operator%
std::cout << ((f1 + f2) * f3 / f4) << std::endl; // 25/24
std::cout << ((f1 + f2) * f3 % f4) << std::endl; // 1/120

/*
std::tuple vs boost::tuple vs pyincpp::Tuple
*/
auto t1 = std::make_tuple(1, 1.5, 'A', "hello", std::tuple<std::tuple<>, std::tuple<>>({}, {}));
auto t2 = boost::make_tuple(1, 1.5, 'A', "hello", boost::tuple<boost::tuple<>, boost::tuple<>>({}, {}));
auto t3 = pyincpp::make_tuple(1, 1.5, 'A', "hello", pyincpp::Tuple<pyincpp::Tuple<>, pyincpp::Tuple<>>({}, {}));

// std::cout << t1 << std::endl; // std::tuple does not support operator<<
std::cout << t2 << std::endl; // (1 1.5 A hello (() ()))
std::cout << t3 << std::endl; // (1, 1.5, A, hello, ((), ()))
```

### 5. 历史

最开始是用的 C 语言开发，目的是学习数据结构。然后 2021 年开始尝试用 C++ 实现一些方便的容器类，只当是练手。后来经过了几次重构，确定了目标和目的，2023 年开始正式开发。

说明一下关于 inline：为了源码简洁性，我最后决定一律采用 inline 的风格。一般不会有问题，除非对程序体积有很高的要求。刚开始我是把声明和定义分开写的，但这是模板，没法分成两个文件，所以我在一个文件里分开写，一部分函数定义前面加上 inline，但是这样最后写完了看起来非常冗长，一大堆的 "template typename inline"，在看了 Java 源码后考虑再三决定全部写在类里面，也就是默认 inline 的形式。inline 只是对编译器的请求而非要求，不能 inline 的函数（比如有递归的函数）编译器是不会执行 inline 的。

开发完了 Integer 类后和 GitHub 上一个有三百多 star 的大整数类 [BigInt](https://github.com/faheel/BigInt) 做了比较，结论是 pyincpp::Integer 的性能综合来看更快，同时易用性和 BigInt 差不多，而源码行数只有 BigInt 的几乎一半，并且代码也更加整洁。

我这个项目用到了 FPGA 里面的独热码思想结合有限状态机，还用到了模板元编程在编译期递归实现任意可变模板参数，听着很厉害，但是不赚钱，也没多少人真的会用，属于自娱自乐，可我创造就是快乐，创造就是意义（反正我不缺钱——饿不死）。
