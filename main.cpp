#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Lexer.hpp"
#include "Utils.hpp"

namespace {
constexpr const char *UsageMessage = "Usage: pinky [--verbose] <filename>";

std::string ReadFile(const std::string &path) {
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    throw std::runtime_error("failed to open file: " + path);
  }

  std::ostringstream buffer;
  buffer << input.rdbuf();
  return buffer.str();
}

int Run(const std::vector<std::string> &args) {
  bool verbose = false;
  std::vector<std::string> positional;
  bool parsingFlags = true;

  for (const auto &arg : args) {
    if (parsingFlags && arg == "--") {
      parsingFlags = false;
      continue;
    }

    if (parsingFlags && !arg.empty() && arg[0] == '-') {
      if (arg == "--verbose") {
        verbose = true;
        continue;
      }

      throw std::runtime_error(UsageMessage);
    }

    positional.push_back(arg);
  }

  if (positional.size() != 1) {
    throw std::runtime_error(UsageMessage);
  }

  const std::string source = ReadFile(positional[0]);
  pinky::Lexer lexer(source);
  const auto tokens = lexer.Tokenize();

  if (verbose) {
    std::cout << pinky::Green << "***************************************"
              << pinky::White << '\n';
    std::cout << pinky::Green << "SOURCE:" << pinky::White << '\n';
    std::cout << pinky::Green << "***************************************"
              << pinky::White << '\n';
    std::cout << source;
    if (source.empty() || source.back() != '\n') {
      std::cout << '\n';
    }

    std::cout << pinky::Green << "***************************************"
              << pinky::White << '\n';
    std::cout << pinky::Green << "TOKENS:" << pinky::White << '\n';
    std::cout << pinky::Green << "***************************************"
              << pinky::White << '\n';
    for (const auto &token : tokens) {
      std::cout << token.DebugString() << '\n';
    }
  }

  return 0;
}
} // namespace

int main(int argc, char *argv[]) {
  try {
    std::vector<std::string> args;
    args.reserve(static_cast<std::size_t>(argc > 0 ? argc - 1 : 0));
    for (int i = 1; i < argc; ++i) {
      args.emplace_back(argv[i]);
    }
    return Run(args);
  } catch (const std::exception &error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
