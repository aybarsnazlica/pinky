cmake_minimum_required(VERSION 4.1)
project(pinky)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

enable_testing()

add_library(pinky_lib
	src/Lexer.cpp
	src/Model.cpp
	src/Token.cpp
	src/Utils.cpp
)

target_include_directories(pinky_lib PUBLIC include)

add_executable(pinky src/main.cpp)
target_compile_definitions(pinky PRIVATE PINKY_SEPARATE_COMPILATION)
target_link_libraries(pinky PRIVATE pinky_lib)

install(
	TARGETS pinky
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_executable(lexer_tests
	tests/TestMain.cpp
	tests/LexerTest.cpp
)
target_link_libraries(lexer_tests PRIVATE pinky_lib)

add_executable(model_tests
	tests/TestMain.cpp
	tests/ModelTest.cpp
)
target_link_libraries(model_tests PRIVATE pinky_lib)

add_executable(parser_tests
	tests/TestMain.cpp
	tests/ParserTest.cpp
)
target_link_libraries(parser_tests PRIVATE pinky_lib)

add_test(NAME lexer_tests COMMAND lexer_tests)
add_test(NAME parser_tests COMMAND parser_tests)
add_test(NAME model_tests COMMAND model_tests)
