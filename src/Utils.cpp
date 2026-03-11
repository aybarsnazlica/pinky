#include "Utils.h"

#include <algorithm>
#include <sstream>

namespace pinky {
std::string PrintPrettyAST(const std::string &astText) {
  std::ostringstream builder;
  int indentation = 0;
  bool newline = false;

  for (char ch : astText) {
    switch (ch) {
      case '(':
        builder << ch << '\n';
        indentation += 2;
        newline = true;
        break;
      case ')':
        if (!newline) {
          builder << '\n';
        }
        indentation -= 2;
        builder << std::string(
            static_cast<std::size_t>(std::max(indentation, 0)), ' ');
        builder << ch;
        newline = true;
        break;
      default:
        if (newline) {
          builder << std::string(
              static_cast<std::size_t>(std::max(indentation, 0)), ' ');
        }
        builder << ch;
        newline = false;
        break;
    }
  }

  return builder.str();
}

std::string Stringify(bool value) {
  return value ? "true" : "false";
}

std::string Stringify(float value) {
  if (value == static_cast<long long>(value)) {
    return std::to_string(static_cast<long long>(value));
  }
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

std::string Stringify(double value) {
  if (value == static_cast<long long>(value)) {
    return std::to_string(static_cast<long long>(value));
  }
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

std::string Stringify(const char *value) {
  return value == nullptr ? "<nil>" : std::string(value);
}

std::string Stringify(const std::string &value) {
  return value;
}
} // namespace pinky
