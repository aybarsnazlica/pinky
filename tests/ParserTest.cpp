#include <vector>

#include "TestFramework.h"

namespace pinky::test {

const std::vector<TestCase> &RegisteredTests() {
  static const std::vector<TestCase> tests = {};
  return tests;
}

} // namespace pinky::test