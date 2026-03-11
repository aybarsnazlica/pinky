#include "Token.h"

#include <iomanip>
#include <sstream>

namespace pinky {

std::string Token::DebugString() const {
  std::ostringstream stream;
  stream << '(' << ToString(Kind) << ", " << std::quoted(Lexeme) << ", " << Line
         << ')';
  return stream.str();
}

const std::unordered_map<std::string, TokenType> &Keywords() {
  static const std::unordered_map<std::string, TokenType> KeywordMap = {
      {"if", TokenType::If},       {"else", TokenType::Else},
      {"then", TokenType::Then},   {"true", TokenType::True},
      {"false", TokenType::False}, {"and", TokenType::And},
      {"or", TokenType::Or},       {"local", TokenType::Local},
      {"while", TokenType::While}, {"do", TokenType::Do},
      {"for", TokenType::For},     {"func", TokenType::Func},
      {"null", TokenType::Null},   {"end", TokenType::End},
      {"print", TokenType::Print}, {"println", TokenType::Println},
      {"ret", TokenType::Ret},
  };

  return KeywordMap;
}

std::string ToString(const TokenType Kind) {
  switch (Kind) {
    case TokenType::LParen:
      return "TOK_LPAREN";
    case TokenType::RParen:
      return "TOK_RPAREN";
    case TokenType::LCurly:
      return "TOK_LCURLY";
    case TokenType::RCurly:
      return "TOK_RCURLY";
    case TokenType::LSquar:
      return "TOK_LSQUAR";
    case TokenType::RSquar:
      return "TOK_RSQUAR";
    case TokenType::Comma:
      return "TOK_COMMA";
    case TokenType::Dot:
      return "TOK_DOT";
    case TokenType::Plus:
      return "TOK_PLUS";
    case TokenType::Minus:
      return "TOK_MINUS";
    case TokenType::Star:
      return "TOK_STAR";
    case TokenType::Slash:
      return "TOK_SLASH";
    case TokenType::Caret:
      return "TOK_CARET";
    case TokenType::Mod:
      return "TOK_MOD";
    case TokenType::Colon:
      return "TOK_COLON";
    case TokenType::Semicolon:
      return "TOK_SEMICOLON";
    case TokenType::Question:
      return "TOK_QUESTION";
    case TokenType::Not:
      return "TOK_NOT";
    case TokenType::GT:
      return "TOK_GT";
    case TokenType::LT:
      return "TOK_LT";
    case TokenType::EQ:
      return "TOK_EQ";
    case TokenType::GE:
      return "TOK_GE";
    case TokenType::LE:
      return "TOK_LE";
    case TokenType::NE:
      return "TOK_NE";
    case TokenType::EQEQ:
      return "TOK_EQEQ";
    case TokenType::Assign:
      return "TOK_ASSIGN";
    case TokenType::GTGT:
      return "TOK_GTGT";
    case TokenType::LTLT:
      return "TOK_LTLT";
    case TokenType::Identifier:
      return "TOK_IDENTIFIER";
    case TokenType::String:
      return "TOK_STRING";
    case TokenType::Integer:
      return "TOK_INTEGER";
    case TokenType::Float:
      return "TOK_FLOAT";
    case TokenType::If:
      return "TOK_IF";
    case TokenType::Then:
      return "TOK_THEN";
    case TokenType::Else:
      return "TOK_ELSE";
    case TokenType::True:
      return "TOK_TRUE";
    case TokenType::False:
      return "TOK_FALSE";
    case TokenType::And:
      return "TOK_AND";
    case TokenType::Or:
      return "TOK_OR";
    case TokenType::Local:
      return "TOK_LOCAL";
    case TokenType::While:
      return "TOK_WHILE";
    case TokenType::Do:
      return "TOK_DO";
    case TokenType::For:
      return "TOK_FOR";
    case TokenType::Func:
      return "TOK_FUNC";
    case TokenType::Null:
      return "TOK_NULL";
    case TokenType::End:
      return "TOK_END";
    case TokenType::Print:
      return "TOK_PRINT";
    case TokenType::Println:
      return "TOK_PRINTLN";
    case TokenType::Ret:
      return "TOK_RET";
  }

  return "TOK_UNKNOWN";
}

} // namespace pinky