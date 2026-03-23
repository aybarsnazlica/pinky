#include "pinky/Parser.h"

#include "pinky/Token.h"

namespace pinky {
Parser::Parser(std::vector<Token> tokens, std::size_t current)
    : Tokens(std::move(tokens)), Current(current) {}

Token Parser::Advance() {
  Token token = Tokens[Current];
  ++Current;
  return token;
}
} // namespace pinky