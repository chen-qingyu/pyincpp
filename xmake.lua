set_languages("cxx17")
add_requires("gtest")
add_cxflags("/utf-8") -- for MSVC
add_ldflags("/subsystem:console") -- for MSVC

target("Tests")
    set_kind("binary")
    add_files("Tests/*.cpp")
    add_packages("gtest")

target("Examples")
    set_kind("binary")
    add_files("Examples/*.cpp")

target("Integer_vs_BigInt")
    set_kind("binary")
    add_files("Documents/Integer_vs_BigInt/*.cpp")
    add_packages("gtest")
