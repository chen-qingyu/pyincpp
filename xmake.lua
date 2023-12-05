set_languages("cxx17")
add_requires("gtest")
if is_plat("windows") then -- for MSVC
    add_cxflags("/utf-8")
    add_ldflags("/subsystem:console")
end

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
