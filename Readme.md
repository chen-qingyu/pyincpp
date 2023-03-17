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
// 字符串转浮点数，支持 inf 和 nan
String(".1e-2").to_decimal(); // 0.1e-2
// 字符串转整数，支持 2-36 进制
String("-0101").to_integer(2); // -5
// 字符串追加
String("hello") += " world!"; // "hello world!"
// 字符串重复
String("hello! ") *= 2; // "hello! hello! "
// 字符串替换
String("hahaha").replace("a", "ooow~ "); // "hooow~ hooow~ hooow~ "
// 字符串切片
String("12345").slice(0, 5, 2); // "135"
// 字符串分割
String("one, two, three").split(", "); // List<String>({"one", "two", "three"})
// 字符串合并
String(".").join(List<String>({"192", "168", "0", "1"})); // "192.168.0.1"

// 列表索引，支持负数
List<int>({1, 2, 3, 4, 5})[-1]; // 5
// 列表遍历
List<int>({1, 2, 3, 4, 5}).traverse([](int& x) { x *= 2; }); // [2, 4, 6, 8, 10]
// 列表去重
List<int>({1, 2, 3, 1, 2, 3, 1, 2, 3}).uniquify(); // [1, 2, 3]
// 列表排序，稳定排序，默认从小到大，可自定义比较器
List<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}).sort([](const int& e1, const int& e2) { return e1 > e2; }); // [9, 8, 7, 6, 5, 4, 3, 2, 1, 0]

// 双端队列尾部入栈，头部/尾部入栈/出栈/引用都支持
Deque<int>({1, 2, 3, 4}).push_back(5); // <1, 2, 3, 4, 5>
// 双端队列向右移位，很生动形象有木有！
Deque<int>({1, 2, 3, 4, 5}) >>= 1; // <5, 1, 2, 3, 4>

// 整数幂取模，非常快
Integer("1024").pow(Integer("1024"), Integer("100")); // 1024^1024 % 100 == 76
// 整数求阶乘
Integer("5").factorial().factorial(); // (5!)! == 668950291344912705758811805409037258675274633313802981029567135230163355...

// 集合求交集，交、并、差、对称差（异或）都支持
Set<int>({1, 2, 3, 4, 5}) & Set<int>({1, 3, 5, 7, 9}); // {1, 3, 5}

// 字典根据键赋值
Map<String, int>({{"one", 1}, {"two", 2}, {"three", 3}})["one"] = 1111; // {"one": 1111, "two": 2, "three": 3}
// 字典取值集合
Map<int, String>({{"one", 1}, {"two", 2}, {"three", 3}}).values(); // {1, 2, 3}

// 元组索引，返回类型可以不同所以使用模板函数
get<2>(Tuple<int, double, char>(1, 2.5, 'A');); // 'A'
// 元组取剩余部分，底层是指针转换，非常快
Tuple<int, double, char>(1, 2.5, 'A').rest(); // (2.5, 'A')

// 任意嵌套多层容器也没问题
Map<String, List<Integer>> map = {
    {"first", {123, 456}}, {"second", {789}}, {"second", {0}}, {"third", {123456789, 5}}};
    // {"first": [123, 456], "second": [789], "third": [123456789, 5]}
map.size(); // 3
map.keys(); // {"first", "second", "third"}
map["third"][-1].factorial(); // 120
```

### 3. 开发历史

最开始是用的C语言开发，目的是学习数据结构。然后2021年开始尝试用C++实现一些方便的容器类，只当是练手。后来经过了几次重构，确定了目标和目的，2023年开始正式开发。

说明一下关于`inline`：为了源码简洁性，我最后决定一律采用inline的风格。一般不会有问题，除非对程序体积有很高的要求。刚开始我是把声明和定义分开写的，但这是模板，没法分成两个文件，所以我在一个文件里分开写，一部分函数定义前面加上inline，但是这样最后写完了看起来非常冗长，一大堆的template typename inline，在看了Java源码后考虑再三决定全部写在类里面，也就是默认inline的形式。inline只是对编译器的请求而非要求，不能inline的函数（比如有递归的函数）编译器是不会执行inline的。

开发完了`Integer`类后和GitHub上一个有三百多star的大整数类`BigInt`做了比较，结论是 mdspp::Integer 的性能综合来看更快，同时易用性和BigInt差不多，而源码行数只有BigInt的几乎一半，并且代码也更加整洁。对比细节：[Integer vs BigInt](./Documents/Integer%20vs%20BigInt/Integer%20vs%20BigInt.md)

我这个项目用到了FPGA里面的独热码思想结合有限状态机，还用到了模板元编程在编译期递归实现任意可变模板参数，听着很厉害，但是不赚钱，也没多少人真的会用，属于自娱自乐。可我创造就是快乐，创造就是意义（反正我不缺钱——饿不死）。

### 4. 开源目的

- 为开源世界做一点微小的贡献。里面的代码对于许多初学数据结构的新手来说，能有一定的帮助。
- 期待有更多人加入，或者提出改进意见，一起把库进一步完善。

### 5. C版本

这个是C++版本的，目标是像Python的内置类型一样好用又优雅。

我还写了C版本的： [MDS (GitHub)](https://github.com/chen-qingyu/MDS) [MDS (Gitee)](https://gitee.com/ChobitsY/mds) ，目标是实现完整而标准的数据结构。
