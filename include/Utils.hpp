#pragma once

#include <sstream>
#include <string>

namespace pinky {
constexpr char White[] = "\u001b[0m";
constexpr char Blue[] = "\u001b[94m";
constexpr char Cyan[] = "\u001b[96m";
constexpr char Green[] = "\u001b[92m";
constexpr char Yellow[] = "\u001b[93m";
constexpr char Red[] = "\u001b[91m";

std::string PrintPrettyAST(const std::string &astText);

std::string Stringify(bool value);

std::string Stringify(float value);

std::string Stringify(double value);

std::string Stringify(const char *value);

std::string Stringify(const std::string &value);

template <typename T> std::string Stringify(const T &value) {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}
} // namespace pinky
