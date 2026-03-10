#include "Lexer.hpp"
#include "Errors.hpp"

namespace pinky {
namespace {
bool IsDigit(const char ch) {
  return std::isdigit(static_cast<unsigned char>(ch)) != 0;
}

bool IsAlpha(const char ch) {
  return std::isalpha(static_cast<unsigned char>(ch)) != 0;
}

bool IsAlphaNumeric(const char ch) {
  return IsAlpha(ch) || IsDigit(ch);
}

std::string QuoteChar(const char ch) {
  switch (ch) {
    case '\0':
      return "'\\0'";
    case '\n':
      return "'\\n'";
    case '\t':
      return "'\\t'";
    case '\r':
      return "'\\r'";
    case '\'':
      return "'\\\''";
    case '\\':
      return "'\\\\'";
    default:
      return std::string("'") + ch + "'";
  }
}
} // namespace

Lexer::Lexer(std::string source) : Source(std::move(source)) {}

char Lexer::Advance() {
  char ch = Source[Current];
  ++Current;
  return ch;
}

char Lexer::Peek() const {
  if (Current >= Source.size()) {
    return '\0';
  }
  return Source[Current];
}

char Lexer::Lookahead(const int n) const {
  const auto Index = static_cast<long long>(Current) + n;
  if (Index < 0 || Index >= static_cast<long long>(Source.size())) {
    return '\0';
  }
  return Source[static_cast<std::size_t>(Index)];
}

bool Lexer::Match(const char expected) {
  if (Current >= Source.size()) {
    return false;
  }
  if (Source[Current] != expected) {
    return false;
  }
  ++Current;
  return true;
}

void Lexer::HandleNumber() {
  while (IsDigit(Peek())) {
    Advance();
  }

  if (Peek() == '.' && IsDigit(Lookahead(1))) {
    Advance();
    while (IsDigit(Peek())) {
      Advance();
    }
    AddToken(TokenType::Float);
    return;
  }

  AddToken(TokenType::Integer);
}

void Lexer::HandleString(const char startQuote) {
  while (Peek() != startQuote && Current < Source.size()) {
    Advance();
  }

  if (Current >= Source.size()) {
    throw LexingError(LineNumber, "Unterminated string.");
  }

  Advance();
  AddToken(TokenType::String);
}

void Lexer::HandleIdentifier() {
  while (IsAlphaNumeric(Peek()) || Peek() == '_') {
    Advance();
  }

  const std::string Text = Source.substr(TokenStart, Current - TokenStart);
  const auto Keyword = Keywords().find(Text);
  if (Keyword != Keywords().end()) {
    AddToken(Keyword->second);
    return;
  }

  AddToken(TokenType::Identifier);
}

void Lexer::AddToken(TokenType Kind) {
  Tokens.push_back(
      Token{Kind, Source.substr(TokenStart, Current - TokenStart), LineNumber});
}

std::vector<Token> Lexer::Tokenize() {
  TokenStart = 0;
  Current = 0;
  LineNumber = 1;
  Tokens.clear();

  while (Current < Source.size()) {
    TokenStart = Current;
    const char ch = Advance();

    switch (ch) {
      case '\n':
        ++LineNumber;
        break;
      case ' ':
      case '\t':
      case '\r':
        break;
      case '(':
        AddToken(TokenType::LParen);
        break;
      case ')':
        AddToken(TokenType::RParen);
        break;
      case '{':
        AddToken(TokenType::LCurly);
        break;
      case '}':
        AddToken(TokenType::RCurly);
        break;
      case '[':
        AddToken(TokenType::LSquar);
        break;
      case ']':
        AddToken(TokenType::RSquar);
        break;
      case '.':
        AddToken(TokenType::Dot);
        break;
      case ',':
        AddToken(TokenType::Comma);
        break;
      case '+':
        AddToken(TokenType::Plus);
        break;
      case '*':
        AddToken(TokenType::Star);
        break;
      case '^':
        AddToken(TokenType::Caret);
        break;
      case '/':
        AddToken(TokenType::Slash);
        break;
      case ';':
        AddToken(TokenType::Semicolon);
        break;
      case '?':
        AddToken(TokenType::Question);
        break;
      case '%':
        AddToken(TokenType::Mod);
        break;
      case '-':
        if (Match('-')) {
          while (Peek() != '\n' && Current < Source.size()) {
            Advance();
          }
        } else {
          AddToken(TokenType::Minus);
        }
        break;
      case '=':
        if (Match('=')) {
          AddToken(TokenType::EQEQ);
        } else {
          AddToken(TokenType::EQ);
        }
        break;
      case '~':
        if (Match('=')) {
          AddToken(TokenType::NE);
        } else {
          AddToken(TokenType::Not);
        }
        break;
      case '<':
        if (Match('=')) {
          AddToken(TokenType::LE);
        } else {
          AddToken(TokenType::LT);
        }
        break;
      case '>':
        if (Match('=')) {
          AddToken(TokenType::GE);
        } else {
          AddToken(TokenType::GT);
        }
        break;
      case ':':
        if (Match('=')) {
          AddToken(TokenType::Assign);
        } else {
          AddToken(TokenType::Colon);
        }
        break;
      case '"':
      case '\'':
        HandleString(ch);
        break;
      default:
        if (IsDigit(ch)) {
          HandleNumber();
        } else if (IsAlpha(ch) || ch == '_') {
          HandleIdentifier();
        } else {
          throw LexingError(LineNumber, "Error at " + QuoteChar(ch) +
                                            ": Unexpected character.");
        }
        break;
    }
  }

  return Tokens;
}
} // namespace pinky
