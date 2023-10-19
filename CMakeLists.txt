cmake_minimum_required(VERSION 3.14)
project(MyDataStructure)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE tests "Tests/*.cpp")
add_executable(Tests ${tests})

target_link_libraries(
  Tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Tests)

file(GLOB_RECURSE examples "Examples/*.cpp")
add_executable(Examples ${examples})


file(GLOB_RECURSE documents "Documents/*.cpp")
add_executable(Documents ${documents})

target_link_libraries(
  Documents
  GTest::gtest_main
)

gtest_discover_tests(Documents)
