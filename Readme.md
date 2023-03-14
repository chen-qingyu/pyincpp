# MineDataStructure++

*——像Python一样好用的C++容器库*

### 1. 基本属性

- 名称：MineDataStructure++，缩写为MDSPP，灵感来源于我喜欢的游戏——Minecraft以及本项目的编程语言——C++。
- 语言：采用标准C++语言编写，最低兼容版本： ISO C++14 。
- 目的：提供一个简洁、好用、健壮的C++容器库。
- 目标：像Python的内置类型一样好用又优雅。
- 模块：List, Set, Map, Integer, String, Tuple, Deque.
- 简洁：Stay simple, stay young. 在保证好用和健壮的前提下，尽量简洁，便于维护和阅读。
- 好用：提供了许多方便的函数，比如String类提供了像Python的str那样的替换、分割、查找等操作，比如List类和String类都支持像Python那样的负数下标等等。
- 健壮：安全的扩容机制，防止溢出。对容器的增删改查都有相应的检查。这么多检查，肯定会对性能有一定影响，但是这个库追求的并不是性能，而是简洁，好用和健壮。
- 优雅：经过我的精心设计，用起来可以像Python的内置类型一样方便。
- 风格：大部分遵循 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) ，小部分基于项目规模和源码简洁性的考虑采用自己的风格。
- 测试：使用 [GoogleTest](https://github.com/google/googletest) 进行了测试，确保测试全部通过。
- 安全：使用 [Dr. Memory](https://drmemory.org/) 进行了检查，确保没有安全问题（比如内存泄漏）。
- 文档：使用 [Doxygen](https://www.doxygen.nl/) 制作了文档。

### 2. 使用说明

| Type          | Implement                   | Python              |
| ------------- | --------------------------- | ------------------- |
| `List<T>`     | `dynamic array`             | `list`              |
| `Set<T>`      | `AVL tree`                  | `set`               |
| `Map<K, V>`   | `Set<Pair<K, V>>`           | `dict`              |
| `Integer`     | `List<signed char>`         | `int`               |
| `String`      | `List<char>`                | `str`               |
| `Tuple<...T>` | `template meta-programming` | `tuple`             |
| `Deque<T>`    | `doubly linked list`        | `collections.deque` |

直接include对应的hpp文件然后`using namespace mdspp;`就可以使用了。

一些简单的例子：

```cpp
String(".1e-2").to_decimal(); // 0.1e-2 支持inf和nan
String("-0101").to_integer(2); // -5 支持2-36进制的整数
String("hello") += " world!"; // "hello world!" 追加
String("hello! ") *= 2; // "hello! hello! " 重复
String("hahaha").replace("a", "ooow~ "); // "hooow~ hooow~ hooow~ " 替换
String("12345").slice(0, 5, 2); // "135" 切片
String("one, two, three").split(", "); // List<String>({"one", "two", "three"}) 分割
String(".").join(List<String>({"192", "168", "0", "1"})); // "192.168.0.1" 合并

List<int>({1, 2, 3, 4, 5})[-1]; // 5 就像Python的负数下标
List<int>({1, 2, 3, 4, 5}).traverse([](int& x) { x *= 2; }); // List<int>({2, 4, 6, 8, 10}) 遍历操作
List<int>({1, 2, 3, 1, 2, 3, 1, 2, 3}).uniquify(); // List<int>({1, 2, 3}) 无序列表快速唯一化
List<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}).sort([](const int& e1, const int& e2) { return e1 > e2; }); // List<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}) 默认从小到大，稳定排序

Deque<int>({1, 2, 3, 4, 5}) >>= 1; // Deque<int>({5, 1, 2, 3, 4}) 很生动形象有木有！

Integer("1024").pow(Integer("1024"), Integer("100")); // 1024^1024 % 100 == 76 快速幂取模，非常快
Integer("5").factorial().factorial(); // (5!)! == 668950291344912705758811805409037258675274633313802981029567135230163355...

Set<int>({1, 2, 3, 4, 5}) & Set<int>({1, 3, 5, 7, 9}); // Set<int>({1, 3, 5}) 集合的交、并、差、对称差（异或）及对应的赋值都支持

Map<String, int>({{"one", 1}, {"two", 2}, {"three", 3}})["one"] = 1111; // Map<String, int>({{"one", 1111}, {"two", 2}, {"three", 3}})
Map<int, String>({{1, "one"}, {2, "two"}, {3, "three"}}).values(); // Set<String>({"one", "two", "three"})

Tuple<int, double, char> tuple(1, 2.5, 'A');
get<0>(tuple); // 1
get<1>(tuple); // 2.5
get<2>(tuple); // 'A'
tuple.rest(); // Tuple<double, char>(2.5, 'A')
```

### 3. 开发历史

最开始是用的C语言开发，目的是学习数据结构。然后2021年开始尝试用C++实现一些方便的容器类，只当是练手。后来经过了几次重构，确定了目标和目的，2023年开始正式开发。

说明一下关于`inline`：为了源码简洁性，我最后决定一律采用inline的风格。一般不会有问题，除非对程序体积有很高的要求。刚开始我是把声明和定义分开写的，但这是模板，没法分成两个文件，所以我在一个文件里分开写，一部分函数定义前面加上inline，但是这样最后写完了看起来非常冗长，一大堆的template typename inline，在看了Java源码后考虑再三决定全部写在类里面，也就是默认inline的形式。inline只是对编译器的请求而非要求，不能inline的函数（比如有递归的函数）编译器是不会执行inline的。

开发完了`Integer`类后和GitHub上一个有三百多star的大整数类`BigInt`做了比较，结论是 mdspp::Integer 的性能综合来看更快，同时易用性和BigInt差不多，而源码行数只有BigInt的几乎一半，并且代码也更加整洁。对比细节：[Integer vs BigInt](./Documents/Integer%20vs%20BigInt/Integer%20vs%20BigInt.md)

### 4. 开源目的

- 为开源世界做一点微小的贡献。里面的代码对于许多初学数据结构的新手来说，能有一定的帮助。
- 期待有更多人加入，或者提出改进意见，一起把库进一步完善。

### 5. C版本

这个是C++版本的，目标是像Python的内置类型一样好用又优雅。

我还写了C版本的： [MDS (GitHub)](https://github.com/chen-qingyu/MDS) [MDS (Gitee)](https://gitee.com/ChobitsY/mds) ，目标是实现完整而标准的数据结构。
