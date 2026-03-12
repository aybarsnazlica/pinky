#include "TestFramework.h"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  const auto &tests = pinky::test::RegisteredTests();

  try {
    if (argc == 2) {
      const std::string test_name = argv[1];
      for (const auto &test : tests) {
        if (test_name == test.Name) {
          test.Function();
          return EXIT_SUCCESS;
        }
      }

      std::cerr << "Unknown test: " << test_name << '\n';
      return EXIT_FAILURE;
    }

    for (const auto &test : tests) {
      std::cout << "Running " << test.Name << '\n';
      test.Function();
    }
  } catch (const std::exception &error) {
    std::cerr << "Test failed with exception: " << error.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Test failed with an unknown exception.\n";
    return EXIT_FAILURE;
  }

  std::cout << "All tests passed.\n";
  return EXIT_SUCCESS;
}