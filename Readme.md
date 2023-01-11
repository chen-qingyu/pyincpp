# MineDataStructure++

*名字灵感来源于我喜欢的游戏——Minecraft和本项目的编程语言——C++*

*缩写为MDSPP*


### 1. 基本属性

- 语言：采用标准C++语言编写，最低兼容版本： ISO C++14 。
- 目的：提供一个简洁、好用、安全的C++数据结构库。
- 目标：像Python的内置类型一样好用又优雅。
- 模块：List, String, Deque, Set, Map, Integer.
- 简洁：Stay simple, stay young. 在保证好用和安全的前提下，尽量简洁，便于维护和阅读。
- 好用：提供了许多方便的函数，比如String类提供了像Python的str那样的替换、分割、查找等操作，比如List类和String类都支持像Python那样的负数下标等等。当然，功能与标准库重合的部分的性能肯定比不上标准库，但是简单用用是完全没问题的。
- 安全：安全的扩容机制，防止溢出。对容器的增删改查都有相应的检查。这么多检查，肯定会对性能有一定影响，但是这个库追求的并不是性能，而是简洁，好用和安全。
- 优雅：经过我的精心设计，用起来可以像Python的内置类型一样方便。
- 风格：大部分遵循 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) ，小部分基于项目规模和源码简洁性的考虑采用自己的风格。
- 测试：每次提交之前使用 [GoogleTest](https://github.com/google/googletest) 进行测试，确保测试通过。

### 2. 模块说明

```
Type        Implement               Python
List<T>     array list              list
Deque<T>    doubly linked list      collections.deque
String      List<char>              str
Set<T>      AVL tree or skip list   set
Map<K, V>   Set<Pair<K, V>>         dict
Integer     self                    int
```

### 3. 开发历史

最开始是用的C语言开发，目的是学习数据结构。然后2021年开始尝试用C++实现一些方便的容器类，只当是练手。后来经过了几次重构，确定了目标和目的，2023年开始正式开发。

说明一下关于`inline`：为了源码简洁性，我最后决定一律采用inline的风格。一般不会有问题，除非对程序体积有很高的要求。
刚开始我是把声明和定义分开写的，但这是模板，没法分成两个文件，所以我在一个文件里分开写，一部分函数定义前面加上inline，但是这样最后写完了看起来非常冗长，一大堆的template typename inline，在看了Java源码后考虑再三决定全部写在类里面，也就是默认inline的形式。inline只是对编译器的请求而非要求，不能inline的函数（比如有递归的函数）编译器是不会执行inline的。

### 4. 开源目的

- 为开源世界做一点微小的贡献。里面的代码对于许多初学数据结构的新手来说，能有一定的帮助。
- 期待有大佬加入，或者提出改进意见，一起把库进一步完善。

### 5. C版本

这个是C++版本的数据结构库，目标是像Python的内置类型一样好用又优雅。

我还写了C版本的： [MDS (GitHub)](https://github.com/chen-qingyu/MDS) [MDS (Gitee)](https://gitee.com/ChobitsY/mds) ，目标是实现完整而标准的数据结构。

### 6. 内存安全

使用 Dr. Memory 进行了内存检查：

```
Dr. Memory version 2.5.0

FINAL SUMMARY:

DUPLICATE ERROR COUNTS:

SUPPRESSIONS USED:

NO ERRORS FOUND:
      0 unique,     0 total unaddressable access(es)
      0 unique,     0 total uninitialized access(es)
      0 unique,     0 total invalid heap argument(s)
      0 unique,     0 total GDI usage error(s)
      0 unique,     0 total handle leak(s)
      0 unique,     0 total warning(s)
      0 unique,     0 total,      0 byte(s) of leak(s)
      0 unique,     0 total,      0 byte(s) of possible leak(s)
```
