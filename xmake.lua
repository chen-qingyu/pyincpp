add_rules("mode.debug", "mode.release")
set_languages("cxx17")
add_requires("gtest", {configs = {main = true}})
add_requires("eigen")
add_requires("mds")
if is_plat("windows") then -- for MSVC
    add_cxflags("/utf-8")
    add_ldflags("/subsystem:console")
end

target("tests")
    set_kind("binary")
    add_packages("gtest")
    add_files("tests/*.cpp")

target("examples")
    set_license("GPL-3.0")
    set_kind("binary")
    add_packages("eigen")
    add_packages("mds")
    add_files("examples/*.cpp")
