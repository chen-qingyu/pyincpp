# PyType

_A C++ type library that is as easy to use as Python built-in types._

[中文](./readme_zh.md)

### 1. Attribute

- Name: PyType.
- Language: C++, minimum compatible version: C++17.
- Goal: Write a C++ type library that is as easy to use as Python built-in types.
- Module: List, Set, Map, Integer, String, Tuple, Deque, Fraction
- Style: Most follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html), a small portion adopt their own style based on project size and source code simplicity considerations.
- Test: Using [Catch2](https://github.com/catchorg/Catch2) for unit testing and benchmark testing and ensure that all tests passed.
- Security: Using [Dr. Memory](https://drmemory.org/) checked to ensure there were no safety issues.
- Document: Using [Doxygen](https://www.doxygen.nl/) to generate documents.
- Build: Using [XMake](https://xmake.io/) to build.

### 2. Feature

- Simple: Stay simple, stay young. While ensuring usability and robustness, try to be concise and easy to maintain and read.
- Friendly: Provides many convenient functions. For example, String class provides replace, split, find and other operations like Python's str, and List class and String class both support negative subscripts like python.
- Robust: A secure expansion mechanism to prevent overflow. There are corresponding checks for the addition, deletion, modification, and inspection of containers. Checking will have an impact on performance, but this library is not pursuing performance, but simplicity, usability, and robustness.
- Elegance: With my careful design, it can be used as conveniently as Python's built-in types.
- Efficiency: Performance comparison was conducted on the parts that overlap with the standard library, and the [benchmark results](./tests/benchmark.cpp) showed that the performance was not poor.

### 3. Usage

Because C++ templates are used, they are all provided in the form of header files (. hpp), header only.

Very convenient to use:

Since pytype has already entered the official xmake repository, you only need to add it in the project configuration: `add_requires("pytype")` and then you can directly `#include <pytype/pytype.hpp>` in the source code.

Alternatively, simply and roughly copy the entire sources directory to the project source directory and then directly `#include "pytype.hpp"`.

There are a total of 8 classes, refer to the 8 commonly used classes in Python:

| Type in PyType | Type in Python       |
| -------------- | -------------------- |
| `List<T>`      | `list`               |
| `Set<T>`       | `set`                |
| `Map<K, V>`    | `dict`               |
| `Integer`      | `int`                |
| `String`       | `str`                |
| `Tuple<Ts...>` | `tuple`              |
| `Deque<T>`     | `collections.deque`  |
| `Fraction`     | `fractions.Fraction` |

Some simple examples:

```cpp
using namespace pytype;

// List index, supports negative numbers
List<int>({1, 2, 3, 4, 5})[-1] // 5
// List traversal
List<int>({1, 2, 3, 4, 5}).map([](int& x) { x *= 2; }) // [2, 4, 6, 8, 10]
// List uniquify
List<int>({1, 2, 3, 1, 2, 3, 1, 2, 3}).uniquify() // [1, 2, 3]
// List sorting, stable sorting, default from small to large, customizable comparator
List<int>({1, 2, 3, 4, 5, 6, 7, 8, 9}).sort([](const int& e1, const int& e2) { return e1 > e2; }) // [9, 8, 7, 6, 5, 4, 3, 2, 1]

// Adding Elements to Set
Set<int>({1, 2, 3, 4}) += 5 // {1, 2, 3, 4, 5}
// Intersection of sets, supports intersection, union, difference, and symmetric difference
Set<int>({1, 2, 3, 4, 5}) & Set<int>({1, 3, 5, 7, 9}) // {1, 3, 5}

// Map assigns values
Map<String, int>({{"one", 1}, {"two", 2}, {"three", 3}})["one"] = 1111 // {"one": 1111, "two": 2, "three": 3}
// Get Map values
Map<int, String>({{"one", 1}, {"two", 2}, {"three", 3}}).values() // {1, 2, 3}

// Modular Integer powers, very fast
Integer("1024").pow("1024", "100") // 76
// Factorial of Integer
Integer("5").factorial().factorial() // 668950291344912705758811805409037258675274633313802981029567135230163355...

// Convert String to floating-point number, supports inf and nan
String(".1e-2").to_decimal() // 0.1e-2
// Convert String to integer, supports base 2-36
String("-0101").to_integer(2) // -5
// String append
String("hello") += " world!" // "hello world!"
// String repetition
String("hello! ") *= 2 // "hello! hello! "
// String replace
String("hahaha").replace("a", "ooow~ ") // "hooow~ hooow~ hooow~ "
// String slice
String("12345").slice(0, 5, 2) // "135"
// String split
String("one, two, three").split(", ") // ["one", "two", "three"]
// String join
String(".").join({"192", "168", "0", "1"}) // "192.168.0.1"
// String format
String("I'm {}, {} years old.").format("Alice", 18) // "I'm Alice, 18 years old."

// Tuple index, return type different, so template function is used
Tuple<int, double, char>(1, 2.5, 'A').get<2>() // 'A'
// Taking the remaining part of a Tuple, the underlying layer is pointer conversion, which is very fast
Tuple<int, double, char>(1, 2.5, 'A').rest() // (2.5, 'A')

// Deque tail push, supports both head and tail push, pop, and element reference
Deque<int>({1, 2, 3, 4}).push_back(5) // <1, 2, 3, 4, 5>
// Deque shifts to the right, very vivid!
Deque<int>({1, 2, 3, 4, 5}) >>= 1 // <5, 1, 2, 3, 4>

// Fraction multiplication, supports addition, subtraction, multiplication, division and modulo
Fraction(1, 2) * Fraction(1, 2) // 1/4
// Convert Fraction to String
Fraction(3, -6).to_string() // "-1/2"

// Arbitrarily nested multiple layers of types
Map<String, List<Integer>> map = {{"first", {123, 456}}, {"second", {789}}, {"second", {0}}, {"third", {"12345678987654321", 5}}}
    // {"first": [123, 456], "second": [789], "third": [12345678987654321, 5]}
map.size() // 3
map.keys() // {"first", "second", "third"}
map["third"][-1].factorial() // 120
```

### 4. Advantage

The advantage of PyType is that it combines the high performance of C++ with the ease of use of Python, and can also be easily combined with other libraries, for example:

```cpp
/*
Combining pytype::Fraction with Eigen library to display accurate matrix operation results.
*/
using Matrix = Eigen::Matrix<pytype::Fraction, 2, 2>; // compiling with boost::rational will fail

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
boost::rational vs pytype::Fraction
*/
boost::rational<int> r1(1, 2), r2(1, 3), r3(1, 4), r4(1, 5);
pytype::Fraction f1(1, 2), f2(1, 3), f3(1, 4), f4(1, 5);

std::cout << ((r1 + r2) * r3 / r4) << std::endl; // 25/24
// std::cout << ((r1 + r2) * r3 % r4) << std::endl; // boost::rational does not support operator%
std::cout << ((f1 + f2) * f3 / f4) << std::endl; // 25/24
std::cout << ((f1 + f2) * f3 % f4) << std::endl; // 1/120

/*
std::tuple vs boost::tuple vs pytype::Tuple
*/
auto t1 = std::make_tuple(1, 1.5, 'A', "hello", std::tuple<std::tuple<>, std::tuple<>>({}, {}));
auto t2 = boost::make_tuple(1, 1.5, 'A', "hello", boost::tuple<boost::tuple<>, boost::tuple<>>({}, {}));
auto t3 = pytype::make_tuple(1, 1.5, 'A', "hello", pytype::Tuple<pytype::Tuple<>, pytype::Tuple<>>({}, {}));

// std::cout << t1 << std::endl; // std::tuple does not support operator<<
std::cout << t2 << std::endl; // (1 1.5 A hello (() ()))
std::cout << t3 << std::endl; // (1, 1.5, A, hello, ((), ()))
```

### 5. History

It was originally developed in C to learn data structures. Then the 2021 started experimenting with some handy container classes in C++, just for practice. After several reconstructions, the goals and objectives were determined, and formal development began in 2023.

A word about inline: for the sake of source brevity, I finally decided to adopt the inline style. There is usually no problem unless there is a high requirement for program size. At first I wrote the declaration and the definition separately, but this is a template, so I can't split it into two files, so I'm going to write it in one file, and I'm going to put inline in front of some of the function definitions, but it ended up being a verbose bunch of "template typename inline". After reading the Java source code, I decided to write it all in the class, as the default inline. Inline is just a request to the compiler, not a requirement, and functions that can not be inline (such as recursive functions) are not executed by the compiler.

After developing the Integer class, I compared it to [BigInt](https://github.com/faheel/BigInt), a big integer class on GitHub with more than 300 stars, and the conclusion is that pytype::Integer has a faster overall performance, while its ease of use is similar to BigInt, and the number of source code lines is almost half of BigInt, and the code is also cleaner.

In this project, I used the One-hot code idea in FPGA combined with the finite state machine. I also used template meta-programming to recursively implement any variable template parameters at compile time. It sounds great, but it doesn't make money, not many people really use, just belong to self-entertainment now, but creation is happiness, creation is meaning.
