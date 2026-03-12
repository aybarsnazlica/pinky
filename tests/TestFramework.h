#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace pinky::test {

class TestFailure : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

using TestFunction = void (*)();

struct TestCase {
  const char *Name;
  TestFunction Function;
};

const std::vector<TestCase> &RegisteredTests();

template <typename T, typename = void>
struct IsStreamInsertable : std::false_type {};

template <typename T>
struct IsStreamInsertable<
    T, std::void_t<decltype(std::declval<std::ostringstream &>()
                            << std::declval<const T &>())>> : std::true_type {};

template <typename T>
void AppendValue(std::ostringstream &stream, const T &value) {
  if constexpr (IsStreamInsertable<T>::value) {
    stream << value;
  } else {
    stream << "<value unavailable>";
  }
}

template <typename Actual, typename Expected>
void ExpectEqual(const Actual &actual, const Expected &expected,
                 const std::string &message) {
  if (!(actual == expected)) {
    std::ostringstream stream;
    stream << message << " Expected: ";
    AppendValue(stream, expected);
    stream << ", actual: ";
    AppendValue(stream, actual);
    throw TestFailure(stream.str());
  }
}

inline void ExpectTrue(bool condition, const std::string &message) {
  if (!condition) {
    throw TestFailure(message);
  }
}

} // namespace pinky::test