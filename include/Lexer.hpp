#pragma once

#include <string>
#include <vector>

#include "Token.hpp"

namespace pinky {
struct LexerTestAccess;

class Lexer {
public:
  explicit Lexer(std::string source);

  char Advance();

  char Peek() const;

  char Lookahead(int n) const;

  bool Match(char expected);

  void HandleNumber();

  void HandleIdentifier();

  void AddToken(TokenType Kind);

  void HandleString(char startQuote);

  std::vector<Token> Tokenize();

private:
  friend struct LexerTestAccess;

  std::string Source;
  std::size_t TokenStart = 0;
  std::size_t Current = 0;
  int LineNumber = 1;
  std::vector<Token> Tokens;
};
} // namespace pinky
