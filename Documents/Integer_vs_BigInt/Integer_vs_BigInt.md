### mds::Integer vs BigInt

这里我简单比较一下 GitHub 上一个有三百多 star 的大整数类 [BigInt](https://github.com/faheel/BigInt) 和我写的大整数类 [mds](https://github.com/chen-qingyu/MDS)::Integer

从**性能**，**易用性**，**源码行数**和**源码整洁度**来比较

#### 性能：综合来看 mds::Integer 更快

- BigInt 的构造和底层用的是 std::string ，而 mds::Integer 用的是我自己写的容器库：构造是 mds::String ，底层是 mds::List
- 测试代码： [TestTime.cpp](./TestTime.cpp) 测试结果放在了代码最后的注释部分
- 这个测试只是粗略的定性，大概对比一下速度。详细的定量分析，欢迎有兴趣的朋友帮我测试
- 测试日期： 2023.03.02 两个都是最新版 (2023.10.19 更新)

#### 易用性：差不多

| All functions     | mds::Integer | BigInt |
| ----------------- | ------------ | ------ |
| `+ - * / %`       | `yes`        | `yes`  |
| `+= -= *= /= %=`  | `yes`        | `yes`  |
| `+ - (unary)`     | `yes`        | `yes`  |
| `abs`             | `yes`        | `yes`  |
| `gcd lcm`         | `yes`        | `yes`  |
| `== != < <= > >=` | `yes`        | `yes`  |
| `pow`             | `yes`        | `yes`  |
| `++ --`           | `yes`        | `yes`  |
| `digits`          | `yes`        | `no`   |
| `factorial`       | `yes`        | `no`   |
| `sqrt`            | `yes`        | `yes`  |
| `>> << (IO)`      | `yes`        | `yes`  |
| `random`          | `no`         | `yes`  |

#### 源码行数：mds::Integer 几乎只有 BigInt 的一半

- mds::Integer: 1048 行
- BigInt: 1964 行

#### 源码整洁度：请看源码自行感受

[Integer.hpp](../../Sources/Integer.hpp)

[BigInt.hpp](./BigInt.hpp)

#### 总结

mds::Integer 的性能综合来看更快，同时易用性和 BigInt 差不多，而源码行数只有 BigInt 的几乎一半，并且代码也更加整洁。

BigInt 只是这一个类就算一个项目，而我的 MDS 里像这样的类，一共七个： List, Set, Map, Integer, String, Tuple, Deque.

当然，那个库在我开始开发这个类的初期阶段也给了一定的帮助（我想说青出于蓝而胜于蓝……其实我还是应届生，有空才写着玩一玩，大佬轻喷）。

而一个 300 多个 star，一个 0 个 star……

一方面是因为那个项目存在的时间比这个久，MDS 是今年才开始正式开发的（虽然早就计划了半年之久），另一方面是因为我没有怎么宣传。

以上。
