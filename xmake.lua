set_license("GPL-3.0")
add_rules("mode.debug", "mode.release")
set_languages("cxx17")
add_requires("catch2")
add_requires("gtest", {configs = {main = true}})
add_requires("eigen")
add_requires("pytype")
if is_plat("windows") then -- for MSVC
    add_cxflags("/utf-8")
    add_ldflags("/subsystem:console")
end

target("tests")
    set_kind("binary")
    add_packages("catch2")
    add_files("tests/*.cpp|benchmark.cpp")

target("examples")
    set_kind("binary")
    add_packages("eigen")
    add_packages("pytype")
    add_files("examples/*.cpp")

target("benchmark")
    set_kind("binary")
    add_packages("pytype")
    add_packages("catch2")
    add_files("tests/benchmark.cpp")
