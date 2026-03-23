#pragma once

#include <stdexcept>
#include <string>

#include "pinky/Utils.h"

namespace pinky {
class LexingError : public std::runtime_error {
public:
  LexingError(int line, std::string message)
      : std::runtime_error(Red + std::string("[Line ") + std::to_string(line) +
                           "]: " + message + " " + White),
        LineNumber(line), MessageText(std::move(message)) {}

  int Line() const noexcept {
    return LineNumber;
  }
  const std::string &Message() const noexcept {
    return MessageText;
  }

private:
  int LineNumber;
  std::string MessageText;
};

class ParseError : public std::runtime_error {
public:
  ParseError(int line, std::string message)
      : std::runtime_error(Red + std::string("[Line ") + std::to_string(line) +
                           "]: " + message + " " + White),
        LineNumber(line), MessageText(std::move(message)) {}

  int Line() const noexcept {
    return LineNumber;
  }
  const std::string &Message() const noexcept {
    return MessageText;
  }

private:
  int LineNumber;
  std::string MessageText;
};

class RuntimeError : public std::runtime_error {
public:
  RuntimeError(int line, std::string message)
      : std::runtime_error(Red + std::string("[Line ") + std::to_string(line) +
                           "]: " + message + " " + White),
        LineNumber(line), MessageText(std::move(message)) {}

  int Line() const noexcept {
    return LineNumber;
  }
  const std::string &Message() const noexcept {
    return MessageText;
  }

private:
  int LineNumber;
  std::string MessageText;
};
} // namespace pinky
