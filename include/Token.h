#pragma once

#include <string>
#include <unordered_map>

namespace pinky {
enum class TokenType {
  LParen,
  RParen,
  LCurly,
  RCurly,
  LSquar,
  RSquar,
  Comma,
  Dot,
  Plus,
  Minus,
  Star,
  Slash,
  Caret,
  Mod,
  Colon,
  Semicolon,
  Question,
  Not,
  GT,
  LT,
  EQ,
  GE,
  LE,
  NE,
  EQEQ,
  Assign,
  GTGT,
  LTLT,
  Identifier,
  String,
  Integer,
  Float,
  If,
  Then,
  Else,
  True,
  False,
  And,
  Or,
  Local,
  While,
  Do,
  For,
  Func,
  Null,
  End,
  Print,
  Println,
  Ret,
};

struct Token {
  TokenType Kind;
  std::string Lexeme;
  int Line;

  std::string DebugString() const;
};

const std::unordered_map<std::string, TokenType> &Keywords();

std::string ToString(TokenType Kind);
} // namespace pinky
