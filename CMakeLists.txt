cmake_minimum_required(VERSION 4.1)
project(pinky)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(pinky_lib
	src/Lexer.cpp
	src/Token.cpp
	src/Utils.cpp
)

target_include_directories(pinky_lib PUBLIC include)

add_executable(pinky src/main.cpp)
target_compile_definitions(pinky PRIVATE PINKY_SEPARATE_COMPILATION)
target_link_libraries(pinky PRIVATE pinky_lib)

add_executable(pinky_tests tests/LexerTest.cpp)
target_link_libraries(pinky_tests PRIVATE pinky_lib)

add_test(NAME pinky_tests COMMAND pinky_tests)
