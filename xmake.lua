set_license("GPL-3.0")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")
add_requires("catch2")

target("tests")
    set_kind("binary")
    add_packages("catch2")
    add_files("tests/*.cpp|benchmark.cpp")

target("benchmark")
    set_kind("binary")
    add_packages("catch2")
    add_files("tests/benchmark.cpp")
