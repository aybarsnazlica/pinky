#include "pinky/Parser.h"

#include "pinky/Errors.h"
#include "pinky/Token.h"

namespace pinky {
Parser::Parser(std::vector<Token> tokens, std::size_t current)
    : Tokens(std::move(tokens)), Current(current) {}

Token Parser::Advance() {
  Token token = Tokens[Current];
  ++Current;
  return token;
}

Token Parser::Peek() const {
  return Tokens[Current];
}

bool Parser::IsNext(TokenType expectedType) const {
  if (Current >= Tokens.size()) {
    return false;
  }
  return Tokens[Current].Kind == expectedType;
}

Token Parser::Expect(TokenType expectedType) {
  if (Current >= Tokens.size()) {
    throw ParseError(PreviousToken().Line, "Found " + PreviousToken().Lexeme +
                                               " at the end of parsing");
  } else if (Peek().Kind == expectedType) {
    Token token = Advance();
    return token;
  } else {
    throw ParseError(Peek().Line, "Expected " + ToString(expectedType) +
                                      " found " + Peek().Lexeme);
  }
}

Token Parser::PreviousToken() const {
  return Tokens[Current - 1];
}

bool Parser::Match(TokenType expectedType) {
  if (Current >= Tokens.size()) {
    return false;
  }
  if (Peek().Kind != expectedType) {
    return false;
  }
  ++Current;
  return true;
}

} // namespace pinky