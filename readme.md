# MyDataStructure

_——像 Python 的内置类型一样优雅的 C++ 库_

![](https://img.shields.io/badge/version-1.0-blue.svg)

### 1. 基本属性

- 名称：MyDataStructure，缩写为 MDS。
- 语言：采用标准 C++ 语言编写，最低兼容版本：ISO C++17 。
- 目的：学习 C++ 和项目开发。
- 目标：实现一个像 Python 的内置类型一样优雅的 C++ 库。
- 模块：List, Set, Map, Integer, String, Tuple, Deque.
- 简洁：Stay simple, stay young. 在保证好用和健壮的前提下，尽量简洁，便于维护和阅读。
- 好用：提供了许多方便的函数，比如 String 类提供了像 Python 的 str 那样的替换、分割、查找等操作，比如 List 类和 String 类都支持像 Python 那样的负数下标等等。
- 健壮：安全的扩容机制，防止溢出。对容器的增删改查都有相应的检查。这么多检查，肯定会对性能有一定影响，但是这个库追求的并不是性能，而是简洁，好用和健壮。
- 优雅：经过我的精心设计，用起来可以像 Python 的内置类型一样方便。
- 风格：大部分遵循 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) ，小部分基于项目规模和源码简洁性的考虑采用自己的风格。
- 测试：使用 [GoogleTest](https://github.com/google/googletest) 进行了测试，确保测试全部通过。
- 安全：使用 [Dr. Memory](https://drmemory.org/) 进行了检查，确保没有安全问题。
- 文档：使用 [Doxygen](https://www.doxygen.nl/) 生成文档。
- 构建：使用 [XMake](https://xmake.io/) 进行构建。

### 2. 使用说明

因为用的是 C++ 模板，所以全部以头文件的形式（.hpp）给出。

使用非常方便：直接拷贝整个 sources 目录到工程目录下然后直接`#include "mds.hpp"`就可以了。

一共七个类，对标 Python 里面的七个常用的类：

| Type in MDS    | Type in Python       |
| -------------- | -------------------- |
| `List<T>`      | `list`               |
| `Set<T>`       | `set`                |
| `Map<K, V>`    | `dict`               |
| `Integer`      | `int`                |
| `String`       | `str`                |
| `Tuple<Ts...>` | `tuple`              |
| `Deque<T>`     | `collections.deque`  |
| `Fraction`     | `fractions.Fraction` |

一些简单的例子：

```cpp
using namespace mds;

// 列表索引，支持负数
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
Map<int, String>({{"one", 1}, {"two", 2}, {"three", 3}}).values() // {1, 2, 3}

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

// 元组索引，返回类型不同所以使用模板函数
Tuple<int, double, char>(1, 2.5, 'A').get<2>() // 'A'
// 元组取剩余部分，底层是指针转换，非常快
Tuple<int, double, char>(1, 2.5, 'A').rest() // (2.5, 'A')

// 双端队列尾部入栈，头部、尾部入栈、出栈、引用都支持
Deque<int>({1, 2, 3, 4}).push_back(5) // <1, 2, 3, 4, 5>
// 双端队列向右移位，很生动形象有木有！
Deque<int>({1, 2, 3, 4, 5}) >>= 1 // <5, 1, 2, 3, 4>

// 分数乘法，加、减、乘、除、模都支持
Fraction(1, 2) * Fraction(1, 2) // Fraction(1, 4)
// 分数转字符串
Fraction(3, -6).to_string() // "-1/2"

// 任意嵌套多层容器
Map<String, List<Integer>> map = {{"first", {123, 456}}, {"second", {789}}, {"second", {0}}, {"third", {"12345678987654321", 5}}}
    // {"first": [123, 456], "second": [789], "third": [12345678987654321, 5]}
map.size() // 3
map.keys() // {"first", "second", "third"}
map["third"][-1].factorial() // 120

// 将Fraction与Eigen库结合显示精确的矩阵运算结果
using Matrix = Eigen::Matrix<Fraction, 2, 2>;

Matrix A;
A << 1, 2, 3, 4;
Matrix B = Matrix::Zero();
Matrix C = Matrix::Ones();
Matrix D = Matrix::Identity();

((A + B) * (C + D)).inverse()
/*
-11/6     5/6
  5/3    -2/3
*/
```

### 3. 开发历史

最开始是用的 C 语言开发，目的是学习数据结构。然后 2021 年开始尝试用 C++ 实现一些方便的容器类，只当是练手。后来经过了几次重构，确定了目标和目的，2023 年开始正式开发。

说明一下关于 inline：为了源码简洁性，我最后决定一律采用 inline 的风格。一般不会有问题，除非对程序体积有很高的要求。刚开始我是把声明和定义分开写的，但这是模板，没法分成两个文件，所以我在一个文件里分开写，一部分函数定义前面加上 inline，但是这样最后写完了看起来非常冗长，一大堆的 template typename inline，在看了 Java 源码后考虑再三决定全部写在类里面，也就是默认 inline 的形式。inline 只是对编译器的请求而非要求，不能 inline 的函数（比如有递归的函数）编译器是不会执行 inline 的。

开发完了 Integer 类后和 GitHub 上一个有三百多 star 的大整数类 [BigInt](https://github.com/faheel/BigInt) 做了比较，结论是 mds::Integer 的性能综合来看更快，同时易用性和 BigInt 差不多，而源码行数只有 BigInt 的几乎一半，并且代码也更加整洁。

我这个项目用到了 FPGA 里面的独热码思想结合有限状态机，还用到了模板元编程在编译期递归实现任意可变模板参数，听着很厉害，但是不赚钱，也没多少人真的会用，属于自娱自乐。可我创造就是快乐，创造就是意义（反正我不缺钱——饿不死）。
