cmake_minimum_required(VERSION 3.10)

project(Bakery)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
file(GLOB_RECURSE lib_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER lib_sources EXCLUDE REGEX "test_")
add_library(bakery SHARED ${lib_sources})

enable_testing()
file(GLOB_RECURSE tests_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER tests_sources INCLUDE REGEX "test_")
message(${tests_sources})
add_executable(run-tests ${tests_sources})
target_link_libraries(run-tests bakery boost_filesystem boost_regex boost_system gmp gmpxx)

add_test(NAME tests COMMAND run-tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS bakery DESTINATION lib)
install(DIRECTORY src/ DESTINATION include/bakery FILES_MATCHING REGEX .*\.hpp)
